#ifndef GRAPH_H
#define GRAPH_H

#include <list>
#include <sstream>
#include <iterator>

#include "heap.h"
#include "hash.h"

using namespace std;

class graph{
    public:
        graph();
        bool isVertex(const string &);
        void dijkstra(const string &);
        void makeGraph(ifstream &);
        void insert(const string &, const string &, int);
        void printGraph(ofstream &);

    private:
        class edge;
        class vertex;
        int capacity;
        
        class edge{
            public:
                int cost;
                vertex *destination;
        };
        class vertex{
            public:
                string id;
                bool known;
                int distance;

                list<edge> adjacent;
                list<string> path;
        };
        list<vertex*> vertexList;
        hashTable *vertices;
};
#endif /* GRAPH_H */