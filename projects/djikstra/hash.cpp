#include <iostream> 
#include <iostream>
#include <stdio.h>
#include <cstdlib>
#include <string>
#include <cstring> // additional libraries
#include <ctime>    // may be redundant / fixed rogue errors
#include <fstream>
#include <algorithm>
#include <vector>
#include <cctype>
#include "hash.h"

using namespace std;

// The constructor initializes the hash table.
// Uses getPrime to choose a prime number at least as large as
// the specified size for the initial size of the hash table.
hashTable::hashTable(int size){  //member fx
    capacity = getPrime(size);
    data.resize(capacity);
    filled = 0;
}

// Insert the specified key into the hash table.
// If an optional pointer is provided,
// associate that pointer with the key.
// Returns 0 on success,
// 1 if key already exists in hash table,
// 2 if rehash fails.
int hashTable::insert(const std::string &key, void *pv){
    int i = hash(key);
    while (data[i].isOccupied){
        if (data[i].key == key && !data[i].isDeleted){ // !! 
            return 1; // already exists 
        } i++;
        if (i==capacity) {
            i = 0;  // success
        }
    }
    // insertion
    data[i].key = key; 
    data[i].isOccupied = true;
    data[i].isDeleted = false;
    data[i].pv = pv;

    if(filled++ > (capacity / 2)) { // rehash condition
        if(!rehash()) { //
                return 2; // fail
        }
    }
        return 0;
}

// Check if the specified key is in the hash table.
// If so, return true; otherwise, return false.
bool hashTable::contains(const string &key){
    return (findPos(key) != -1);
}


// Get the pointer associated with the specified key.
// If the key does not exist in the hash table, return nullptr.
// If an optional pointer to a bool is provided,
// set the bool to true if the key is in the hash table,
// and set the bool to false otherwise.
void * hashTable::getPointer(const std::string &key, bool *b){
     int pos;
       if ((pos = findPos(key)) >= 0)
       {
           if (b != NULL)
           {
               *b = true;
           }
           return data[pos].pv;
       }
       if (b != NULL)
       {
           *b = false;
       }
       return NULL;
}

// Set the pointer associated with the specified key.
// Returns 0 on success,
// 1 if the key does not exist in the hash table.
int hashTable::setPointer(const std::string &key, void *pv){
    int pos;
    if ((pos = findPos(key)) >= 0)
    {
        data[pos].pv = pv;
        return 0;
    }
    return 1;
}

// Delete the item with the specified key.
// Returns true on success,
// false if the specified key is not in the hash table.
bool hashTable::remove(const std::string &key) {
    unsigned hashIndex = findPos(key);
    if (hashIndex == -1) { 
        return false;
    }
    data[hashIndex].isDeleted = true; // "lazily deleted"
    return true; 
}

int hashTable::hash(const std::string &key) {
    const char *strPtr = key.c_str(); // randomly chosen hash fx
    unsigned int hashIndex = 31; 
    while(*strPtr){ // ptr to first character then to 0 on end of string
        hashIndex = (hashIndex * 11939) ^ (strPtr[0] * 39119);
        strPtr++;
    }
    return hashIndex % capacity;
}


// Search for an item with the specified key.
// Return the position if found, -1 otherwise.
int hashTable::findPos(const std::string &key){
    int hashIndex = hash(key);
    while(data[hashIndex].isOccupied){
        if(data[hashIndex].key == key){
            if(!data[hashIndex].isDeleted){
                return hashIndex;
            }
            else{
                // return -1;
            }
        }
        hashIndex++;
        hashIndex %= capacity;
    }
    return -1;
}

// The rehash function; makes the hash table bigger.
// Returns true on success, false if memory allocation fails.
bool hashTable::rehash(){
    int initialSize = data.size();
        vector<hashItem> temp = data; 
        data.clear();
        capacity = getPrime(capacity);
        if(capacity == data.size()) { // max cap condition 
            cout << "The hash table has reached its maximum capacity." << endl;
            return false;
    }
        data.resize(capacity);
        if(data.size() != capacity){ // extraneous fail 
            cout << "ERROR: Unable to rehash the hash table." << endl;
            return false;
    }
        int finalSize = data.size();
        filled = 0;
     
        for(int i = 0; i < temp.size(); i++) { //rehash insert
        if((temp[i].isOccupied == true) && (temp[i].isDeleted == false))
            insert(temp[i].key, temp[i].pv);
    }
    return true;
}


//getPrime function
unsigned int hashTable::getPrime(int size){

    static int primes[] = {12289, 24593, 49157, 98317, 
    196613, 393241, 786433, 1572869, 3145739, 6291469}; // good hash primes
    
    for(unsigned int i = 0; i < 10 ; i++){
        if(primes[i] > (size * 2)){
            return primes[i];
        }
    }
    return primes[(sizeof(primes) / sizeof(primes[0])) - 1];
}


