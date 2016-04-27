/* 1822. Hugo II's War - http://acm.timus.ru/problem.aspx?num=1822
 *
 * Strategy:
 * For a given x, it's straightforward to calculate how many days are required to summon the army
 * by traversing the tree depth-first. Then, binary search can be utilized on x to find the optimal
 * value.
 *
 * Performance:
 * O(n log n log(1/e)) where e is the error margin, runs the test cases in 0.046s using
 * 1,476KB memory. It is possible to run this linearly in n if we use a linear k-th smallest
 * algorithm.
 */

#include <stdio.h>
#include <vector>
#include <algorithm>

struct Node
{
    std::vector<Node*> v; // Children
    int t;
} nodes[10001];

int dfs(double x, Node* node)
{
    std::vector<std::pair<int, Node*>> times;
    times.reserve(node->v.size());
    for(auto n : node->v)
        times.push_back( { dfs(x, n), n } );
    // Find the x%th vassal by time
    std::sort(times.begin(), times.end());
    return node->t + (!node->v.empty() ? times[std::ceil((times.size())*(x/100.0))-1].first : 0.0);
}

float bin(double l, double r, int maxd) // Binary search
{
    while(true)
    {
        if(std::abs(l-r) < 1e-6)
            return l;
        double mid = (l+r)/2;
        int d = dfs(mid, nodes+1);
        if(d > maxd)
            r = mid;
        else
            l = mid;
    }
}

int main()
{
    int n, d, p, t;
    scanf("%d %d", &n, &d);
    for(int i = 2; i <= n; i++)
    {
        scanf("%d %d", &p, &t);
        nodes[p].v.push_back(nodes + i);
        nodes[i].t = t;
    }
    printf("%lf\n", bin(0, 100, d));
}