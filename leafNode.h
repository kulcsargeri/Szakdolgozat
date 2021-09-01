#ifndef LEAFNODE_H
#define LEAFNODE_H

#include "node.h"

template < typename KEY, typename VALUE >
class LeafNode : public Node <KEY, VALUE>
{
    private:
        KEY key_;
        VALUE value_;
    public:
        LeafNode(KEY _key, VALUE _value) : key_(_key), value_(_value) {};
        AdditionalNode<KEY, VALUE> add(KEY key, VALUE value) override;
        VALUE search(KEY key) const override;
        virtual void remove(KEY key) override;
        ~LeafNode();
};

template < typename KEY, typename VALUE >
AdditionalNode<KEY, VALUE> LeafNode<KEY, VALUE>::add(KEY key, VALUE value){ //új levél, visszatérés feljebbi szintre
    AdditionalNode<KEY, VALUE> a_node;
    a_node.nodehelper_ = new LeafNode(key, value);
    a_node.keyhelper_ = key;
    return a_node;
}

template < typename KEY, typename VALUE >
VALUE LeafNode<KEY, VALUE>::search(KEY key) const { //visszatérés VALUE-val, ha volt ilyen elem
    if(this->key_ == key)
        return this->value_;
    return nullptr;
}

template < typename KEY, typename VALUE >
void LeafNode<KEY, VALUE>::remove(KEY key) { 
    
}

#endif