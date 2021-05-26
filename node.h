#ifndef Node_H
#define Node_H

template < typename KEY>
struct Additional_node
{
    KEY keyhelper = 0;
    Node* nodehelper = nullptr;
};

template < typename KEY, typename VALUE >
class Node
{
    protected:
        int n;
        KEY keys[n-1];
        VALUE values[n];
        Node(int _n);
    public:
        VALUE search(KEY key) const;
        void remove(KEY key);
};

template < typename KEY, typename VALUE >
Node<KEY, VALUE>::Node(int _n){
    this.n = _n;
    for(int i=0; i<this->n-1; ++i){
        keys[i] = __INT_MAX__; //KEY_MAX??
        values[i] = nullptr;
    }
    values[n-1] = nullptr;
}

template < typename KEY, typename VALUE >
VALUE Node<KEY, VALUE>::search(KEY key) const { //rekurzív lineáris keresés
    for(int i=0; i<this->n-1; ++i){
        if(keys[i]>key)
            return values[i].search(key);
    }
    return values[this->n-1].search(key);
}

#endif

// 4 6      8
//2 4 6    7 8