#include "QueensFind.hpp"
#include <iostream>
#include <cstring>
#include <cmath>
using namespace std;

constexpr int SIZE = 100000;
bool board[SIZE][SIZE];
int main()
{
    clock_t mcl = clock();
    for (int t = 0; t < 1; t++)
    {
        Alg4Dnmc_PP clag(SIZE);
        const int *out = clag.solve(INT32_MAX);
        memset(board,0,sizeof(board));
        for (int i=0;i<SIZE;i++)
            cout << out[i] << ' ';
        /*
        for (int i=0;i<SIZE;i++)
        {
            for (int j=0;j<SIZE;j++)
            {
                cout << (board[i][j]?'*':'.');
            }
            cout << '\n';
        }
        */
        cout << endl;
    }

    cout << double(clock() - mcl) / CLOCKS_PER_SEC << endl;
    cl.show();
}