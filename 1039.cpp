/* 1039. Anniversary Party - http://acm.timus.ru/problem.aspx?num=1039
 *
 * Strategy:
 * From each root, go through the employee tree in a depth-first fashion, calculating and caching
 * the total maximum conviviality for including and excluding the root node for each subtree
 * encountered.
 *
 * Performance:
 * O(N), running the tests in 0.015s using 664KB memory; one of the 3 fastest solutions for the
 * compiler used.
 */


#include <stdio.h>
#include <vector>
#include <stack>
#include <algorithm>

struct employee
{
    employee* sup; // Superior (only used to find the root nodes)
    std::vector<employee*> subs; // Subordinates
    int cur; // Next subtree to be explored when visiting this node during the depth-first search
    int incl, excl; // Maximim conviviality for this subtree when including and excluding this node
    int n; // The number of this node
};

employee e[6001];

int main()
{
    int N, ans = 0;
    scanf("%d", &N);
    for(int i = 1; i <= N; i++)
    {
        e[i].n = i;
        scanf("%d", &e[i].incl);
    }
    while(true)
    {
        int x, y;
        scanf("%d %d", &x, &y);
        if(x*y == 0)
            break;
        e[x].sup = &e[y];
        e[y].subs.push_back(e+x);
    }
    std::stack<employee*> s;
    for(int i = 1; i <= N; i++) // Push all the root nodes
        if(!e[i].sup)
            s.push(e+i);
    while(!s.empty())
    {
        auto p = s.top();
        if(p->cur < p->subs.size()) // Push the next subtree to be dfs:ed through
            s.push(p->subs[p->cur++]);
        else // We've finished searching through all the subtrees; calculate the final value and pop
        {
            for(auto it = p->subs.begin(); it < p->subs.end(); it++)
            {
                p->excl += std::max((*it)->excl, (*it)->incl);
                p->incl += (*it)->excl;
            }
            if(!p->sup)
                ans += std::max(p->excl, p->incl);
            s.pop();
        }
    }
    printf("%d\n", ans);
    return 0;
}