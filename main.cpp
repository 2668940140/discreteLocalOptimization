#include "QueensFind.hpp"
#include <iostream>
#include <cstring>
#include <cmath>
using namespace std;

constexpr int SIZE = 1000;
bool board[SIZE][SIZE];
int main()
{
    for (int i = 0; i < 1; i++)
    {
        memset(board,0,sizeof(board));
        Alg4Dnmc clag(SIZE);
        const int* out = clag.solve(INT32_MAX);
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