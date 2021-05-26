#ifndef Tree_H
#define Tree_H

#include "node.h"

template< typename KEY, typename VALUE >
class Tree
{
private:
    Node* root;
    int n;
    Tree(int _n);
public:
    VALUE search(KEY key) const;
    void insert(KEY key, VALUE value);
};

template< typename KEY, typename VALUE >
Tree<KEY, VALUE> :: Tree(int _n){
    this->n = _n;
    this->root = new InnerNode(_n);
}

template< typename KEY, typename VALUE >
VALUE Tree<KEY, VALUE> :: search(KEY key) const { //fő osztályban a keresés
    VALUE v = root.search(key);
    return v;
}

template< typename KEY, typename VALUE >
void Tree<KEY, VALUE> :: insert(KEY key, VALUE value){ //fő osztályban a beszúrás
    Additional_node<KEY> a_node = this->root.add(key, value);
    if(a_node.nodehelper != nullptr) //új gyökércsúcs keletkezett
        this->root = a_node.nodehelper;
}

#endif