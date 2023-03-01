#include "QueensFind.hpp"
#include <iostream>
#include <cstring>
#include <cmath>
using namespace std;

constexpr int SIZE = 300;
bool board[SIZE][SIZE];
int main()
{
    clock_t cl = clock();
    for (int i = 0; i < 10; i++)
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
    cout << double(clock() - cl) / CLOCKS_PER_SEC;
}