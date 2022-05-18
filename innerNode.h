#ifndef INNERNODE_H
#define INNERNODE_H

#include "node.h"

template < typename KEY, typename VALUE >
class Tree;

template < typename KEY, typename VALUE >
class InnerNode : public Node <KEY, VALUE>
{
    private:
        int key_count_;
        int children_count_;
        KEY *keys_;
        Node<KEY, VALUE> **children_;
        bool root_ = false;
        bool leaf_ = false; //lowest_layer
        // add helper functions
        void AddWithoutSplit(int& i, AdditionalNode<KEY, VALUE>& a_node);
        void FirstCopy(int& a, int b, AdditionalNode<KEY, VALUE> a_node);
        void SecondCopy(int& a, int b, int& innerind, InnerNode<KEY, VALUE>* innerhelper);
        void SplitToLastHalf(int& i, int& innerind, InnerNode<KEY, VALUE>* innerhelper, AdditionalNode<KEY, VALUE>& a_node);
        void CopyLastHalfWithAddingElement(int& i, int& j, int& innerind, InnerNode<KEY, VALUE>* innerhelper, AdditionalNode<KEY, VALUE>& a_node);
        void SplitToFirstHalf(int& i, int& innerind, InnerNode<KEY, VALUE>* innerhelper, AdditionalNode<KEY, VALUE>& a_node);
        void CreateNewRoot(AdditionalNode<KEY, VALUE>& a_node);
        // add helper functions
        // remove helper functions
        void AfterRemoveAboveLeafs(int& ind);
        void MergeWithYoungerSibling(int& ind);
        void AskYoungerSiblingForChildren(int& ind);
        void MergeWithOlderSibling(int& ind);
        void AskOlderSiblingForChildren(int& ind);
        // remove helper functions
    public:
        InnerNode(int _key_count, int _children_count);
        AdditionalNode<KEY, VALUE> add(KEY key, VALUE value) override;
        VALUE search(KEY key) const override;
        bool remove(KEY key) override;
        void ConvertToNewTree(Tree<KEY, VALUE>* tree) override; //TODO: rekurziv destruktor hivasok
        void print(int space_count, std::ofstream& myfile) override;
        bool IsKeyInTree(KEY key) const override;
        bool GetKeyIsMaxAtIndex(int keyIndex) override;
        KEY GetKeyAtIndex(int keyIndex) override;
        Node<KEY, VALUE>* GetValueAtIndex(int valueIndex) override;
        void SetKeyAtIndex(KEY key, int keyIndex) override;
        void SetValueAtIndex(Node<KEY, VALUE>* value, int valueIndex) override;
        // TODO: Is it necessary? Only used when adding a guard leaf node. Maybe create a method?
        friend class Tree<KEY, VALUE>; 
};

template < typename KEY, typename VALUE >
InnerNode<KEY, VALUE>::InnerNode(int _key_count, int _children_count) : Node<KEY, VALUE>() {
    key_count_ = _key_count;
    children_count_ = _children_count;
    keys_ = new KEY[key_count_];
    children_ = new Node<KEY, VALUE>*[children_count_];
    for(int i=0; i<key_count_; ++i){
        keys_[i] = std::numeric_limits<KEY>::max();
        children_[i] = nullptr;
    }
    children_[key_count_] = nullptr;
}


template < typename KEY, typename VALUE >
AdditionalNode<KEY, VALUE> InnerNode<KEY, VALUE>::add(KEY key, VALUE value){
    int i = 0;
    while(i<this->key_count_ && this->keys_[i]<=key) //kulcs megkeresése
        ++i;
    AdditionalNode<KEY, VALUE> a_node = this->children_[i]->add(key, value); //rekurzív hívások levélszintig
    if(a_node.nodehelper_ == nullptr) return a_node; //nem történt hasítás lejjebbi szinten  /*jobb oldal bal gyerek kell még*/
    if(this->keys_[this->key_count_ - 1] == std::numeric_limits<KEY>::max()){ //nem telített a csúcs, nem fogunk hasítani, csak beillesztjük a kapott KEY, VALUE párt
        AddWithoutSplit(i, a_node);
        return a_node;
    }
    int innerind = 0; //hasítani fogunk
    InnerNode<KEY, VALUE>* innerhelper = new InnerNode(this->key_count_, this->children_count_);
    if(this->leaf_) innerhelper->leaf_ = true;
    if(i>=(this->children_count_)/2){ //beillesztés a csúcs második felébe kerül, így másolás után tudjuk ezt megtenni
        SplitToLastHalf(i, innerind, innerhelper, a_node);
    }else{ //első félbe kell beilleszteni
        SplitToFirstHalf(i, innerind, innerhelper, a_node);
    }
    a_node.nodehelper_ = innerhelper;
    if(this->root_){ //ha a gyökércsúcsot hasítottuk, akkor új gyökércsúcsot kell létrehoznunk
        CreateNewRoot(a_node);
    }
    return a_node;
}

template < typename KEY, typename VALUE >
void InnerNode<KEY, VALUE>::AddWithoutSplit(int& i, AdditionalNode<KEY, VALUE>& a_node){
    this->FirstCopy(i, this->key_count_, a_node);
    a_node.nodehelper_ = nullptr;
    a_node.keyhelper_ = 0;
}

template < typename KEY, typename VALUE >
void InnerNode<KEY, VALUE>::FirstCopy(int& a, int b, AdditionalNode<KEY, VALUE> a_node){ //első fél eltolása végéig/feléig beszúrás utántól
    while(a<--b){
        this->keys_[b] = this->keys_[b-1];
        this->children_[b+1] = this->children_[b];
    }
    this->keys_[a] = a_node.keyhelper_;
    this->children_[a+1] = a_node.nodehelper_;
}

template < typename KEY, typename VALUE >
void InnerNode<KEY, VALUE>::SecondCopy(int& a, int b, int& innerind, InnerNode<KEY, VALUE>* innerhelper){ //második fél másolása a beszúrásig/-tól
    while(a<b){
        innerhelper->keys_[innerind] = this->keys_[a];
        innerhelper->children_[innerind+1] = this->children_[a+1];
        this->keys_[a] = std::numeric_limits<KEY>::max();
        this->children_[a+1] = nullptr;
        ++a;
        ++innerind;
    }
}

template < typename KEY, typename VALUE >
void InnerNode<KEY, VALUE>::SplitToLastHalf(int& i, int& innerind, InnerNode<KEY, VALUE>* innerhelper, AdditionalNode<KEY, VALUE>& a_node){
    int j=(this->children_count_)/2;
    if(i != j){ //nem a második fél első eleme a beszúrandó elem
        CopyLastHalfWithAddingElement(i, j, innerind, innerhelper, a_node);
    } else { //második fél első eleme
        innerhelper->children_[innerind] = a_node.nodehelper_;
    }
    SecondCopy(j, this->key_count_, innerind, innerhelper);
}

template < typename KEY, typename VALUE >
void InnerNode<KEY, VALUE>::CopyLastHalfWithAddingElement(int& i, int& j, int& innerind, InnerNode<KEY, VALUE>* innerhelper, AdditionalNode<KEY, VALUE>& a_node){
    KEY keyhelper = this->keys_[j];
    this->keys_[j] = std::numeric_limits<KEY>::max();
    ++j;
    innerhelper->children_[innerind] = this->children_[j];
    this->children_[j] = nullptr;
    SecondCopy(j, i, innerind, innerhelper);
    innerhelper->keys_[innerind] = a_node.keyhelper_;
    innerhelper->children_[innerind+1] = a_node.nodehelper_;
    ++innerind;
    a_node.keyhelper_ = keyhelper;
}

template < typename KEY, typename VALUE >
void InnerNode<KEY, VALUE>::SplitToFirstHalf(int& i, int& innerind, InnerNode<KEY, VALUE>* innerhelper, AdditionalNode<KEY, VALUE>& a_node){
    int b = (this->children_count_)/2;
    KEY k = this->keys_[i];
    Node<KEY, VALUE>* v = this->children_[i+1];
    this->keys_[i] = a_node.keyhelper_;
    this->children_[i+1] = a_node.nodehelper_;
    KEY khelper = k;
    Node<KEY, VALUE>* vhelper = v;
    while(++i<b && k != std::numeric_limits<KEY>::max()){
        khelper = this->keys_[i];
        vhelper = this->children_[i+1];
        this->keys_[i] = k;
        this->children_[i+1] = v;
        k = khelper;
        v = vhelper;
    }
    innerhelper->keys_[innerind] = this->keys_[i];
    innerhelper->children_[innerind] = vhelper;
    ++innerind;
    a_node.keyhelper_ = khelper;
    this->keys_[i] = std::numeric_limits<KEY>::max();
    ++i;
    innerhelper->children_[innerind] = this->children_[i];
    this->children_[i] = nullptr;
    SecondCopy(i, this->key_count_, innerind, innerhelper);
}

template < typename KEY, typename VALUE >
void InnerNode<KEY, VALUE>::CreateNewRoot(AdditionalNode<KEY, VALUE>& a_node){
    InnerNode<KEY, VALUE>* nroot = new InnerNode(this->key_count_, this->children_count_);
    nroot->root_ = true;
    nroot->keys_[0] = a_node.keyhelper_;
    nroot->children_[0] = this;
    nroot->children_[1] = a_node.nodehelper_;
    a_node.nodehelper_ = nroot;
    this->root_ = false;
}

template < typename KEY, typename VALUE >
VALUE InnerNode<KEY, VALUE>::search(KEY key) const { //rekurzív lineáris keresés
    for(int i=0; i<this->key_count_; ++i){
        if(this->keys_[i]>key)
            return this->children_[i]->search(key);
    }
    return this->children_[this->key_count_]->search(key);
}

template < typename KEY, typename VALUE >
bool InnerNode<KEY, VALUE>::IsKeyInTree(KEY key) const { //rekurzív lineáris keresés
    for(int i=0; i<this->key_count_; ++i){
        if(this->keys_[i]>key)
            return this->children_[i]->IsKeyInTree(key);
    }
    return this->children_[this->key_count_]->IsKeyInTree(key);
}

template < typename KEY, typename VALUE >
bool InnerNode<KEY, VALUE>::remove(KEY key) {
    int ind = 0;
    while(ind<this->key_count_ && this->keys_[ind]<=key){
        ++ind;
    }
    bool split = this->children_[ind]->remove(key);
    if(leaf_){//tesztelve
        AfterRemoveAboveLeafs(ind);
    }
    else if(!split) return false;//tesztelve
    else if(ind != 0){
        if(this->children_[ind-1]->GetKeyIsMaxAtIndex(key_count_/2)){//tesztelve
            MergeWithYoungerSibling(ind);
        }else{//tesztelve
            AskYoungerSiblingForChildren(ind);
        }
    }
    else if(ind == 0){//tesztelve
        if(this->children_[1]->GetKeyIsMaxAtIndex(key_count_/2)){
            MergeWithOlderSibling(ind);
        }else{
            AskOlderSiblingForChildren(ind);
        }
    }
    if(this->keys_[key_count_/2-1] == std::numeric_limits<KEY>::max())
        return true;
    return false;
}

template< typename KEY, typename VALUE >
void InnerNode<KEY, VALUE>::AfterRemoveAboveLeafs(int& ind){
    delete this->children_[ind];
    if(ind == 0){
        this->children_[ind] = this->children_[ind+1];
        ++ind;
    }
    for(ind; ind<this->key_count_; ++ind){
        this->keys_[ind-1] = this->keys_[ind];
        this->children_[ind] = this->children_[ind+1];
    }
    this->keys_[key_count_-1] = std::numeric_limits<KEY>::max();
    this->children_[children_count_-1] = nullptr;
}

template< typename KEY, typename VALUE >
void InnerNode<KEY, VALUE>::MergeWithYoungerSibling(int& ind){
    int from_ind = key_count_-1;
    while(this->children_[ind-1]->GetKeyIsMaxAtIndex(from_ind)){
        --from_ind;
    }
    ++from_ind;
    this->children_[ind-1]->SetKeyAtIndex(this->keys_[ind-1],from_ind);
    this->children_[ind-1]->SetValueAtIndex(this->children_[ind]->GetValueAtIndex(0),from_ind+1);
    ++from_ind;
    for(int i=0; this->children_[ind]->GetKeyAtIndex(i) != std::numeric_limits<KEY>::max(); ++i, ++from_ind){
            this->children_[ind-1]->SetKeyAtIndex(this->children_[ind]->GetKeyAtIndex(i),from_ind);
            this->children_[ind-1]->SetValueAtIndex(this->children_[ind]->GetValueAtIndex(i+1),from_ind+1);
    }
    delete this->children_[ind];
    for(ind; ind<key_count_; ++ind){
        this->keys_[ind-1] = this->keys_[ind];
        this->children_[ind] = this->children_[ind+1];
    }
    this->keys_[key_count_-1] = std::numeric_limits<KEY>::max();
    this->children_[children_count_-1] = nullptr;
}

template< typename KEY, typename VALUE >
void InnerNode<KEY, VALUE>::AskYoungerSiblingForChildren(int& ind){
    int from_ind = key_count_-1;
    while(this->children_[ind-1]->GetKeyIsMaxAtIndex(from_ind)){
        --from_ind;
    }
    for(int i=key_count_-1; i>0; --i){
        this->children_[ind]->SetKeyAtIndex(this->children_[ind]->GetKeyAtIndex(i-1),i);
        this->children_[ind]->SetValueAtIndex(this->children_[ind]->GetValueAtIndex(i),i+1);
    }
    this->children_[ind]->SetValueAtIndex(this->children_[ind]->GetValueAtIndex(0),1);
    this->children_[ind]->SetValueAtIndex(this->children_[ind-1]->GetValueAtIndex(from_ind+1),0);
    this->children_[ind]->SetKeyAtIndex(this->keys_[ind-1],0);
    this->keys_[ind-1] = this->children_[ind-1]->GetKeyAtIndex(from_ind);
    this->children_[ind-1]->SetValueAtIndex(nullptr,from_ind+1);
    this->children_[ind-1]->SetKeyAtIndex(std::numeric_limits<KEY>::max(),from_ind);
}

template< typename KEY, typename VALUE >
void InnerNode<KEY, VALUE>::MergeWithOlderSibling(int& ind){
    int from_ind = 0;
    while(!this->children_[0]->GetKeyIsMaxAtIndex(from_ind)){
        ++from_ind;
    }
    this->children_[0]->SetKeyAtIndex(this->keys_[0],from_ind);
    this->children_[0]->SetValueAtIndex(this->children_[1]->GetValueAtIndex(0),from_ind+1);
    ++from_ind;
    for(int i=0; this->children_[ind]->GetKeyAtIndex(i) != std::numeric_limits<KEY>::max(); ++i, ++from_ind){
            this->children_[0]->SetKeyAtIndex(this->children_[1]->GetKeyAtIndex(i),from_ind);
            this->children_[0]->SetValueAtIndex(this->children_[1]->GetValueAtIndex(i+1),from_ind+1);
    }
    delete this->children_[1];
    ++ind;
    for(ind; ind<key_count_; ++ind){
        this->keys_[ind-1] = this->keys_[ind];
        this->children_[ind] = this->children_[ind+1];
    }
    this->keys_[key_count_-1] = std::numeric_limits<KEY>::max();
    this->children_[children_count_-1] = nullptr;
}

template< typename KEY, typename VALUE >
void InnerNode<KEY, VALUE>::AskOlderSiblingForChildren(int& ind){
    int from_ind = 0;
    while(!this->children_[0]->GetKeyIsMaxAtIndex(from_ind)){
        ++from_ind;
    }
    this->children_[0]->SetValueAtIndex(this->children_[1]->GetValueAtIndex(0),from_ind+1);
    this->children_[1]->SetValueAtIndex(this->children_[1]->GetValueAtIndex(1),0);
    this->children_[0]->SetKeyAtIndex(this->keys_[0],from_ind);
    this->keys_[0] = this->children_[1]->GetKeyAtIndex(0);
    for(int i=0; i<key_count_-1; ++i){
        this->children_[1]->SetKeyAtIndex(this->children_[1]->GetKeyAtIndex(i+1),i);
        this->children_[1]->SetValueAtIndex(this->children_[1]->GetValueAtIndex(i+2),i+1);
    }
    this->children_[1]->SetValueAtIndex(nullptr,children_count_-1);
    this->children_[1]->SetKeyAtIndex(std::numeric_limits<KEY>::max(),key_count_-1);
}

template < typename KEY, typename VALUE >
KEY InnerNode<KEY, VALUE>::GetKeyAtIndex(int keyIndex){
    return this->keys_[keyIndex];
}

template < typename KEY, typename VALUE >
Node<KEY, VALUE>* InnerNode<KEY, VALUE>::GetValueAtIndex(int valueIndex){
    return this->children_[valueIndex];
}

template < typename KEY, typename VALUE >
void InnerNode<KEY, VALUE>::SetKeyAtIndex(KEY key, int keyIndex){
    this->keys_[keyIndex] = key;
}

template < typename KEY, typename VALUE >
void InnerNode<KEY, VALUE>::SetValueAtIndex(Node<KEY, VALUE>* value, int valueIndex){
    this->children_[valueIndex] = value;
}

template < typename KEY, typename VALUE >
bool InnerNode<KEY, VALUE>::GetKeyIsMaxAtIndex(int keyIndex){
    return this->keys_[keyIndex] == std::numeric_limits<KEY>::max();
}

template< typename KEY, typename VALUE >
void InnerNode<KEY, VALUE>::ConvertToNewTree(Tree<KEY, VALUE>* tree){
    for(int ind = key_count_-1; ind >= 0; --ind){
        if(this->keys_[ind] == std::numeric_limits<KEY>::max()) continue;
        this->children_[ind+1]->ConvertToNewTree(tree);
        if(leaf_) delete this->children_[ind+1];
    }
    this->children_[0]->ConvertToNewTree(tree);
    delete this->children_[0];
    delete this;
}

template < typename KEY, typename VALUE >
void InnerNode<KEY, VALUE>::print(int space_count, std::ofstream& myfile){
    for(int i=0; i<space_count; ++i){
        myfile<<" ";
    }
    if(!root_) myfile<<"- ";
    myfile<<"keys: ["<<keys_[0];
    for(int i=1; i<key_count_ && keys_[i] != std::numeric_limits<KEY>::max(); ++i){
        myfile<<", "<<keys_[i];
    }
    myfile<<"]\n";
    for(int i=0; i<space_count; ++i){
        myfile<<" ";
    }
    if(!root_) myfile<<"  ";
    myfile<<"children:\n";
    int new_space_count = space_count;
    new_space_count += root_ ? 2 : 4;
    children_[0]->print(new_space_count, myfile);
    for(int i=0; i<key_count_ && keys_[i] != std::numeric_limits<KEY>::max(); ++i){
        children_[i+1]->print(new_space_count, myfile);
    }
}

#endif