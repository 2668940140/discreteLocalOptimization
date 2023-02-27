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

template <int n>
class ClimbingAlg_RandomStart
{
private:
    static int seed;
    int board[n];
    bool dp[n][n];
    bool mem[n];
    minstd_rand gen = minstd_rand(seed);
    uniform_int_distribution<int> distrubution = uniform_int_distribution(0,n-1);

    inline int cost()
    {
        int out = 0;
        memset(dp, 0, sizeof(dp));
        for (int i = 0; i < n - 1; i++)
            for (int j = i+1; j < n; j++)
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
                            for (int k=0;k<i;k++)
                                dp[k][i]=mem[k];
                            for (int k=i+1;k<n;k++)
                                dp[i][k]=mem[k];
                            found = true;
                            goto toNext;
                        }
                    }
                    board[i]=pre;
                }
            toNext:
                if (!found) break;
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
int ClimbingAlg_RandomStart<n>::seed = time(nullptr);