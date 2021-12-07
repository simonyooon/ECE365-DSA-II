// Simon Yoon
// DSA II - useHeap

#include "heap.h"
#include "hash.h"
#include <string>
#include <iostream>

using namespace std;

// heap constructor , initialization
heap::heap(int capacity){
    data.resize(capacity+1);
    filled = 0;
    mapping = new hashTable(capacity*2);
    this->capacity = capacity;
    }

void heap::swap(int a, int b){ // swapping place of nodes
    node Temp=data[a];
    data[a]=this->data[b];
    data[b]=Temp;
    mapping->setPointer(data[a].id,&data[a]);
    mapping->setPointer(data[b].id,&data[b]);
}

int heap::insert(const std::string &id, int key, void *pv){
    if (filled >= capacity)
    {
        return 1; // heap is full
    }
    if (mapping->contains(id) == false)
    {
        filled++;
        data[filled].id = id; //node, id, key, ptr assignment
        data[filled].key = key;
        data[filled].pData = pv;

        mapping->insert(id, &data[filled]); // filling mapping hashTable
        percolateUp(filled);
    }
    else
    {
        return 2; // node already exists
    }
    return 0;
}

int heap::getPos (node * pn){ // node position
    return (pn - &data[0]);
}

int heap::deleteMin(std::string *pId , int *pKey , void *ppData ){
    if (filled != 0) {
        if ( pId != nullptr ) {
            *pId = data[1].id; //update values for deletion
        }
        if ( pKey != nullptr) {
            *pKey = data[1].key;
        }
        if ( ppData != nullptr) {
            *(static_cast<void**> (ppData)) = data[1].pData;
            }
        mapping->remove(data[1].id); // delete root from hashTable and replace with last item
        data[1] = data[filled];
        filled--;
        percolateDown(1); // displace for deletion

        return 0;
        } else {
        return 1; // heap is empty
    }
}
// set key values based on string id
int heap::setKey(const std::string &id, int key) {
    if(!mapping->contains(id)){
        return 1;
    }else{
        node* pn =static_cast<node *>(mapping->getPointer(id)); // ptr -> node of id
        int tempkey=pn->key;
        pn->key=key;
        int index= getPos(pn);
        if (key > tempkey){ // percolate down if new key is greater than old key
            percolateDown(index);
        }else if(key < tempkey){ // percolate down if new key is less than old key
            percolateUp (index);
        }
        return 0;
    } return 0;
}

// remove item from heap
int heap::remove(const std::string &id, int *pKey , void *ppData ){
    if(!mapping->contains(id)) {
        return 1; // heap does not have id
    }else {
        node* pn= static_cast<node *>(mapping->getPointer (id)); // ptr to node id
        if (pKey != nullptr) *pKey = pn->key; // pKey initialized to key of node to be deleted
        if (ppData != nullptr) *(static_cast <void**> (ppData)) = pn->pData; // aassociated node ptr
        int tempkey= pn->key;
        mapping->remove (id); //remove node from hashTable
        *pn = data[filled--];

        int newKey = pn->key; // reassign
        int pos=getPos(pn);
        if (tempkey < newKey) {
            percolateDown (pos);
        }else if (tempkey > newKey) {
            percolateUp (pos);
        }
        return 0;
    }
}

// move a node up the tree via insert
void heap::percolateUp (int posCur){
    node Temp=data[posCur];
    int Parent=(posCur/2); //parent

    while(posCur>1 && (Temp.key)<(data[Parent].key)){
        data[posCur] = data[Parent]; // store node
        mapping->setPointer(data[posCur].id, &data[posCur]);
        posCur=Parent; // move to parent pos
        Parent=(posCur/2);
    }
    data[posCur]=Temp; // change node pos
    mapping->setPointer(data[posCur].id,&data[posCur]); // reassign
}

// move a node down the tree via deletion
void heap::percolateDown (int posCur){
    mapping->setPointer(data[posCur].id,&data[posCur]);
    int j=0;
    int right=(2*posCur+1);  // right child
    if(right<=(filled) && data[posCur].key>data[right].key){
        int left=(2*posCur); // left child
        if(data[left].key<data[right].key){ // determine correct position
            j=left;
            }else{
            j=right;
            }
        }else{
            int left=(2*posCur);
            if( left<=(filled) && data[posCur].key>data[left].key){
                j=left;
            }
        }
        if(j>=1) swap(j,posCur); // swap correct with current
        posCur=j;

    while(posCur>=1) { // run at least once + while until true
        int j=0;
        int right=(2*posCur+1);
        if( right<=(filled) && data[posCur].key>data[right].key){
            int left=(2*posCur);
            if(data[left].key<data[right].key){
                j=left;
            }else{
                j=right;
            }
        }else{
            int left=(2*posCur);
            if( left<=(filled) && data[posCur].key>data[left].key){
                j=left;
            }
        }
        if(j>=1) swap(j,posCur);
        posCur=j;
    }

}


