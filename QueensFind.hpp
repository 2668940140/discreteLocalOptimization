#include "DisLocOpt.hpp"
#include <set>
#include <random>
#include <ctime>
#include <cmath>
#include <cstring>
using namespace std;

struct Queen
{
    int row, col;
    Queen(int x, int y)
    {
        row = x;
        col = y;
    }
    bool operator<(const Queen &m) const
    {
        return row < m.row || row == m.row && col < m.col;
    }
};

template <int n>
class Node
{
private:
    static int seed;
    set<Queen> board;
    long long knownCost = -1;

public:
    Node()
    {
        for (int i = 0; i < n; i++)
            board.insert({i, i});
    }

    void randomize()
    {
        board.clear();
        minstd_rand gen(seed);
        uniform_int_distribution distribution(0, n - 1);
        for (int i = 0; i < n; i++)
        {
            board.insert({i, distribution(gen)});
        }
        seed = distribution(gen);
        knownCost = -1;
        return;
    }

    // return the number of fighting queens pairs in O(n^2)
    virtual long long cost() //<= 0 or stuck -> end
    {
        if (knownCost != -1)
            return knownCost;
        long long ans = 0;
        auto last = prev(board.end());
        for (auto p1 = board.begin(); p1 != last; p1++)
            for (auto p2 = next(p1); p2 != board.end(); p2++)
            {
                if (p1->row == p2->row || p1->col == p2->col ||
                    abs(p1->row - p2->row) == abs(p1->col - p2->col))
                    ans++;
            }
        knownCost = ans;
        return ans;
    }

    virtual vector<Node> neighbors() const
    {
        vector<Node> out;
        Node single;
        for (auto p = board.begin(); p != board.end(); p++)
        {
            Node base = *this;
            base.board.erase(*p);
            base.knownCost = -1;
            for (int i = 0; i < n; i++)
            {
                if (i == p->col)
                    continue;
                single = base;
                single.board.insert(Queen(p->row, i));
                out.emplace_back(single);
            }
        }
        return out;
    }

    const set<Queen> &content() const { return board; };
};
template <int n>
int Node<n>::seed = time(nullptr);

// find the first better
template <int n>
class ClimbingAlg_RandomStart_FirstBetter
{
private:
    static int seed;
    int board[n];
    bool dp[n][n];
    bool mem[n];
    minstd_rand gen = minstd_rand(seed);
    uniform_int_distribution<int> distrubution = uniform_int_distribution(0, n - 1);

    inline int cost()
    {
        int out = 0;
        memset(dp, 0, sizeof(dp));
        for (int i = 0; i < n - 1; i++)
            for (int j = i + 1; j < n; j++)
            {
                if (board[i] == board[j] || abs(i - j) == abs(board[i] - board[j]))
                {
                    dp[i][j] = true;
                    out++;
                }
            }
        return out;
    }

    inline int costChanged(int pos) // the row that has changed
    {
        int out = 0;
        for (int i = 0; i < pos; i++)
        {
            if (dp[i][pos])
                out--;
            if (board[i] == board[pos] || abs(i - pos) == abs(board[i] - board[pos]))
            {
                mem[i] = true;
                out++;
            }
            else
                mem[i] = false;
        }
        for (int i = pos + 1; i < n; i++)
        {
            if (dp[pos][i])
                out--;
            if (board[i] == board[pos] || abs(i - pos) == abs(board[i] - board[pos]))
            {
                mem[i] = true;
                out++;
            }
            else
                mem[i] = false;
        }
        return out;
    }

public:
    array<int, n> solve(int maxSteps = INT32_MAX, int maxRounds = INT32_MAX)
    {
        while (maxRounds--)
        {
            memset(board, 0, sizeof(board));
            for (int i = 0; i < n; i++)
            {
                board[i] = distrubution(gen);
            }
            int costNow = cost();
            if (costNow == 0)
            {
                array<int, n> ar;
                copy(board, board + n, ar.begin());
                return ar;
            }

            int nowSteps = maxSteps;
            while (costNow > 0 && nowSteps--)
            {
                bool found = false;
                for (int i = 0; i < n; i++)
                {
                    int pre = board[i];
                    for (int j = 0; j < n; j++)
                    {
                        if (j == pre)
                            continue;
                        board[i] = j;
                        int costDp = costChanged(i);
                        if (costDp < 0)
                        {
                            costNow += costDp;
                            for (int k = 0; k < i; k++)
                                dp[k][i] = mem[k];
                            for (int k = i + 1; k < n; k++)
                                dp[i][k] = mem[k];
                            found = true;
                            goto toNext;
                        }
                    }
                    board[i] = pre;
                }
            toNext:
                if (!found)
                    break;
            }

            if (costNow == 0)
            {
                array<int, n> ar;
                copy(board, board + n, ar.begin());
                return ar;
            }
        }
        array<int, n> ar;
        return ar;
    }
};
template <int n>
int ClimbingAlg_RandomStart_FirstBetter<n>::seed = time(nullptr);
// find the first best
template <int n>
class ClimbingAlg_RandomStart_FirstBest
{
private:
    static int seed;
    int board[n];
    bool dp[n][n];
    bool mem[n];
    bool bestMem[n];
    minstd_rand gen = minstd_rand(seed);
    uniform_int_distribution<int> distrubution = uniform_int_distribution(0, n - 1);

    inline int cost()
    {
        int out = 0;
        memset(dp, 0, sizeof(dp));
        for (int i = 0; i < n - 1; i++)
            for (int j = i + 1; j < n; j++)
            {
                if (board[i] == board[j] || abs(i - j) == abs(board[i] - board[j]))
                {
                    dp[i][j] = true;
                    out++;
                }
            }
        return out;
    }

    inline int costChanged(int pos) // the row that has changed
    {
        int out = 0;
        for (int i = 0; i < pos; i++)
        {
            if (dp[i][pos])
                out--;
            if (board[i] == board[pos] || abs(i - pos) == abs(board[i] - board[pos]))
            {
                mem[i] = true;
                out++;
            }
            else
                mem[i] = false;
        }
        for (int i = pos + 1; i < n; i++)
        {
            if (dp[pos][i])
                out--;
            if (board[i] == board[pos] || abs(i - pos) == abs(board[i] - board[pos]))
            {
                mem[i] = true;
                out++;
            }
            else
                mem[i] = false;
        }
        return out;
    }

public:
    array<int, n> solve(int maxSteps = INT32_MAX, int maxRounds = INT32_MAX)
    {
        while (maxRounds--)
        {
            memset(board, 0, sizeof(board));
            for (int i = 0; i < n; i++)
            {
                board[i] = distrubution(gen);
            }
            int costNow = cost();
            if (costNow == 0)
            {
                array<int, n> ar;
                copy(board, board + n, ar.begin());
                return ar;
            }

            int nowSteps = maxSteps;
            while (costNow > 0 && nowSteps--)
            {
                bool found = false;
                int bestChange = 0;
                int bestChangeMethod[2] = {0};
                for (int i = 0; i < n; i++)
                {
                    int pre = board[i];
                    for (int j = 0; j < n; j++)
                    {
                        if (j == pre)
                            continue;
                        board[i] = j;
                        int costDp = costChanged(i);
                        if (costDp < bestChange)
                        {
                            bestChange = costDp;
                            bestChangeMethod[0] = i;
                            bestChangeMethod[1] = j;
                            memcpy(bestMem, mem, sizeof(mem));
                            found = true;
                        }
                    }
                    board[i] = pre;
                }
                if (!found)
                    break;
                costNow += bestChange;
                board[bestChangeMethod[0]] = bestChangeMethod[1];
                for (int i = 0; i < bestChangeMethod[0]; i++)
                    dp[i][bestChangeMethod[0]] = bestMem[i];
                for (int i = bestChangeMethod[0] + 1; i < n; i++)
                    dp[bestChangeMethod[0]][i] = bestMem[i];
            }

            if (costNow == 0)
            {
                array<int, n> ar;
                copy(board, board + n, ar.begin());
                return ar;
            }
        }
        array<int, n> ar;
        return ar;
    }
};
template <int n>
int ClimbingAlg_RandomStart_FirstBest<n>::seed = time(nullptr);


inline int calSuitVal(int* ar, int n, int out)
{
    for (int i=0;i<n-1;i++)
        for (int j=i+1;j<n;j++)
            if (ar[i]==ar[j]||abs(i-j)==abs(ar[i]-ar[j]))
                out--;
    return out;
}

template <int n,int popSize>
class GeneticAlg
{
private:
    static int seed;
    int pop[2][popSize][n];
    int suitVal[popSize];
    int suitLineForDistribution[popSize];
    int targetSuitVal = n*(n-1)/2;
    minstd_rand gen = minstd_rand(seed);
    uniform_int_distribution<int> uniDibut = uniform_int_distribution(0, n - 1);

//return val (min,max)
pair<int,int> randomizePop()
{
    pair<int,int> out{INT32_MAX,0};
    for (int i=0;i<popSize;i++)
        for (int j=0;j<n;j++)
            pop[0][i][j]=uniDibut(gen);
    for (int i=0;i<popSize;i++)
    {
        suitVal[i]=calSuitVal(pop[0][i],n,targetSuitVal);
        out.first=min(suitVal[i],out.first);
        out.second=max(suitVal[i],out.second);
    }
    return out;
}
public:
    array<int,n> solve(double muteProbility = 0.1)
    {
        bernoulli_distribution bnlDibut(muteProbility);

        pair<int,int> minmaxSuitVal = randomizePop();
        int popSwitch=0;
        while (minmaxSuitVal.second<targetSuitVal)
        {
            for (int i=0;i<popSize;i++)
            {
                suitLineForDistribution[i]=suitVal[i]-minmaxSuitVal.first;
            }
            discrete_distribution dctDibut(suitLineForDistribution,suitLineForDistribution+popSize);
            int prePopSwitch = popSwitch;
            popSwitch=(popSwitch+1)%2;
            
            minmaxSuitVal=pair<int,int>(INT32_MAX,0);
            for (int i=0;i<popSize;i++)
            {
                int choice[2] = {dctDibut(gen),dctDibut(gen)};
                int breakPoint = uniDibut(gen);
                
                for (int j=0;j<breakPoint;j++)
                    pop[popSwitch][i][j]=pop[prePopSwitch][choice[0]][j];
                for (int j=breakPoint;j<n;j++)
                    pop[popSwitch][i][j]=pop[prePopSwitch][choice[1]][j];
                if (bnlDibut(gen))
                {
                    pop[popSwitch][i][uniDibut(gen)]=uniDibut(gen);
                }
                suitVal[i]=calSuitVal(pop[popSwitch][i],n,targetSuitVal);
                minmaxSuitVal.first=min(minmaxSuitVal.first,suitVal[i]);
                minmaxSuitVal.second=max(minmaxSuitVal.second,suitVal[i]);
            }  
        }
        array<int,n> ans;
        int bestIndex = max_element(suitVal,suitVal+popSize)-suitVal;
        copy(pop[popSwitch][bestIndex],pop[popSwitch][bestIndex]+n,ans.begin());
        calSuitVal(pop[popSwitch][bestIndex],n,targetSuitVal);
        return ans;
    }
};
template <int n,int popSize>
int GeneticAlg<n,popSize>::seed = time(nullptr);