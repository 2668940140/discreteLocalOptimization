#include <vector>
#include <algorithm>
#include <cstdint>
using namespace std;

class BasicNode
{
    public:
    virtual double cost() const; //<= 0 or stuck -> end
    virtual vector<BasicNode> neighbors() const;
};

template<typename Node>
class ClimbingAlg
{
    private:
    Node now;

    public:
    ClimbingAlg(const Node& iniState) : now(iniState) {}
    
    Node solve(int maxSteps = INT32_MAX)
    {

        while (maxSteps--)
        {
        double nowCost = now.cost();
        if (nowCost<=0) return now;
        vector<Node> storage = now.neighbors();
        auto p= min_element(storage.begin(),storage.end(),
        [](const Node& m1, const Node& m2){return m1.cost() < m2.cost();}
        );
        long long bestCost = p->cost();
        if (bestCost>=nowCost) return now;
        now = *p;
        }

        return now;
        
    }
};