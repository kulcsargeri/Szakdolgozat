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

#endif