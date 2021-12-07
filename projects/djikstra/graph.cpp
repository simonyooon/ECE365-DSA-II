#define BIG_NUM 1000000

#include "graph.h"

#include <climits>
#include <iostream>

using namespace std;

graph::graph(){
    vertices = new hashTable(BIG_NUM); // random big num
    capacity = 0;
}

void graph::makeGraph(ifstream &input){
    int distance;
    string v1, v2;
    while (input >> v1 >> v2 >> distance){ 
        insert(v1, v2, distance);
    }
}

void graph::printGraph(ofstream &out){
    for(list<vertex *>::const_iterator it = vertexList.begin(); it!= vertexList.end(); ++it){  
        out << (*it)->id << ": ";
        if ((*it)->distance == INT_MAX){
            out << "NO PATH" << endl; // path invalid 
        }
        else {
            out << (*it) ->distance << " [";
            for (list<string>::const_iterator i = (*it) ->path.begin(); i!= (*it) ->path.end(); ++i){
                out << *i;
                if (next(i,1)!= (*it) ->path.end()){ // check if valid path and list in out
                    out << ", ";
                }
            }
            out << "]" << endl; // colon , comma, brackets for output formatting
        }
    }
}

void graph::dijkstra(const string &startvec){
    vertex *start = (vertex *)vertices -> getPointer(startvec);
    start -> distance = 0;
    start -> path.push_back(startvec);
    heap dHeap(capacity);

    dHeap.insert(startvec, start->distance, start); // dijkstra via heap
    for (list<vertex*>::const_iterator it = vertexList.begin(), end = vertexList.end();
         it != end; ++it){
        if ((*it)-> id != startvec){
            dHeap.insert((*it)->id, (*it)->distance, *it);
        }
    }
    vertex *tempvec = nullptr;
    for (int i = 0; i < capacity; ++i){
        dHeap.deleteMin(nullptr, nullptr, &tempvec); // use deleteMin to retrieve smallest element
        tempvec-> known = true;
        for (list<edge>::const_iterator it = tempvec->adjacent.begin(), end = tempvec->adjacent.end(); it != end && tempvec->distance != INT_MAX; ++it){
            if ( (!it -> destination->known)    &&  (it->destination->distance > (it ->cost + tempvec -> distance))   &&  (tempvec -> distance != INT_MAX) ){
                
                it->destination->path.assign(tempvec->path.begin(), tempvec->path.end());
                it->destination->path.push_back(it->destination->id);

                it->destination->distance = it->cost + tempvec ->distance;
                dHeap.setKey(it->destination->id, it->destination->distance);
            } // path walk 
        }
            }
}

void graph::insert(const string &v1, const string &v2, int distance){
    edge tempedge;
    vertex *tempv1;
    vertex *tempv2;
    
    if (!vertices -> contains(v1)){
        tempv1 = new vertex;
        tempv1 -> id = v1;
        tempv1 -> known = false;
        tempv1 -> distance = INT_MAX;
        vertexList.push_back(tempv1);
        vertices -> insert(v1, tempv1);
        capacity++;
    }
    else {
        tempv1 = (vertex*)vertices -> getPointer(v1);
    }
    if (!vertices -> contains(v2)){
            tempv2 = new vertex;
            tempv2 -> id = v2;
            tempv2 -> known = false;
            tempv2 -> distance = INT_MAX;
                vertexList.push_back(tempv2);
                vertices -> insert(v2, tempv2);
                capacity++;
    }
    else {
        tempv2 = (vertex *)vertices -> getPointer(v2);
    }
    tempedge.destination = tempv2;
    tempedge.cost = distance; // record cost

    tempv1 -> adjacent.push_back(tempedge); 
}

bool graph::isVertex(const string &v){
    return (vertices -> contains(v)); 
}