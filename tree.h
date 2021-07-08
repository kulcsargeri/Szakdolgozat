#ifndef TREE_H
#define TREE_H

#include "node.h"

template< typename KEY, typename VALUE >
class Tree
{
private:
    Node<KEY, VALUE>* root_;
    int children_count_;
    Tree(int _children_count);
public:
    VALUE search(KEY key) const;
    void insert(KEY key, VALUE value);
};

template< typename KEY, typename VALUE >
Tree<KEY, VALUE> :: Tree(int _children_count){
    children_count_ = _children_count;
    this->root_ = new InnerNode(_children_count-1, _children_count);
}

template< typename KEY, typename VALUE >
VALUE Tree<KEY, VALUE> :: search(KEY key) const { //fő osztályban a keresés
    VALUE v = root_.search(key);
    return v;
}

template< typename KEY, typename VALUE >
void Tree<KEY, VALUE> :: insert(KEY key, VALUE value){ //fő osztályban a beszúrás
    AdditionalNode<KEY, VALUE> a_node = this->root_.add(key, value);
    if(a_node.nodehelper_ != nullptr) //új gyökércsúcs keletkezett
        this->root_ = a_node.nodehelper_;
}

#endif

/*++ Az nagyon tetszik, hogy a Node alaposztályban a VALUE template paramétert úgy is használhatjuk, hogy egy másik Node-ra mutat, és úgy is, hogy egy értéket tárol. 
Szerintem ez elég szép. Az insert visszatérési értéke is jó lett így szerintem. ++*/