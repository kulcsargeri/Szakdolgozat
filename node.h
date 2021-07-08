#ifndef NODE_H
//++ csupa nagybetűvel szokás írni: #ifndef NODE_H ++//
#define NODE_H

template < typename KEY, typename VALUE>
struct AdditionalNode
/*++ A struct nevének furcsa írásmódja van. Igazodjunk egy C++ style guide-hoz, például: https://google.github.io/styleguide/cppguide.html#General_Naming_Rules
Eszerint: struct AdditionalNode
Ha ezt vesszük alapul, akkor a függvényneveket is módosítani kell. ++*/
{
    KEY keyhelper_ = 0;
    Node<KEY, VALUE>* nodehelper_ = nullptr;
};

template < typename KEY, typename VALUE >
class Node
{
    protected:
        int key_count_;
        int children_count_;
        KEY *keys;
        VALUE *values;
        /*++ Ez működik így? Lefordul? Csak mert az n a konstruktor közben kap értéket. 
        Biztos, hogy azután jönnek létre a tömbök? Másrészt, lehet, hogy a GCC-ben lefordul, de szerintem nem standard. 
        Szerintem pointerként kellene definiálni és dinamikusan foglalni. Viszont akkor figyelj majd, hogy a destruktorban szabadítsuk fel. ++*/
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
        /*++ Szerintem a this->n helyett simán lehetne n. És lehetne kifejezőbb neve, például: children_count
        Közben ahogy néztem a többi kódot, láttam, hogy ez nem feltétlenül a gyerekek száma. 
        És ahogy javasoltam, lehetne két méret: value_count és key_count. 
        Az InnerNode konstruktorában tudjuk ezeket állítani children_count és children_count - 1 -re ++*/
        keys[i] = std::numeric_limits<KEY>::max();
        values[i] = nullptr;
    }
    values[key_count_] = nullptr;
}

/*++ Ennek az implementációnak nem az InnerNode-ban lenne a helye? A Node-ban legyen ez a függvény (és az add, remove is) tisztán virtuális. 
Ezzel a Node egy absztrakt alaposztály lesz. ++*/

#endif