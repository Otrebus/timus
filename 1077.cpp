/* 1077. Travelling Tours - http://acm.timus.ru/problem.aspx?num=1077
 *
 * Strategy:
 * The basic idea is to construct a spanning tree of the given graph, and for every edge in the
 * graph not in the spanning tree, we create one new cycle and add this to the answer. The fact
 * that we can construct such a cycle from a spanning tree and an extra edge is given from the
 * definition of a spanning tree, but it is not entirely obvious that each such addition of an
 * edge into the spanning tree gives rise to only one new unique cycle, so let's prove it by
 * induction over the number of edges added to a spanning tree:
 *   For n = 1, a single edge added to a spanning tree gives rise to one unique cycle: the edge 
 * added and the two branches up to their least common ancestor.
 *   Assume that we have added n = k edges to the original spanning tree, and that we have 
 * identified the maximum number of cycles, k, where each cycle has at least one edge not belonging
 * to any other cycle. For the case of n = k + 1, adding another edge gives rise to a new unique
 * cycle by having that edge as the unique part of the cycle and only using the spanning tree edges
 * as the rest of the cycle, as described above. Now, assume that we can add a second cycle into the
 * solution set using this edge (cycles not using this edge could have been identified before this
 * step, which would have violated our assumption that we have identified the maximum number of
 * cycles at step k, so only the currently added edge can give rise to another cycle). Let the
 * notation [v, u] denote some chain of nodes from v to u inclusive. Assume without loss of
 * generality that the first cycle consists of the original node [u1, u2] followed by a continuous
 * chain T of nodes whose individual nodes we denote by t. The second cycle added by edge [u1, u2]
 * would then have to start at the added edge, would continue through a possibly empty set [t1, t2]
 * used by the first cycle, then break off into some chain [s1, s2], then into another possibly
 * empty set [t4, t5], directly back into [u1, u2]. Since the t nodes have no unused edges (they
 * were used by the first cycle), the unique edge must exist somewhere in [s1, s2]. But that means
 * that the cycle [s1, s2] followed by T back into [s1, s2] would also have been a new unique cycle,
 * and since this cycle already existed before step k+1, we arrive at a contradiction by violating
 * the induction assumption of having identified the maximum number of cycles. Hence, only one
 * cycle can be added per each edge extraneous to the minimum spanning tree. 
 *   The number of edges extraneous to the spanning tree(s) of a graph is M - N + C, where C is the
 * number of components of the graph, so this is also the number of unique cycles that we will
 * identify. 
 * In actually constructing the solution, we conceptually build the minimum spanning tree at the
 * same time as we add solution edges in a single depth-first search. Whenever we run into a node
 * that was already explored, we know that this will not be part of the minimum spanning tree and
 * instead will be part of a new cycle of the solution set, so we can backtrack through our current
 * DFS path from the root to add the cycle to the solution set.
 *
 * Performance:
 * O(N*M) since each edge extraneous to the spanning tree gives rise to up to a chain of nodes. The
 * solution runs in 0.062s using 1200KB memory.
 */

#include <stdio.h>
#include <vector>

class Node
{
public:
    int id;
    bool visited;
    std::vector<Node*> adj;
};

const int maxn = 200;

Node nodes[maxn+1];
Node* s[maxn*maxn]; // The stack of pending nodes
Node* t[maxn]; // The nodes currently in the DFS path
int depth[maxn+1]; // The position of each node in the DFS stack

inline std::vector<int> makePath(Node* n, Node* v) // Adds a path to the solution set
{
    std::vector<int> path;
    path.reserve(1+depth[n->id]-depth[v->id]);
    for(int i = depth[v->id]; i <= depth[n->id]; i++)
        path.push_back(t[i]->id);
    return path;
}

void dfs(int i, std::vector<std::vector<int>>& ans)
{
    if(nodes[i].visited)
        return;
    int sp = 0, tp = 0; // Stack tops
    s[sp++] = nodes+i;
    while(sp)
    {
        auto n = s[--sp];
        if(!n)
        {
            --tp;
            continue;
        }
        else if(n->visited)
            continue;
        t[tp++] = n;
        depth[n->id] = tp - 1;
        n->visited = true;

        s[sp++] = 0; // Denotes that we have searched all edges adjacent to the node on top of t
        for(auto& v : n->adj)
        {
            if(!v->visited)
                s[sp++] = v;
            else if(depth[n->id] > depth[v->id] + 1)
                ans.push_back(makePath(n, v));
        }
    }
}

int main()
{
    int N, M;
    std::vector<std::vector<int>> ans;
    ans.reserve(200);
    scanf("%d %d", &N, &M);
    for(int i = 1; i <= N; i++)
        nodes[i].id = i;
    for(int i = 1; i <= M; i++)
    {
        int q, p;
        scanf("%d %d", &q, &p);
        if(q == p)
            ans.push_back({q});
        else
        {
            nodes[q].adj.push_back(nodes+p);
            nodes[p].adj.push_back(nodes+q);
        }
    }
    for(int i = 1; i <= N; i++)
        dfs(i, ans);
    printf("%d", ans.size());
    for(auto& a : ans)
    {
        printf("\n%d ", a.size());
        for(auto& e : a)
            printf("%d ", e);
    }
    return 0;
}