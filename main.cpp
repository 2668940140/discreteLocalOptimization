#include "QueensFind.hpp"
#include <iostream>
#include <cstring>
using namespace std;

const int SIZE = 100;
bool board[SIZE][SIZE];
int main()
{
    for (int i = 0; i < 1; i++)
    {
        memset(board,0,sizeof(board));
        ClimbingAlg_RandomStart<SIZE> clag;
        array<int,SIZE> out= clag.solve();
        for (int i=0;i<SIZE;i++)
            board[i][out[i]]=true;
        for (int i=0;i<SIZE;i++)
        {
            for (int j=0;j<SIZE;j++)
                cout << board[i][j];
            cout << endl;
        }
    }
}