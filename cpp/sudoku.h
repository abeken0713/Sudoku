#include <iostream>
#include <vector>
#include <utility>
#include <set>
using namespace std;

class Sudoku{
private:
    vector< vector<int> > field;  // 9x9 [0-9]
    vector< vector<bool> > original;
    vector< set< pair<int,int> > > cand_per_num;
    vector< vector< vector<bool> > > cand_in_field;
    vector< vector<int> > num_cand;
public:
    Sudoku(char* _filename);
    Sudoku(vector< vector<int> > _field);
    void _readFile(char* _filename);
    void _initField(vector< vector<int> > _field);
    void _updateCandidates();
    void fill(pair<int,int> pos, int val);
    void show();
    bool isComplete();
    bool isValid(pair<int,int> pos, int val);
};