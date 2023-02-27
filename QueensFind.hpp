#include "DisLocOpt.hpp"
#include <set>
#include <random>
#include <ctime>
#include <cmath>
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
    set<Queen> board;

public:
    Node()
    {
        for (int i = 0; i < n; i++)
            board.insert({i, i});
    }

    void randomize()
    {
        board.clear();

        minstd_rand gen(time(nullptr));
        uniform_int_distribution distribution(0, n - 1);
        for (int i = 0; i < n; i++)
        {
            board.insert({i, distribution(gen)});
        }
        return;
    }

    // return the number of fighting queens pairs in O(n^2)
    virtual long long cost() const//<= 0 or stuck -> end
    {
        long long ans = 0;
        auto last = prev(board.end());
        for (auto p1 = board.begin(); p1 != last; p1++)
            for (auto p2 = next(p1); p2 != board.end(); p2++)
            {
                if (p1->row == p2->row || p1->col == p2->col ||
                    abs(p1->row - p2->row) == abs(p1->col - p2->col))
                    ans++;
            }
        return ans;
    }

    virtual vector<Node> neighbors() const
    {
        vector<Node> out;
        Node single;
        for (auto p = board.begin(); p != board.end(); p++)
        {
            if (p->col > 0)
            {
                single = *this;
                single.board.erase(single.board.find(*p));
                single.board.insert(Queen(p->row, p->col - 1));
                out.emplace_back(single);
            }
            if (p->col < n - 1)
            {
                single = *this;
                single.board.erase(single.board.find(*p));
                single.board.insert(Queen(p->row, p->col + 1));
                out.emplace_back(single);
            }
        }
        return out;
    }

    const set<Queen> &content() const { return board; };
};