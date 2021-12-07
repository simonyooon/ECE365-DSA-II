// spellcheck.cpp 
// Simon Yoon 
// DSA II Sable Fall 2021

#include <iostream> 
#include <fstream> 
#include <string> 
#include <time.h>  // runtime fx
#include <algorithm>  // lowercase conversion
#include <stdio.h> 
#include <cctype> 
#include <ctime> 
#include <cstring> 
#include <cstdlib> 
#include "hash.h"

using namespace std; 

hashTable loadDictionary(const string &dictionary){ 

    int valid;
    string line;
    ifstream input;
    hashTable hTable(40000); // arbitrary size for load speed 
    input.open(dictionary);

    if (input.is_open()){ // input file open attempt 
        while(getline(input,line)) {
            valid = 1; 
            for (int i=0; i<line.size(); i++){ // check validity 
                if (!(isalnum(line[i]) || 
                     line[i] == '-'    || 
                     line[i] == '\'')  || 
                     line.size() > 20) {
                        valid = 0;
                        line.clear();
                }
            }
            if (valid == 1) { // valid to insert dictionary into table
                line = line.substr(0, line.length());
                transform(line.begin(), line.end(), line.begin(),::tolower); // lowercase conversion
                hTable.insert(line);
            }
        }
    } else {
      cout << "File failed to open." << endl;
        exit(EXIT_FAILURE);
    }
    input.close();
    return hTable; 
  } 

void spellCheck(hashTable &hash, const string &inputfile, const string &outputfile) {
    ifstream input;
    input.open(inputfile);

  // input file open attempt
    if(!input.is_open()){
        cerr << "Error: Failed to open file for reading: " << inputfile << " " << strerror(errno) << endl;
        exit(EXIT_FAILURE);
    }
  // output file open attempt 
    ofstream output;
    output.open(outputfile);
    
    if(!output.is_open()){
        cerr << "Error: Failed to open file for reading: " << outputfile << " " << strerror(errno) << endl;
        exit(EXIT_FAILURE);
    }
    string word, line;
    word.clear();  // preventative measures
    line.clear();
    int hasNum = 0;
    int lineNum = 0;
    while(getline(input, line)) {
        lineNum++;
        word.clear();
        transform(line.begin(), line.end(), line.begin(), ::tolower); // lowercase conversion 
        for(int i = 0; i <= line.size(); i++) {
            if(isalnum(line[i]) ||  // valid word conditions
                 line[i] == '-' || 
                 line[i] == '\'') {
                word += line[i];
                if(isdigit(line[i])) {
                    hasNum = 1;
                }
            }
            else {
                if(!word.empty() && !hasNum) { // invalid word handling
                    if(word.size() > 20) { 
                        output << "Long word at line " << lineNum << ", starts: " << word.substr(0,20) << endl;
                        word.clear();
                        hasNum = 0;
                    }
                    else if(!hash.contains(word)) {
                        output << "Unknown word at line " << lineNum << ": " << word << endl;
                        word.clear();
                        hasNum = 0;
                    }
                    else {
                        word.clear();
                        hasNum = 0;
                    }
                }
                else {
                    word.clear();
                    hasNum = 0;
                }
            }

        }
        line.clear();
    }
    input.close();
    output.close();
}


int main() {  
  
    string dictionary, inputfile, outputfile;
    clock_t loadstart = clock();

    cout<<"Enter name of dictionary : ";
    cin>>dictionary; 

    hashTable h = loadDictionary(dictionary); 
      
    clock_t loadstop = clock();
    double loadTime = ((double) loadstop - loadstart) / CLOCKS_PER_SEC;
    printf("Total time(in seconds) to load dictionary: %f \n",loadTime); 

    cout<<"Enter name of input file : ";
    cin>>inputfile;
    cout<<"Enter name of output file: ";
    cin>>outputfile; 

    clock_t checkstart = clock();

    spellCheck(h,inputfile,outputfile);

    clock_t checkstop = clock();
    double checkTime = ((double) checkstop - checkstart) / CLOCKS_PER_SEC; 
    printf("Total time(in seconds) to spell-check document: %f \n",checkTime);
    
    return 0;
}
