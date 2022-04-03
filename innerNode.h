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
        bool leaf_ = false;
        void FirstCopy(int &a, int b, AdditionalNode<KEY, VALUE> a_node);
        void SecondCopy(int& a, int b, int& innerind, InnerNode<KEY, VALUE>* innerhelper);
    public:
        InnerNode(int _key_count, int _children_count);
        AdditionalNode<KEY, VALUE> add(KEY key, VALUE value) override;
        VALUE search(KEY key) const override;
        bool remove(KEY key) override;
        void ConvertToNewTree(Node<KEY, VALUE>* root) override;
        void print() override;
        bool GetKeyIsMaxAtIndex(int keyIndex) override;
        KEY GetKeyAtIndex(int keyIndex) override;
        Node<KEY, VALUE>* GetValueAtIndex(int valueIndex) override;
        void SetKeyAtIndex(KEY key, int keyIndex) override;
        void SetValueAtIndex(Node<KEY, VALUE>* value, int valueIndex) override;
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
    if(this->leaf_) innerhelper->leaf_ = true;
    if(i>=(this->children_count_+1)/2){ //beillesztés a csúcs második felébe kerül, így másolás után tudjuk ezt megtenni
        int j=(this->children_count_+1)/2;
        if(i != j){ //nem a második fél első eleme a beszúrandó elem
            KEY keyhelper = this->keys[j];
            this->keys[j] = std::numeric_limits<KEY>::max();
            ++j;
            innerhelper->children[innerind] = this->children[j];
            this->children[j] = nullptr;
            SecondCopy(j, i, innerind, innerhelper);
            innerhelper->keys[innerind] = a_node.keyhelper_;
            innerhelper->children[innerind+1] = a_node.nodehelper_;
            ++innerind;
            a_node.keyhelper_ = keyhelper;
        } else { //második fél első eleme
            innerhelper->children[innerind] = a_node.nodehelper_;
        }
        SecondCopy(j, this->key_count_, innerind, innerhelper);
    }else{ //első félbe kell beilleszteni
        //* this->FirstCopy(i, (this->children_count_+1)/2, a_node); *// 
        //ez elromlott mert két dolgot is vissza kellene adni, hogy folytathassuk
        int b = (this->children_count_+1)/2;
        KEY k = this->keys[i];
        Node<KEY, VALUE>* v = this->children[i+1];
        this->keys[i] = a_node.keyhelper_;
        this->children[i+1] = a_node.nodehelper_;
        KEY khelper;
        Node<KEY, VALUE>* vhelper;
        while(++i<b && k != std::numeric_limits<KEY>::max()){
            khelper = this->keys[i];
            vhelper = this->children[i+1];
            this->keys[i] = k;
            this->children[i+1] = v;
            k = khelper;
            v = vhelper;
        }
        innerhelper->keys[innerind] = this->keys[i];
        innerhelper->children[innerind] = vhelper;
        ++innerind;
        a_node.keyhelper_ = khelper;
        this->keys[i] = std::numeric_limits<KEY>::max();
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
bool InnerNode<KEY, VALUE>::remove(KEY key) {
    int ind = 0;
    while(ind<this->key_count_ && this->keys[ind]<=key){
        ++ind;
    }
    bool split = this->children[ind]->remove(key);
    if(leaf_){
        this->children[ind]->~Node();
        if(ind == 0)
            this->children[ind] = this->children[ind+1];
        for(ind; ind+1<this->key_count_; ++ind){
            this->keys[ind] = this->keys[ind+1];
            this->children[ind+1] = this->children[ind+2];
        }
        this->keys[key_count_-1] = std::numeric_limits<KEY>::max();
        this->children[children_count_-1] = nullptr;
        if(this->keys[(key_count_-1)/2] == std::numeric_limits<KEY>::max())
            return true;
        return false;
    }
    if(!split) return false;
    if(ind != 0){
        if(this->children[ind-1]->GetKeyIsMaxAtIndex((key_count_-1)/2)){//
            int from_ind = key_count_-1;
            while(this->children[ind-1]->GetKeyIsMaxAtIndex(from_ind)){//
                --from_ind;
            }
            KEY k = this->keys[ind-1];
            Node<KEY, VALUE>* v = this->children[ind]->GetValueAtIndex(0);//
            this->keys[ind-1] = this->children[ind-1]->GetKeyAtIndex(from_ind);//
            this->children[ind]->SetValueAtIndex(this->children[ind-1]->GetValueAtIndex(from_ind),0);//
            this->children[ind-1]->SetKeyAtIndex(std::numeric_limits<KEY>::max(),from_ind);//
            this->children[ind-1]->SetValueAtIndex(nullptr,from_ind);//
            for(int i=0; i<((key_count_-1)/2+1); ++i){
                 KEY khelper = this->children[ind]->GetKeyAtIndex(i);//
                 Node<KEY,VALUE>* vhelper = this->children[ind]->GetValueAtIndex(i+1);//
                 this->children[ind]->SetKeyAtIndex(k,i);//
                 this->children[ind]->SetValueAtIndex(v,i+1);//
                 k = khelper;
                 v = vhelper;
            }
        }
        return false;
    }
    if(ind != this->key_count_-1){
        if(!this->children[ind+1]->GetKeyIsMaxAtIndex((key_count_-1)/2)){//
            this->children[ind]->SetKeyAtIndex(this->keys[ind],(key_count_-1)/2-1);//
            this->keys[ind] = this->children[ind+1]->GetKeyAtIndex(0);//
            this->children[ind]->SetValueAtIndex(this->children[ind+1]->GetValueAtIndex(0),(key_count_-1)/2);//
            this->children[ind+1]->SetValueAtIndex(this->children[ind+1]->GetValueAtIndex(1),0);//
            int i = 0;
            for(; i<key_count_-1; ++i){
                this->children[ind+1]->SetKeyAtIndex(this->children[ind+1]->GetKeyAtIndex(i+1),i);//
                this->children[ind+1]->SetValueAtIndex(this->children[ind+1]->GetValueAtIndex(i+2),i+1);//
            }
            this->children[ind+1]->SetKeyAtIndex(std::numeric_limits<KEY>::max(),i);//
            this->children[ind+1]->SetValueAtIndex(nullptr,i+1);//
        }
        return false;
    }
    if(ind != this->key_count_-1){
        this->children[ind]->SetKeyAtIndex(this->keys[ind-1],(key_count_-1)/2-1);//
        this->children[ind]->SetValueAtIndex(this->children[ind+1]->GetValueAtIndex(0),(key_count_-1)/2);//
        for(int i=0; i<(this->key_count_+1)/2; ++i){//
            this->children[ind]->SetKeyAtIndex(this->children[ind+1]->GetKeyAtIndex(i),(key_count_-1)/2+i);//
            this->children[ind]->SetValueAtIndex(this->children[ind+1]->GetValueAtIndex(i+1),(key_count_-1)/2+1+i);//
        }
        for(; ind<key_count_; ++ind){
            this->keys[ind] = this->keys[ind+1];
            this->children[ind+1] = this->children[ind+2];
        }
        this->keys[ind] = std::numeric_limits<KEY>::max();
        this->children[ind+1] = nullptr;
    }
    return false;
}

template< typename KEY, typename VALUE >
void InnerNode<KEY, VALUE>::ConvertToNewTree(Node<KEY, VALUE>* root){
    for(int ind = key_count_-1; ind >= 0; --ind){
        if(this->keys[ind] == std::numeric_limits<KEY>::max()) continue;
        this->children[ind+1]->ConvertToNewTree(root);
        if(leaf_) this->children[ind+1]->~Node();
    }
    this->children[0]->ConvertToNewTree(root);
    this->children[0]->~Node();
    this->~InnerNode();
}

template < typename KEY, typename VALUE >
KEY InnerNode<KEY, VALUE>::GetKeyAtIndex(int keyIndex){
    return this->keys[keyIndex];
}

template < typename KEY, typename VALUE >
Node<KEY, VALUE>* InnerNode<KEY, VALUE>::GetValueAtIndex(int valueIndex){
    return this->children[valueIndex];
}

template < typename KEY, typename VALUE >
void InnerNode<KEY, VALUE>::SetKeyAtIndex(KEY key, int keyIndex){
    this->keys[keyIndex] = key;
}

template < typename KEY, typename VALUE >
void InnerNode<KEY, VALUE>::SetValueAtIndex(Node<KEY, VALUE>* value, int valueIndex){
    this->children[valueIndex] = value;
}

template < typename KEY, typename VALUE >
bool InnerNode<KEY, VALUE>::GetKeyIsMaxAtIndex(int keyIndex){
    return this->keys[keyIndex] == std::numeric_limits<KEY>::max();
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