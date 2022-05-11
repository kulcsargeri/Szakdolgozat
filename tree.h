#ifndef TREE_H
#define TREE_H

#include "node.h"
#include "innerNode.h"
#include "leafNode.h"

template< typename KEY, typename VALUE >
class Tree
{
private:
    InnerNode<KEY, VALUE>* root_; // TODO: esetleg Node<KEY, VALUE>*
    int children_count_;
public:
    VALUE search(KEY key) const;
    void insert(KEY key, VALUE value);
    void remove(KEY key);
    void ChangeChildrenCount(int new_children_count);
    void structure();
    Tree(int _children_count);
};

template< typename KEY, typename VALUE >
Tree<KEY, VALUE>::Tree(int _children_count){
    children_count_ = _children_count;
    this->root_ = new InnerNode<KEY, VALUE>(_children_count-1, _children_count);
    this->root_->children_[0] = new LeafNode<KEY, VALUE>(std::numeric_limits<KEY>::max(), VALUE());
    this->root_->root_ = true;
    this->root_->leaf_ = true;
}

template< typename KEY, typename VALUE > //TODO: try catch nullptr
VALUE Tree<KEY, VALUE>::search(KEY key) const { //fő osztályban a keresés
    try{
        VALUE v = root_->search(key);
        return v;
    } catch(const char* msg){
        return msg;
    }
}

template< typename KEY, typename VALUE >
void Tree<KEY, VALUE>::insert(KEY key, VALUE value){ //fő osztályban a beszúrás
    AdditionalNode<KEY, VALUE> a_node = root_->add(key, value);
    if(a_node.nodehelper_ != nullptr){ //új gyökércsúcs keletkezett

        this->root_ = static_cast<InnerNode<KEY, VALUE>*>(a_node.nodehelper_);
    }
}

template< typename KEY, typename VALUE >
void Tree<KEY, VALUE>::remove(KEY key){
    if(this->root_->IsKeyInTree(key)){
        this->root_->remove(key);
        if(this->root_->GetKeyIsMaxAtIndex(0)){
            this->root_ = static_cast<InnerNode<KEY, VALUE>*>(this->root_->GetValueAtIndex(0));
            this->root_->root_ = true;
        }
        std::cout<<"Successful deletion\n";
    }
    else std::cout<<"Unseccessful deletion\n";
}

template< typename KEY, typename VALUE >
void Tree<KEY, VALUE>::ChangeChildrenCount(int new_children_count){
    Tree<KEY, VALUE>* tree = new Tree<KEY, VALUE>(new_children_count);
    this->root_->ConvertToNewTree(tree);
    delete this->root_;
    this->root_ = tree->root_;
    this->children_count_ = new_children_count;
}

template< typename KEY, typename VALUE >
void Tree<KEY, VALUE>::structure(){
    std::cout<<"keys_per_block: "<<this->children_count_-1<<"\n";
    std::cout<<"tree:\n";
    this->root_->print(2);
    std::cout<<"\n";
}

#endif