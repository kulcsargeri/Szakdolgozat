#ifndef InnerNode_H
#define InnerNode_H

#include "node.h"
#include "leafNode.h"

template < typename KEY, typename VALUE >
void snd_cpy(int& a, int b, int& innerind, InnerNode<KEY, VALUE> innerhelper);

template < typename KEY, typename VALUE >
class InnerNode : public Node <KEY, VALUE>
{
    private:
        bool root = false;
        void InnerNode<KEY, VALUE>::fst_cpy(int &a, int b);
    public:
        InnerNode(int _n) : Node(_n) {}
        Additional_node<KEY> add(KEY key, VALUE value);
};

template < typename KEY, typename VALUE >
Additional_node<KEY> InnerNode<KEY, VALUE>::add(KEY key, VALUE value){
    int i = 0;
    while(i<n-1 && keys[i]<=key) //kulcs megkeresése
        ++i;
    Additional_node<KEY> a_node = values[i].add(key, value); //rekurzív hívások levélszintig
    if(a_node.nodehelper == nullptr) return a_node; //nem történt hasítás lejjebbi szinten  /*jobb oldal bal gyerek kell még*/
    if(keys[n-1] == __INT_MAX__){ //nem telített a csúcs, nem fogunk hasítani, csak beillesztjük a kapott KEY, VALUE párt
        this->fst_cpy(i, n-1);
        a_node.nodehelper = nullptr;
        a_node.keyhelper = 0;
        return a_node;
    }
    int innerind = 0; //hasítani fogunk
    InnerNode<KEY, VALUE> innerhelper = new InnerNode(this->n);
    if(i>=(this->n+1)/2){ //beillesztés a csúcs második felébe kerül, így másolás után tudjuk ezt megtenni
        int j=(this->n+1)/2;
        if(i != j){ //nem a második fél első eleme a beszúrandó elem
            KEY keyhelper = keys[j];
            ++j;
            innerhelper.values[innerind] = values[j];
            values[j] = nullptr;
            snd_cpy(j, i, innerind, innerhelper);
            innerhelper.keys[innerind] = a_node.keyhelper;
            innerhelper.values[innerind+1] = a_node.nodehelper;
            ++innerind;
            a_node.keyhelper = keyhelper;
        } else { //második fél első eleme
            ++j;
            innerhelper.values[innerind] = a_node.nodehelper;
            ++innerind;
        }
        snd_cpy(j, this->n-1, innerind, innerhelper);
    }else{ //első félbe kell beilleszteni
        this->fst_cpy(i, (this->n+1)/2);
        a_node.keyhelper = keys[i];
        ++i;
        innerhelper.values[innerind] = values[i];
        values[i] = nullptr;
        snd_cpy(i, this->n-1, innerind, innerhelper);
    }
    a_node.nodehelper = innerhelper;
    if(this->root){ //ha a gyökércsúcsot hasítottuk, akkor új gyökércsúcsot kell létrehoznunk
        InnerNode<KEY, VALUE> nroot = new InnerNode(this->n);
        nroot.root = true;
        nroot.keys[0] = a_node.keyhelper;
        nroot.values[0] = this;
        nroot.values[1] = a_node.nodehelper;
        a_node.nodehelper = nroot;
        this->root = false;
    }
    return a_node;
}

template < typename KEY, typename VALUE >
void fst_cpy(int& a, int b){ //első fél eltolása végéig/feléig beszúrás utántól
    KEY k = keys[a];
    VALUE v = values[a+1];
    keys[a] = a_node.keyhelper;
    values[a+1] = a_node.nodehelper;
    while(++a<b && k != __INT_MAX__){
        KEY khelper = keys[a];
        VALUE vhelper = values[a+1];
        keys[a] = k;
        values[a+1] = v;
        k = khelper;
        v = vhelper;
    }
}

template < typename KEY, typename VALUE >
void snd_cpy(int &a, int b, int &innerind, InnerNode<KEY, VALUE> innerhelper){ //második fél másolása a beszúrásig/-tól
    while(a<b){
        innerhelper.keys[innerind] = keys[a];
        innerhelper.values[innerind+1] = values[a+1];
        keys[a] = __INT_MAX__;
        values[a+1] = nullptr;
        ++a;
        ++innerind;
    }
}

#endif