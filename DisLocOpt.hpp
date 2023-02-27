#include <vector>
#include <algorithm>
#include <cstdint>
#include <random>
#include <ctime>

using namespace std;

class BasicNode
{
public:
    virtual double cost(); //<= 0 or stuck -> end
    virtual vector<BasicNode> neighbors();
    virtual void randomize() const;
};

template <typename Node>
class ClimbingAlg
{
private:
    Node now;

public:
    ClimbingAlg(const Node &iniState) : now(iniState) {}

    Node solve(int maxSteps = INT32_MAX)
    {
        while (maxSteps--)
        {
            double nowCost = now.cost();
            if (nowCost <= 0)
                return now;
            vector<Node> storage = now.neighbors();
            auto p = min_element(storage.begin(), storage.end(),
                                 [](Node &m1, Node &m2)
                                 { return m1.cost() < m2.cost(); });
            long long bestCost = p->cost();
            if (bestCost >= nowCost)
                return now;
            now = *p;
        }

        return now;
    }
};

template <typename Node>
class ClimbingAlg_RandomTranslation
{
private:
    Node now;
    static int seed;

public:
    ClimbingAlg_RandomTranslation(const Node &iniState) : now(iniState) {}

    Node solve(int maxSteps = INT32_MAX, int maxRandomTranslation = 10)
    {
        int leftTranslation = maxRandomTranslation;
        while (maxSteps--)
        {
            double nowCost = now.cost();
            if (nowCost <= 0)
                return now;
            vector<Node> storage = now.neighbors();

            auto p = min_element(storage.begin(), storage.end(),
                                 [](Node &m1, Node &m2)
                                 { return m1.cost() < m2.cost(); });
            long long bestCost = p->cost();
            if (bestCost<nowCost)
            {
                leftTranslation = maxRandomTranslation;
                now = *p;
                continue;
            }
            if (bestCost>nowCost) return now;
            if (leftTranslation--)
            {
                vector<int> choices;
                for (int i=0;i<storage.size();i++)
                {
                    if (storage[i].cost()==bestCost)
                        choices.emplace_back(i);
                }
                minstd_rand0 gen(seed);
                uniform_int_distribution<int> distribution(0,choices.size());
                now = storage[choices[distribution(gen)]];
                seed = distribution(gen);
            }
            else return now;
        }
        return now;
    }
};

template<typename Node>
int ClimbingAlg_RandomTranslation<Node>::seed = time(nullptr);

