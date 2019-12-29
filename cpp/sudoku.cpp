#include <iostream>
#include <fstream>
#include <vector>
#include "sudoku.h"
using namespace std;

Sudoku::Sudoku(char* _filename){
    this->_readFile(_filename);
}

Sudoku::Sudoku(vector<vector<int>> _field){
    this->_initField(_field);
}

void Sudoku::_readFile(char* _filename){
    ifstream ifs(_filename);
    string buf;
    this->field.clear();
    this->original.clear();
    for (int i = 0; i < 9; ++i){
        this->field.push_back(vector<int>(9, 0));
        getline(ifs, buf);
        for (int j = 0; j < 9; ++j){
            this->field[i][j] = buf[j] - '0';
            this->original[i][j] = (this->field[i][j] > 0);
        }
    }
    this->_updateCandidates();
}

void Sudoku::_initField(vector<vector<int>> _field){
    this->field = _field;
    for (int i = 0; i < 9; ++i){
        for (int j = 0; j < 9; ++j){
            this->original[i][j] = (this->field[i][j] > 0);
        }
    }
}

void Sudoku::_updateCandidates(){
    if (this->num_cand.empty()){
        // init
        this->cand_per_num.clear();
        this->cand_in_field.clear();
        this->num_cand.clear();
        for (int i = 0; i < 9; ++i){
            this->cand_per_num.push_back(set<pair<int,int>>());
            this->cand_in_field.push_back(vector<set<int>>());
            for (int j = 0; j < 9; ++j){
                this->cand_in_field[i].push_back(set<int>());
                for (int k = 0; k < 9; ++k){
                    this->cand_in_field[i][j].insert(k+1);
                }
            }
            this->num_cand.push_back(vector<int>(9, 0));
        }
        // fill cand_in_field
        for (int i = 0; i < 9; ++i){
            for (int j = 0; j < 9; ++j){
                if (this->field[i][j] > 0){
                    int val = this->field[i][j];
                    for (int k = 0; k < 9; ++k){
                        if (j != k){
                            this->cand_in_field[i][k].erase(val);
                        }
                        if (i != k){
                            this->cand_in_field[k][j].erase(val);
                        }
                    }
                    this->num_cand[i][j] = this->cand_in_field[i][j].size();
                }
            }
        }
        // fill cand_per_num
        for (int i = 0; i < 9; ++i){
            for (int j = 0; j < 9; ++j){
                if (this->field[i][j] == 0){
                    for (int v: this->cand_in_field[i][j]){
                        this->cand_per_num[v-1].insert(tuple<int,int>(i, j));
                    }
                }
            }
        }
    }
}

void Sudoku::fill(pair<int,int> pos, int val){
    int i = pos.first;
    int j = pos.second;
    this->field[i][j] = val;
    for (pair<int,int>& p: this->cand_per_num[val-1]){
        
    }
}