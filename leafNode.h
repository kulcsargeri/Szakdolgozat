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
        bool remove(KEY key) override;
        void ConvertToNewTree(Tree<KEY, VALUE>* tree) override;
        bool IsKeyInTree(KEY key) const override;
        void print(int space_count, std::ofstream& myfile) override;
        bool GetKeyIsMaxAtIndex(int keyIndex) override;
        KEY GetKeyAtIndex(int keyIndex) override;
        Node<KEY, VALUE>* GetValueAtIndex(int valueIndex) override;
        void SetKeyAtIndex(KEY key, int keyIndex) override;
        void SetValueAtIndex(Node<KEY, VALUE>* value, int valueIndex) override;
        ~LeafNode();
};

template < typename KEY, typename VALUE >
AdditionalNode<KEY, VALUE> LeafNode<KEY, VALUE>::add(KEY key, VALUE value){ //új levél, visszatérés feljebbi szintre
    AdditionalNode<KEY, VALUE> a_node;
    if(key > this->key_){
        a_node.nodehelper_ = new LeafNode(key, value);
        a_node.keyhelper_ = key;
    }else{
        a_node.nodehelper_ = new LeafNode(this->key_, this->value_);
        a_node.keyhelper_ = this->key_;
        this->key_ = key;
        this->value_ = value;
    }
    return a_node;
}

template < typename KEY, typename VALUE >
VALUE LeafNode<KEY, VALUE>::search(KEY key) const { //visszatérés VALUE-val, ha volt ilyen elem
    if(key != key_)
        throw "Key is not in tree!";
    return this->value_;
}

template < typename KEY, typename VALUE >
void LeafNode<KEY, VALUE>::print(int space_count, std::ofstream& myfile){
    for(int i=0; i<space_count; ++i){
        myfile<<" ";
    }
    myfile<<"- ["<<key_<<"]\n";
}

template < typename KEY, typename VALUE >
bool LeafNode<KEY, VALUE>::remove(KEY key) { 
    return false;
}

template< typename KEY, typename VALUE >
void LeafNode<KEY, VALUE>::ConvertToNewTree(Tree<KEY, VALUE>* tree){
    tree->insert(this->key_, this->value_);
}

template< typename KEY, typename VALUE >
bool LeafNode<KEY, VALUE>::IsKeyInTree(KEY key) const {
    return key == key_;
}

template < typename KEY, typename VALUE >
bool LeafNode<KEY, VALUE>::GetKeyIsMaxAtIndex(int keyIndex) {
    return true;
}

template < typename KEY, typename VALUE >
KEY LeafNode<KEY, VALUE>::GetKeyAtIndex(int keyIndex) 
{
    return key_;
}

template < typename KEY, typename VALUE >        
Node<KEY,VALUE>* LeafNode<KEY, VALUE>::GetValueAtIndex(int vaueIndex) 
{
    return this;
}

template < typename KEY, typename VALUE >
void LeafNode<KEY, VALUE>::SetKeyAtIndex(KEY key, int keyIndex) 
{

}

template < typename KEY, typename VALUE >
void LeafNode<KEY, VALUE>::SetValueAtIndex(Node<KEY,VALUE>* value, int valueIndex) 
{
    
}

#endif