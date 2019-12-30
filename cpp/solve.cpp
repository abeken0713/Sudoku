#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include "sudoku.h"
using namespace std;


int main(int argc, char **argv){
    if (argc < 2){
        cout << "Filename required" << endl;
        return 1;
    }
    char* filename = argv[1];
    Sudoku game(filename);

    return 0;
}