#include<bits/stdc++.h>
using namespace std;
class Alg4Dnmc
{
private: 
    struct ShuffleGen
    {
        minstd_rand& genInserted;
        uniform_int_distribution<int> dtbtForShuffle;
        ShuffleGen(minstd_rand& m) : genInserted(m) {};
        int operator ()(int x) {return dtbtForShuffle(genInserted)%x;}
    };
    
    // data
    const int n;
    static int seed;
    vector<int> board = vector<int>(n);
    vector<bool> colOccupied;

    // for fast diag conflict judgement
    //[0] is i-col[i]+n-1 in [0,2n-2], [1] is i+col[i] in [0,2n-2]
    // store total num
    vector<int> diagCfl[2];
    vector<int> buffer = vector<int>(n);
    int conflictPairCnt;
    int* pBuffer[8];

    minstd_rand gen = minstd_rand(seed);
    uniform_int_distribution<int> distribution = uniform_int_distribution<>(0, n - 1);
    ShuffleGen shuffleGen = ShuffleGen(gen);

public:
    Alg4Dnmc(int m) : n(m) {}
private:
    int preGenerate() // random generate the maxNumber in tryTimes for each place
    {
        colOccupied.assign(n,0);
        diagCfl[0].assign(2*n-1,0);
        diagCfl[1].assign(2*n-1,0);
        for (int i=0;i<n;i++)
            buffer[i]=i;
        conflictPairCnt = 0;
        int queenOk;
        for (queenOk = 0; queenOk < n; queenOk++)
        {
            bool found = false;
            random_shuffle(buffer.begin(),buffer.end(),shuffleGen);
            for (int i=0;i<n;i++)
            {
                int j = buffer[i];
                if (colOccupied[j] || diagCfl[0][queenOk - j + n - 1] || diagCfl[1][queenOk + j])
                    continue;
                found = true;
                board[queenOk] = j;
                colOccupied[j] = true;
                diagCfl[0][queenOk - j + n - 1] = 1;
                diagCfl[1][queenOk + j] = 1;
                break;
            }
            if (!found)
                break;
        }

        for (int i = queenOk; i < n; i++)
            buffer[i-queenOk] = i;
        random_shuffle(&buffer[0], &buffer[0] + n - queenOk, shuffleGen);
        for (int j = 0, i=0; j < n; j++)
        {
            if (colOccupied[j])
                continue;
            colOccupied[j] = true;
            conflictPairCnt += diagCfl[0][buffer[i] - j + n - 1]++;
            conflictPairCnt += diagCfl[1][buffer[i] + j]++;
            board[buffer[i++]] = j;
        }
        return queenOk;
    }

    inline int calCost(int x, int y)
    {
        // assert(i!=j);
        int out = 0;
        pBuffer[0] = &diagCfl[0][x-board[x]+n-1];
        pBuffer[1] = &diagCfl[1][x+board[x]];
        pBuffer[2] = &diagCfl[0][y-board[y]+n-1];
        pBuffer[3] = &diagCfl[1][y+board[y]];
        pBuffer[4] = &diagCfl[0][x-board[y]+n-1];
        pBuffer[5] = &diagCfl[1][x+board[y]];
        pBuffer[6] = &diagCfl[0][y-board[x]+n-1];
        pBuffer[7] = &diagCfl[1][y+board[x]];
        for (int i=0;i<4;i++)
            out -= --(*pBuffer[i]);
        for (int i=4;i<8;i++)
            out += (*pBuffer[i])++;
        for (int i=0;i<4;i++)
            (*pBuffer[i])++;
        for (int i=4;i<8;i++)
            --(*pBuffer[i]);
        return out;
    }

    inline void swapQueens(int x, int y)
    {
        pBuffer[0] = &diagCfl[0][x-board[x]+n-1];
        pBuffer[1] = &diagCfl[1][x+board[x]];
        pBuffer[2] = &diagCfl[0][y-board[y]+n-1];
        pBuffer[3] = &diagCfl[1][y+board[y]];
        pBuffer[4] = &diagCfl[0][x-board[y]+n-1];
        pBuffer[5] = &diagCfl[1][x+board[y]];
        pBuffer[6] = &diagCfl[0][y-board[x]+n-1];
        pBuffer[7] = &diagCfl[1][y+board[x]];
        for (int i=0;i<4;i++)
            --(*pBuffer[i]);
        for (int i=4;i<8;i++)
            (*pBuffer[i])++;
        swap(board[x],board[y]);
    }

public:
    const int *solve(int maxRounds = INT32_MAX)
    {
        while (maxRounds--)
        {
            int queenOk = preGenerate();
            bool found = true;
            uniform_int_distribution<int> dtbt(queenOk, n - 1);
            for (int i = 0; i < n; i++)
                buffer[i] = i;
            while (found)
            {
                found = false;
                if (conflictPairCnt==0)
                    return &board[0];
                int first = dtbt(gen), second;
                int tmpCost;
                random_shuffle(buffer.begin(), buffer.end(), shuffleGen);
                for (int i = 0; i < n; i++)
                {
                    if (buffer[i] == first)
                        continue;
                    if ((tmpCost=calCost(first,buffer[i]))>=0) continue;
                    found = true;
                    swapQueens(first,buffer[i]);
                    conflictPairCnt += tmpCost;
                    break;
                }
            }
        }
        return nullptr;
    }
};
int Alg4Dnmc::seed = time(nullptr);

int main()
{
    cin.sync_with_stdio(false);
    cin.tie(nullptr);
    int n;
    const int *ans;
    while (cin >> n && n)
    {
        Alg4Dnmc solver(n);
        ans = solver.solve();
        for (int i=0;i<n;i++)
        {
            cout << ans[i] + 1;
            if (i != n-1) cout << ' ';
        }
        cout << endl;
    }
    return 0;
}