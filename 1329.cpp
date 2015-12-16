/* 1329. Galactic History - http://acm.timus.ru/problem.aspx?num=1329
 *
 * Strategy:
 * Remap every id in the input (including queries) to a number such that every child has a larger
 * id than all its ancestors. For each node, include a skiplist that keeps track of the 2^i'th
 * parent of each node (eg. its parent, grandparent, grandparents grandparent, etc). Then, given
 * some query, we can quickly iterate through the lineage using the skiplists of the ancestors 
 * of the query point until we possibly find the other node we are looking for.
 *
 * Performance:
 * O(n*log n), running the test cases in 0.249s using 7292KB memory.
 */

#include <stdio.h>
#include <cctype>
#include <vector>
#include <queue>
#include <algorithm>

inline int getint()
{
    int ret = 0, ch, minus = 1;
    while(!isdigit(ch = getc(stdin)))
        if(ch == '-')
            minus = -1;
    ret = ch - '0';
    while(isdigit(ch = getc(stdin)))
        ret = ret*10 + ch - '0';
    return ret*minus;
}

struct node
{
    int id;
    std::vector<node*> p; // Parent skip list
    std::vector<node*> v; // List of children, for constructing the above
};

node nodes[40001];
unsigned short map[40001];

void dfs(node* n, node* p)
{
    int i = 0; // Power of the parent, 2^i (2^1 is the grandparent, etc)
    for(n->p.push_back(p); p; p = p->p.size() > i ? p->p[i++] : 0) ; // Find the 2^it'h parents
    for(auto m : n->v)
        dfs(m, n);
    n->v.clear();
}

int main()
{
    int N = getint(), L, id = 0;
    node* root;
    while(N--)
    {
        int x = getint(), y = getint();
        if(y != -1)
            nodes[y].v.push_back(nodes + x), nodes[y].id = y, nodes[x].id = x;
        else
            root = nodes + x;
    }
    std::queue<node*> q;
    q.push(root);

    while(!q.empty()) // Do BFS to remap input ids
    {
        node* n = q.front();
        q.pop();
        map[n->id] = id;
        n->id = id++;
        for(auto v : n->v)
            q.push(v);
    }
    dfs(root, 0);
    L = getint();
    while(L--)
    {
        int A = getint(), B = getint(), ans = 1;
        int a = map[A], b = map[B];
        if(a > b)
            std::swap(A, B), std::swap(a, b), ans = 2;
        for(auto n = nodes[B];;)
        {
            // Skip through the parent list to find the first parent with an id not less than
            // what we are looking for
            auto it = n.p.rbegin();
            while(it != n.p.rend() && (*it)->id < a)
                it++;
            if(it == n.p.rend())
            {
                printf("0\n", ans);
                break;
            }
            else if(*it == nodes+A)
            {
                printf("%d\n", ans);
                break;
            }
            n = **it; // Do the same thing but now starting closer to our target
        }
    }
    return 0;
}