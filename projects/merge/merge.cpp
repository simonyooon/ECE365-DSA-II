#include <iostream>
#include <cerrno>
#include <cstdlib>
#include <string>
#include <stdio.h>
#include <cstring>
#include <errno.h>
#include <fstream>

using namespace std;

/*
 ./merge.exe
 Enter name of input file : input.txt
 Enter name of output file : output.txt
*/

bool mtx[1001][1001]; // boolean matrix to check if merge is valid

int main(){
    string inputName, outputName;
	ifstream input;
	ofstream output;

	cout << "Enter name of input file: ";
	cin >> inputName;
	cout << "Enter name of output file: ";
	cin >> outputName;
	
    input.open(inputName.c_str());
	output.open(outputName.c_str());
    
    string one, two, merge;
    // check if files are open
	if (input.is_open() && output.is_open()) {
		while (input >> one && input >> two && input >> merge) {
            //not a merge if (lengths are !=) or (last letter are !=) 
			if ((one.length() + two.length()) != merge.length() ||
                (merge[merge.length()-1] !=one[one.length()-1] && 
                 merge[merge.length()-1]!=two[two.length()-1])) 
            {
				output << "*** NOT A MERGE ***" << endl;
				continue;
			}
            // dynamic programming
			for (int i = 0; i < two.length()+1; i++) { // row and column count
				for (int j = 0; j < one.length()+1; j++) {
                    // first is valid, handle ties
					if (i == 0 && j > 0) { 
						if (merge[j-1] == one[j-1]) { 
                            if(j == 1){
                                mtx[i][j] = true;
                                continue;
                            }
                            else{
                                mtx[i][j] = mtx[i][j-1];
                                continue;
                            }
						}
					} 
					else if (j == 0 && i > 0) { 
						if (merge[i-1] == two[i-1]) {
                            if(i == 1){
                                mtx[i][j] = true;
                                continue;
                            }
                            else{
                                mtx[i][j] = mtx[i-1][j];
                                continue;
                            }
						}
					}
					    mtx[i][j] = false;
				}
			}

			for (int i = 1; i < two.length()+1; i++) {
				for (int j = 1; j < one.length()+1; j++) {
					mtx[i][j] = false;
					if (mtx[i-1][j] || mtx[i][j-1]) {
						if (two[i-1] == one[j-1] && merge[i+j-1] == one[j-1]) {
							mtx[i][j] = true;
						}
					    else if (mtx[i-1][j]) {
							if (merge[i+j-1] == two[i-1]) {
								mtx[i][j] = true;
							}
						}
						else if (mtx[i][j-1]) {
							if (merge[i+j-1] == one[j-1]) {
								mtx[i][j] = true;
							}
						}
					}
				}
			}

			if (mtx[two.length()][one.length()]==1) {
				int m = two.length();
				int n = one.length();

				while (n > 0 && m >= 0) { 
					if (mtx[m][n] && (m == 0 || !mtx[m-1][n])) {
						merge[m+n-1] = toupper(merge[m+n-1]); //
						n--;
					}
					else
						m--;
				}
				output << merge << endl;
			}
			else{
				output << "*** NOT A MERGE ***" << endl;
            }
		}
        // error handling for file open fail
	} else if(!input.is_open() || !output.is_open()){
        if(!input.is_open() && !output.is_open()){
            fprintf(stderr, "Failed to open either files: %s & %s, ERROR: %s\n", inputName.c_str(), outputName.c_str(), strerror(errno));
        }
        else if(!input.is_open()){
            fprintf(stderr, "Failed to open input file: %s, ERROR: %s\n", inputName.c_str(), strerror(errno));
        }
        else if(!output.is_open()){
            fprintf(stderr, "Failed to open output file: %s, ERROR: %s\n", outputName.c_str(), strerror(errno));
        }
        input.close(); 
        output.close();
        return -1;
    }
	input.close();
	output.close();
	return 0;
}