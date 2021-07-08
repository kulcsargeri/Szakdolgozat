#ifndef INNERNODE_H
#define INNERNODE_H

#include "node.h"

template < typename KEY, typename VALUE >
void SecondCopy(int& a, int b, int& innerind, InnerNode<KEY, VALUE> innerhelper);
//++ Style: írjuk ki teljes szavakkal: second_copy, vagy SecondCopy ++//

template < typename KEY, typename VALUE >
class InnerNode : public Node <KEY, VALUE>
/*++Ez nem így nézne ki:
class InnerNode : public Node<KEY, Node*>
?++*/
{
    private:
        bool root_ = false;
        void InnerNode<KEY, VALUE>::FirstCopy(int &a, int b);
        InnerNode(int _key_count, int _children_count) : Node(_key_count, _children_count) {}
    public:
        AdditionalNode<KEY, VALUE> add(KEY key, VALUE value);
        VALUE search(KEY key) const;
};

template < typename KEY, typename VALUE >
AdditionalNode<KEY, VALUE> InnerNode<KEY, VALUE>::add(KEY key, VALUE value){
    int i = 0;
    while(i<key_count_ && keys[i]<=key) //kulcs megkeresése
        ++i;
    AdditionalNode<KEY, VALUE> a_node = values[i].add(key, value); //rekurzív hívások levélszintig
    if(a_node.nodehelper_ == nullptr) return a_node; //nem történt hasítás lejjebbi szinten  /*jobb oldal bal gyerek kell még*/
    if(keys[key_count_] == std::numeric_limits<KEY>::max()){ //nem telített a csúcs, nem fogunk hasítani, csak beillesztjük a kapott KEY, VALUE párt
        this->FirstCopy(i, key_count_);
        a_node.nodehelper_ = nullptr;
        a_node.keyhelper_ = 0;
        return a_node;
    }
    int innerind = 0; //hasítani fogunk
    InnerNode<KEY, VALUE> innerhelper = new InnerNode(key_count_, children_count_);
    if(i>=(children_count_+1)/2){ //beillesztés a csúcs második felébe kerül, így másolás után tudjuk ezt megtenni
        int j=(children_count_+1)/2;
        if(i != j){ //nem a második fél első eleme a beszúrandó elem
            KEY keyhelper = keys[j];
            ++j;
            innerhelper.values[innerind] = values[j];
            values[j] = nullptr;
            SecondCopy(j, i, innerind, innerhelper);
            innerhelper.keys[innerind] = a_node.keyhelper_;
            innerhelper.values[innerind+1] = a_node.nodehelper_;
            ++innerind;
            a_node.keyhelper_ = keyhelper;
        } else { //második fél első eleme
            ++j;
            innerhelper.values[innerind] = a_node.nodehelper_;
            ++innerind;
        }
        SecondCopy(j, key_count_, innerind, innerhelper);
    }else{ //első félbe kell beilleszteni
        this->FirstCopy(i, (children_count_+1)/2);
        a_node.keyhelper_ = keys[i];
        ++i;
        innerhelper.values[innerind] = values[i];
        values[i] = nullptr;
        SecondCopy(i, key_count_, innerind, innerhelper);
    }
    a_node.nodehelper_ = innerhelper;
    if(this->root_){ //ha a gyökércsúcsot hasítottuk, akkor új gyökércsúcsot kell létrehoznunk
        InnerNode<KEY, VALUE> nroot = new InnerNode(key_count_ ,children_count_);
        nroot.root_ = true;
        nroot.keys[0] = a_node.keyhelper_;
        nroot.values[0] = this;
        nroot.values[1] = a_node.nodehelper_;
        a_node.nodehelper_ = nroot;
        this->root_ = false;
    }
    return a_node;
}

template < typename KEY, typename VALUE >
VALUE InnerNode<KEY, VALUE>::search(KEY key) const { //rekurzív lineáris keresés
    for(int i=0; i<key_count_; ++i){
        if(keys[i]>key)
            return values[i].search(key);
    }
    return values[key_count_].search(key);
}

template < typename KEY, typename VALUE >
void FirstCopy(int& a, int b){ //első fél eltolása végéig/feléig beszúrás utántól
    KEY k = keys[a];
    VALUE v = values[a+1];
    keys[a] = a_node.keyhelper_;
    values[a+1] = a_node.nodehelper_;
    while(++a<b && k != std::numeric_limits<KEY>::max()){
        KEY khelper = keys[a];
        VALUE vhelper = values[a+1];
        keys[a] = k;
        values[a+1] = v;
        k = khelper;
        v = vhelper;
    }
}

template < typename KEY, typename VALUE >
void SecondCopy(int &a, int b, int &innerind, InnerNode<KEY, VALUE> innerhelper){ //második fél másolása a beszúrásig/-tól
    while(a<b){
        innerhelper.keys[innerind] = keys[a];
        innerhelper.values[innerind+1] = values[a+1];
        keys[a] = std::numeric_limits<KEY>::max();
        values[a+1] = nullptr;
        ++a;
        ++innerind;
    }
}

#endif