#ifndef NODE_H
#define NODE_H

#include <limits>

template < typename KEY, typename VALUE >
class Tree;

template < typename KEY, typename VALUE >
struct AdditionalNode;

template < typename KEY, typename VALUE >
class Node
{
    protected:
        // int key_count_;
        // int children_count_;
        // KEY *keys;
        // VALUE *values;
        // Node(int _key_count, int _children_count);
        Node() {}
    public:
        virtual VALUE search(KEY key) const = 0;
        virtual bool remove(KEY key) = 0;
        virtual AdditionalNode<KEY, VALUE> add(KEY key, VALUE value) = 0;
        virtual void ConvertToNewTree(Tree<KEY, VALUE>* tree) = 0;
        virtual bool IsKeyInTree(KEY key) const = 0;
        virtual void print(int space_count, std::ofstream& myfile) = 0;
        virtual bool GetKeyIsMaxAtIndex(int keyIndex) = 0;
        virtual KEY GetKeyAtIndex(int keyIndex) = 0;
        virtual Node<KEY,VALUE>* GetValueAtIndex(int vaueIndex) = 0;
        virtual void SetKeyAtIndex(KEY key, int keyIndex) = 0;
        virtual void SetValueAtIndex(Node<KEY,VALUE>* value, int valueIndex) = 0;
        virtual ~Node() = default;
};

template < typename KEY, typename VALUE >
struct AdditionalNode
{
    KEY keyhelper_ = 0;
    Node<KEY, VALUE>* nodehelper_ = nullptr;
};

// template < typename KEY, typename VALUE >
// Node<KEY, VALUE>::Node(int _key_count, int _children_count){
//     key_count_ = _key_count;
//     children_count_ = _children_count;
//     *keys = new KEY(key_count_);
//     *values = new VALUE(children_count_);
//     for(int i=0; i<key_count_; ++i){
//         keys[i] = std::numeric_limits<KEY>::max();
//         values[i] = nullptr;
//     }
//     values[key_count_] = nullptr;
// }

#endif