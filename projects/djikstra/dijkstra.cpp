#include "heap.h"
#include "hash.h"
#include "graph.h"

#include <chrono>

using namespace std;
int main(int argc, const char* argv[]) {
    string inFile, outFile;
    ifstream input;
    ofstream output;
    graph dijkstra;
    string startvec;
    
    while (!input.is_open()){
        cout << "Enter name of input file: " << endl; // input 
        cin >> inFile;
        input.open(inFile, ifstream::in);
        if(!input.is_open()){
            fprintf(stderr, "ERROR opening input file: %s\n", strerror(errno));
            cout << "Invalid input file, please try again." << endl; 
        }
    }
    
    dijkstra.makeGraph(input); // construct graph
    input.close();

    while(!dijkstra.isVertex(startvec)){
        cout << "Please enter a start vertex: " << endl; // starting vector input
        cin >> startvec;
        if(!dijkstra.isVertex(startvec))
            cout << "Invalid vertex, please try again." << endl;

    }
    clock_t start, end; // record time heuristics 
    start = clock();
    dijkstra.dijkstra(startvec);    
    end = clock();
    double time = (double)(end-start)/(double)CLOCKS_PER_SEC;
    cout << "Total Time (in seconds) to apply Dijkstra's algorithm: " << time << endl;

    while(!output.is_open()){
        cout << "Enter name of output file: " << endl; // output
        cin >> outFile;
        output.open(outFile, ofstream::out | ofstream::trunc);
        if(!output.is_open()){
            fprintf(stderr, "ERROR creating input file: %s\n", strerror(errno));
            cout << "Invalid output file, please try again." << endl;
        }
    }
    
    dijkstra.printGraph(output);
    output.close();
    return 0;
}