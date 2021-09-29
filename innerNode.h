#ifndef INNERNODE_H
#define INNERNODE_H

#include "node.h"

template < typename KEY, typename VALUE >
class Tree;

template < typename KEY, typename VALUE >
class InnerNode : public Node <KEY, VALUE>
{
    private:
        int key_count_;
        int children_count_;
        KEY *keys;
        Node<KEY, VALUE> **children;
        bool root_ = false;
        void FirstCopy(int &a, int b, AdditionalNode<KEY, VALUE> a_node);
        void SecondCopy(int& a, int b, int& innerind, InnerNode<KEY, VALUE>* innerhelper);
    public:
        InnerNode(int _key_count, int _children_count);
        AdditionalNode<KEY, VALUE> add(KEY key, VALUE value) override;
        virtual VALUE search(KEY key) const override;
        virtual void remove(KEY key) override;
        void print() override;
        // TODO: Is it necessary? Only used when adding a guard leaf node. Maybe create a method?
        friend class Tree<KEY, VALUE>; 
};

template < typename KEY, typename VALUE >
InnerNode<KEY, VALUE>::InnerNode(int _key_count, int _children_count) : Node<KEY, VALUE>() {
    key_count_ = _key_count;
    children_count_ = _children_count;
    keys = new KEY[key_count_];
    children = new Node<KEY, VALUE>*[children_count_];
    for(int i=0; i<key_count_; ++i){
        keys[i] = std::numeric_limits<KEY>::max();
        children[i] = nullptr;
    }
    children[key_count_] = nullptr;
}


template < typename KEY, typename VALUE >
AdditionalNode<KEY, VALUE> InnerNode<KEY, VALUE>::add(KEY key, VALUE value){
    int i = 0;
    while(i<this->key_count_ && this->keys[i]<=key) //kulcs megkeresése
        ++i;
    AdditionalNode<KEY, VALUE> a_node = this->children[i]->add(key, value); //rekurzív hívások levélszintig
    if(a_node.nodehelper_ == nullptr) return a_node; //nem történt hasítás lejjebbi szinten  /*jobb oldal bal gyerek kell még*/
    if(this->keys[this->key_count_ - 1] == std::numeric_limits<KEY>::max()){ //nem telített a csúcs, nem fogunk hasítani, csak beillesztjük a kapott KEY, VALUE párt
        this->FirstCopy(i, this->key_count_, a_node);
        a_node.nodehelper_ = nullptr;
        a_node.keyhelper_ = 0;
        return a_node;
    }
    int innerind = 0; //hasítani fogunk
    InnerNode<KEY, VALUE>* innerhelper = new InnerNode(this->key_count_, this->children_count_);
    if(i>=(this->children_count_+1)/2){ //beillesztés a csúcs második felébe kerül, így másolás után tudjuk ezt megtenni
        int j=(this->children_count_+1)/2;
        if(i != j){ //nem a második fél első eleme a beszúrandó elem
            KEY keyhelper = this->keys[j];
            ++j;
            innerhelper->children[innerind] = this->children[j];
            this->children[j] = nullptr;
            SecondCopy(j, i, innerind, innerhelper);
            innerhelper->keys[innerind] = a_node.keyhelper_;
            innerhelper->children[innerind+1] = a_node.nodehelper_;
            ++innerind;
            a_node.keyhelper_ = keyhelper;
        } else { //második fél első eleme
            ++j;
            innerhelper->children[innerind] = a_node.nodehelper_;
            ++innerind;
        }
        SecondCopy(j, this->key_count_, innerind, innerhelper);
    }else{ //első félbe kell beilleszteni
        this->FirstCopy(i, (this->children_count_+1)/2, a_node);
        a_node.keyhelper_ = this->keys[i];
        ++i;
        innerhelper->children[innerind] = this->children[i];
        this->children[i] = nullptr;
        SecondCopy(i, this->key_count_, innerind, innerhelper);
    }
    a_node.nodehelper_ = innerhelper;
    if(this->root_){ //ha a gyökércsúcsot hasítottuk, akkor új gyökércsúcsot kell létrehoznunk
        InnerNode<KEY, VALUE>* nroot = new InnerNode(this->key_count_, this->children_count_);
        nroot->root_ = true;
        nroot->keys[0] = a_node.keyhelper_;
        nroot->children[0] = this;
        nroot->children[1] = a_node.nodehelper_;
        a_node.nodehelper_ = nroot;
        this->root_ = false;
    }
    return a_node;
}

template < typename KEY, typename VALUE >
VALUE InnerNode<KEY, VALUE>::search(KEY key) const { //rekurzív lineáris keresés
    for(int i=0; i<this->key_count_; ++i){
        if(this->keys[i]>key)
            return this->children[i]->search(key);
    }
    return this->children[this->key_count_]->search(key);
}

template < typename KEY, typename VALUE >
void InnerNode<KEY, VALUE>::remove(KEY key) {
}

template < typename KEY, typename VALUE >
void InnerNode<KEY, VALUE>::FirstCopy(int& a, int b, AdditionalNode<KEY, VALUE> a_node){ //első fél eltolása végéig/feléig beszúrás utántól
    KEY k = this->keys[a];
    Node<KEY, VALUE>* v = this->children[a+1];
    this->keys[a] = a_node.keyhelper_;
    this->children[a+1] = a_node.nodehelper_;
    while(++a<b && k != std::numeric_limits<KEY>::max()){
        KEY khelper = this->keys[a];
        Node<KEY, VALUE>* vhelper = this->children[a+1];
        this->keys[a] = k;
        this->children[a+1] = v;
        k = khelper;
        v = vhelper;
    }
}

template < typename KEY, typename VALUE >
void InnerNode<KEY, VALUE>::SecondCopy(int &a, int b, int &innerind, InnerNode<KEY, VALUE>* innerhelper){ //második fél másolása a beszúrásig/-tól
    while(a<b){
        innerhelper->keys[innerind] = this->keys[a];
        innerhelper->children[innerind+1] = this->children[a+1];
        this->keys[a] = std::numeric_limits<KEY>::max();
        this->children[a+1] = nullptr;
        ++a;
        ++innerind;
    }
}

template < typename KEY, typename VALUE >
void InnerNode<KEY, VALUE>::print(){
    std::cout<<"(";
    children[0]->print();
    for(int i=0; i<key_count_ && keys[i] != std::numeric_limits<KEY>::max(); ++i){
        std::cout<<keys[i]<<" ";
        children[i+1]->print();
    }
    std::cout<<")";
}

#endif