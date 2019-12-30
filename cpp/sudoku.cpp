#include <iostream>
#include <fstream>
#include <vector>
#include <numeric>
#include "sudoku.h"
using namespace std;

Sudoku::Sudoku(char* _filename){
    cout << "Sudoku::Sudoku" << endl;
    this->_readFile(_filename);
}

Sudoku::Sudoku(vector< vector<int> > _field){
    this->_initField(_field);
}

void Sudoku::_readFile(char* _filename){
    ifstream ifs(_filename);
    string buf;
    this->field.clear();
    this->original.clear();
    for (int i = 0; i < 9; ++i){
        this->field.push_back(vector<int>(9, 0));
        this->original.push_back(vector<bool>(9, false));
        getline(ifs, buf);
        for (int j = 0; j < 9; ++j){
            this->field[i][j] = buf[j] - '0';
            this->original[i][j] = (this->field[i][j] > 0);
        }
    }
    this->_updateCandidates();
}

void Sudoku::_initField(vector< vector<int> > _field){
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
            this->cand_per_num.push_back(set< pair<int,int> >());
            this->cand_in_field.push_back(vector< vector<bool> >());
            for (int j = 0; j < 9; ++j){
                this->cand_in_field[i].push_back(vector<bool>(9, 1));
            }
            this->num_cand.push_back(vector<int>(9, 0));
        }
        // fill cand_in_field
        for (int i = 0; i < 9; ++i){
            for (int j = 0; j < 9; ++j){
                if (this->field[i][j] > 0){
                    int val = this->field[i][j];
                    // horizontal and vertical
                    for (int k = 0; k < 9; ++k){
                        this->cand_in_field[i][k][val-1] = false;
                        this->cand_in_field[k][j][val-1] = false;
                    }
                    // block
                    for (int k = 0; k < 3; ++k){
                        for (int l = 0; l < 3; ++l){
                            this->cand_in_field[i/3*3+k][j/3*3+l][val-1] = false;
                        }
                    }
                }
            }
        }
        // fill num_cand
        for (int i = 0; i < 9; ++i){
            for (int j = 0; j < 9; ++j){
                if (this->field[i][j] == 0){
                    this->num_cand[i][j] = accumulate(
                        this->cand_in_field[i][j].cbegin(),
                        this->cand_in_field[i][j].cend(), 0
                    );
                }
            }
        }
        // fill cand_per_num
        for (int i = 0; i < 9; ++i){
            for (int j = 0; j < 9; ++j){
                if (this->field[i][j] == 0){
                    for (int k = 0; k < 9; ++k){
                        if (this->cand_in_field[i][j][k]){
                            this->cand_per_num[k].insert(pair<int,int>(i, j));
                        }
                    }
                }
            }
        }
    }
}

void Sudoku::fill(pair<int,int> pos, int val){
    int i = pos.first;
    int j = pos.second;
    if (!isValid(pos, val)){
        cerr << "Invalid operation to (" << i << ", " << j << ") with " << val << endl;
        return;
    }
    this->field[i][j] = val;
    for (auto iter=this->cand_per_num[val-1].begin(); iter!=this->cand_per_num[val-1].end();){
        if (iter->first == i || iter->second == j){
            this->cand_per_num[val-1].erase(iter);
        }
        else{
            ++iter;
        }
    }
    // for (auto& [x, y]: this->cand_per_num[val-1]){
    //     if (x == i || x == j){
    //         this->cand_per_num[val-1].erase(pair<int,int>(x, y));
    //     }
    // }
    for (int x = 0; x < 9; ++x){
        if (this->field[x][j] == 0){
            this->num_cand[x][j] -= this->cand_in_field[x][j][val-1];
            this->cand_in_field[x][j][val-1] = false;
        }
        if (this->field[i][x] == 0){
            this->num_cand[i][x] -= this->cand_in_field[i][x][val-1];
            this->cand_in_field[i][x][val-1] = false;
        }
    }
}

void Sudoku::show(){
    cout << "---- Field ----" << endl << endl;
    for (int i = 0; i < 9; ++i){
        for (int j = 0; j < 9; ++j){
            if (j > 0 && j % 3 == 0){
                cout << "| ";
            }
            if (this->field[i][j] > 0){
                cout << this->field[i][j] << " ";
            }
            else{
                cout << "- ";
            }
        }
        cout << endl;
        if (i == 2 || i == 5){
            for (int k = 0; k < 21; ++k){
                if (k == 6 || k == 14){
                    cout << "+";
                }
                else{
                    cout << "-";
                }
            }
            cout << endl;
        }
    }
    cout << "\n---- Number of Candidates ----" << endl << endl;
    for (int i = 0; i < 9; ++i){
        for (int j = 0; j < 9; ++j){
            if (j > 0 && j % 3 == 0){
                cout << "| ";
            }
            if (this->field[i][j] == 0){
                cout << this->num_cand[i][j] << " ";
            }
            else{
                cout << "- ";
            }
        }
        cout << endl;
        if (i == 2 || i == 5){
            for (int k = 0; k < 21; ++k){
                if (k == 6 || k == 14){
                    cout << "+";
                }
                else{
                    cout << "-";
                }
            }
            cout << endl;
        }
    }
    cout << "\n---- Candidates ----" << endl << endl;
    for (int x = 0; x < 27; ++x){
        int i = x / 3;
        for (int y = 0; y < 27; ++y){
            int j = y / 3;
            int k = (x % 3)*3 + (y % 3);
            if (y > 0 && y % 3 == 0){
                if (j > 0 && j % 3 == 0){
                    cout << "  ";
                }
                else{
                    cout << "| ";
                }
            }
            if (this->cand_in_field[i][j][k]){
                cout << (k+1) << " ";
            }
            else{
                cout << "- ";
            }
        }
        cout << endl;
        if (x < 26 && (x+1) % 3 == 0){
            if ((i+1) % 3 != 0){
                for (int t=0; t < 69; ++t){
                    if ((21 <= t && t <= 23) || (45 <= t && t <= 47))
                        cout << " ";
                    else
                        cout << "-";
                }
            }
            cout << endl;
        }
    }
}

bool Sudoku::isComplete(){
    bool out = true;
    for (int i = 0; i < 9; ++i){
        for (int j = 0; j < 9; ++j){
            out &= (this->field[i][j] > 0);
        }
    }
    return out;
}

bool Sudoku::isValid(pair<int,int> pos, int val){
    int i = pos.first;
    int j = pos.second;
    if (this->field[i][j] > 0) return false;
    for (int k = 0; k < 9; ++k){
        if (this->field[k][j] == val) return false;
        if (this->field[i][k] == val) return false;
    }
    for (int k = 0; k < 3; ++k){
        for (int l = 0; l < 3; ++l){
            if (this->field[i/3*3+k][j/3*3+l] == val) return false;
        }
    }
    return true;
}