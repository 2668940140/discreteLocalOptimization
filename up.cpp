#include<bits/stdc++.h>
using namespace std;
class Alg4Dnmc_PP
{
private:
    struct ShuffleGen
    {
        minstd_rand &genInserted;
        uniform_int_distribution<int> dtbtForShuffle;
        ShuffleGen(minstd_rand &m) : genInserted(m){};
        int operator()(int x) { return dtbtForShuffle(genInserted) % x; }
    };

    // data
    const int n;
    static int seed;
    vector<int> board = vector<int>(n);
    vector<bool> colOccupied;

    // for fast diag conflict judgement
    //[0] is i-col[i]+n-1 in [0,2n-2], [1] is i+col[i] in [0,2n-2]
    // store total num
    //[2] [3] store the representative
    vector<int> diagCfl[2];
    vector<int> buffer = vector<int>(n);
    int randomLimit = ceil(3 * n / 4);
    int conflictPairCnt;
    int *pBuffer[8];
    int badCnt = 0;
    vector<int> badInd = vector<int>(n);
    vector<pair<bool, int>> badState;
    minstd_rand gen = minstd_rand(seed);
    uniform_int_distribution<int> distribution = uniform_int_distribution<int>(0, n - 1);
    ShuffleGen shuffleGen = ShuffleGen(gen);

public:
    Alg4Dnmc_PP(int m) : n(m) {}

private:
    void calBad()
    {
        badState.assign(n, pair<bool,int>(false, -1));
        badCnt = 0;
        for (int i = 0; i < n; i++)
        {
            if (diagCfl[0][i - board[i] + n - 1] > 1 || diagCfl[1][i + board[i]] > 1)
            {
                badState[i].first = true;
                badState[i].second = badCnt;
                badInd[badCnt++] = i;
            }
        }
    }

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
            //random_shuffle(buffer.begin(),buffer.end(),shuffleGen);
            for (int i=0;i< n;i++)
            {
                int j = distribution(gen);
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
            for (; queenOk < n; queenOk++)
        {
            bool found = false;
            //random_shuffle(buffer.begin(),buffer.end(),shuffleGen);
            for (int i=0;i< n;i++)
            {
                int j = i;
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
        calBad();
        return queenOk;
    }

    inline int calCost(int x, int y)
    {
        // assert(i!=j);
        int out = 0;
        pBuffer[0] = &diagCfl[0][x - board[x] + n - 1];
        pBuffer[1] = &diagCfl[1][x + board[x]];
        pBuffer[2] = &diagCfl[0][y - board[y] + n - 1];
        pBuffer[3] = &diagCfl[1][y + board[y]];
        pBuffer[4] = &diagCfl[0][x - board[y] + n - 1];
        pBuffer[5] = &diagCfl[1][x + board[y]];
        pBuffer[6] = &diagCfl[0][y - board[x] + n - 1];
        pBuffer[7] = &diagCfl[1][y + board[x]];
        for (int i = 0; i < 4; i++)
            out -= --(*pBuffer[i]);
        for (int i = 4; i < 8; i++)
            out += (*pBuffer[i])++;
        for (int i = 0; i < 4; i++)
            (*pBuffer[i])++;
        for (int i = 4; i < 8; i++)
            --(*pBuffer[i]);
        return out;
    }

    inline void swapQueens(int x, int y)
    {
        pBuffer[0] = &diagCfl[0][x - board[x] + n - 1];
        pBuffer[1] = &diagCfl[1][x + board[x]];
        pBuffer[2] = &diagCfl[0][y - board[y] + n - 1];
        pBuffer[3] = &diagCfl[1][y + board[y]];
        pBuffer[4] = &diagCfl[0][x - board[y] + n - 1];
        pBuffer[5] = &diagCfl[1][x + board[y]];
        pBuffer[6] = &diagCfl[0][y - board[x] + n - 1];
        pBuffer[7] = &diagCfl[1][y + board[x]];
        for (int i = 0; i < 4; i++)
            --(*pBuffer[i]);
        for (int i = 4; i < 8; i++)
            (*pBuffer[i])++;
        if (*pBuffer[4] == *pBuffer[5] == 1 && badState[x].first)
        {
            badState[x].first = false;
            swap(badInd[badState[x].second],badInd[--badCnt]);
            badState[badInd[badState[x].second]].second=badState[x].second;
        }
            if (*pBuffer[5] == *pBuffer[6] == 1 && badState[y].first)
        {
            badState[y].first = false;
            swap(badInd[badState[y].second],badInd[--badCnt]);
            badState[badInd[badState[y].second]].second=badState[y].second;
        }
        swap(board[x], board[y]);
    }

public:
    const int *solve(int maxRounds = INT32_MAX)
    {
        while (maxRounds--)
        {
            preGenerate();
            bool found = true;
            for (int i = 0; i < n; i++)
                buffer[i] = i;
            while (found)
            {
                found = false;
                if (conflictPairCnt == 0)
                    return &board[0];
                if (badCnt==0)
                    calBad();
                int first, second;
                int tmpCost;

                for (int i = 0; i < n; i++)
                {
                    first = badInd[shuffleGen(badCnt)], second = distribution(gen);
                    if (buffer[i] == first)
                        continue;
                    if ((tmpCost = calCost(first, buffer[i])) >= 0)
                        continue;
                    found = true;
                    swapQueens(first, buffer[i]);
                    conflictPairCnt += tmpCost;
                    break;
                }
            }
        }
        return nullptr;
    }
};
int Alg4Dnmc_PP::seed = 0;

int main()
{
    cin.sync_with_stdio(false);
    cin.tie(nullptr);
    int n;
    const int *ans;
    while (cin >> n && n)
    {
        Alg4Dnmc_PP solver(n);
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