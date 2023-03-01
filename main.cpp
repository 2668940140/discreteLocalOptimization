#include "QueensFind.hpp"
#include <iostream>
#include <cstring>
#include <cmath>
using namespace std;

constexpr int SIZE = 5000;
bool board[SIZE][SIZE];
int main()
{
    clock_t mcl = clock();
    for (int t = 0; t < 1; t++)
    {
        memset(board, 0, sizeof(board));
        Alg4Dnmc_P clag(SIZE);
        const int *out = clag.solve(INT32_MAX);
        for (int i = 0; i < SIZE; i++)
            board[i][out[i]] = true;
        for (int i = 0; i < SIZE; i++)
        {
            for (int j = 0; j < SIZE; j++)
                cout << board[i][j];
            cout << endl;
        }
    }

    cout << double(clock() - mcl) / CLOCKS_PER_SEC << endl;
    cl.show();
}