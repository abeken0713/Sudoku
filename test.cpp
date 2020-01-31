#include <vector>
#include <iostream>
#include <algorithm>
#include <iterator>
using namespace std;

void print(vector< vector<int> >& vec){
    for (size_t i = 0; i < vec.size(); ++i){
        for (size_t j = 0; j < vec[i].size(); ++j){
            cout << vec[i][j] << " ";
        }
        cout << endl;
    }
}

int main(){
    vector< vector<int> > v1, v2;
    v1.assign(3, vector<int>(4, 1));
    cout << "v1" << endl;
    print(v1);

    copy(v1.begin(), v1.end(), back_inserter(v2));
    cout << "v2" << endl;
    print(v2);

    v2[0][0] = 5;
    cout << "v2" << endl;
    print(v2);
    cout << "v1" << endl;
    print(v1);
}