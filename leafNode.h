#ifndef LEAFNODE_H
#define LEAFNODE_H

#include "node.h"

template < typename KEY, typename VALUE >
class LeafNode : public Node <KEY, VALUE>
{
    private:
        KEY *key_;
        VALUE *value_;
        LeafNode(KEY _key, VALUE _value);
        /*++ Azt gondolom, hogy itt meg kellene hívni az alaposztály (Node) konstruktorát. Mit adunk át n-ként? 
        Létre fognak jönni n és n-1 méretű tömbök, amiket végül nem használunk. 
        Azt javasolnám, hogy akkor a Node-ban lévő tömbök méretét állítsuk 1-re az itteni private tagváltozók helyett. 
        Akkor külön kell tudni állítani a Node-ban a két tömb méretét, tehát ne n és n-1 legyen, hanem két szám. 
        És azok itt 1 és 1, az InnerNode-ban pedig majd n és n-1 lesznek. ++*/
    public:
        AdditionalNode<KEY> add(KEY key, VALUE value);
        VALUE Node::search(KEY key) override;
        ~LeafNode();
        /*++ Szerintem delete this nem kell, az tulajdonképpen a destruktort hívná. Kicsit több erről:
        https://isocpp.org/wiki/faq/freestore-mgmt#delete-this ++*/
};

template < typename KEY, typename VALUE >
LeafNode<KEY, VALUE>::LeafNode(KEY _key, VALUE _value){
    this = new Node(1, 1);
    key_[0] = _key;
    value_[0] = _value;
}

template < typename KEY, typename VALUE >
AdditionalNode<KEY> LeafNode<KEY, VALUE>::add(KEY key, VALUE value){ //új levél, visszatérés feljebbi szintre
    AdditionalNode<KEY, VALUE> a_node;
    a_node.nodehelper_ = new LeafNode(key, value);
    a_node.keyhelper_ = key;
    return a_node;
}

template < typename KEY, typename VALUE >
VALUE LeafNode<KEY, VALUE>::search(KEY key) override { //visszatérés VALUE-val, ha volt ilyen elem
    if(this->key_[0] == key)
        return this->value_[0];
    return nullptr;
}

#endif