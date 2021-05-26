#ifndef LeafNode_H
#define LeafNode_H

#include "node.h"

//keys[1];
//values[1];

template < typename KEY, typename VALUE >
class LeafNode : public Node <KEY, VALUE>
{
    private:
        KEY key;
        VALUE value;
    public:
        LeafNode(KEY _key, VALUE _value) : key(_key), value(_value) {}
        Additional_node<KEY> add(KEY key, VALUE value);
        VALUE Node::search(KEY key) override;
        ~LeafNode(){
            delete this;
        }
};

template < typename KEY, typename VALUE >
Additional_node<KEY> LeafNode<KEY, VALUE>::add(KEY key, VALUE value){ //új levél, visszatérés feljebbi szintre
    Additional_node<KEY> a_node;
    a_node.nodehelper = new LeafNode(key, value);
    a_node.keyhelper = key;
    return a_node;
}

template < typename KEY, typename VALUE >
VALUE LeafNode<KEY, VALUE>::search(KEY key) override { //visszatérés VALUE-val, ha volt ilyen elem
    if(this->key == key)
        return this->value;
    return nullptr;
}

#endif