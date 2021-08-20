#ifndef NODE_H
#define NODE_H

#include <limits>

template < typename KEY, typename VALUE >
class Node
{
    protected:
        int key_count_;
        int children_count_;
        KEY *keys;
        VALUE *values;
        Node(int _key_count, int _children_count);
    public:
        virtual VALUE search(KEY key) const;
        virtual void remove(KEY key);
};

template < typename KEY, typename VALUE >
Node<KEY, VALUE>::Node(int _key_count, int _children_count){
    key_count_ = _key_count;
    children_count_ = _children_count;
    *keys = new KEY(key_count_);
    *values = new VALUE(children_count_);
    for(int i=0; i<key_count_; ++i){
        keys[i] = std::numeric_limits<KEY>::max();
        values[i] = nullptr;
    }
    values[key_count_] = nullptr;
}

#endif