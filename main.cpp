#include "QueensFind.hpp"
#include <iostream>
using namespace std;

const int SIZE = 8;
bool board[SIZE][SIZE];
int main()
{
    
    Node<SIZE> initalState;
    initalState.randomize();
    
    ClimbingAlg<Node<SIZE>> clAlg(initalState);
    Node ans = clAlg.solve();
    for (auto& val: ans.content()) board[val.row][val.col]=true;
    for (int i=0;i<SIZE;i++)
    {
        for (int j=0;j<SIZE;j++)
        {
            cout<<board[i][j]?' ':'*';
        }
        cout << endl;
    }
    
}