/* 1320. Graph Decomposition - http://acm.timus.ru/problem.aspx?num=1320
 *
 * Strategy:
 * If any graph component has an uneven number of edges, print "0", else print "1".
 *
 * This works since a component is pairable if and only if it has an even number of edges; a proof
 * sketch follows (with the component referred to as a "graph"):
 *
 *   If a graph is pairable:
 * Every pair in the graph consists of an even number of edges (2), so the entire graph must
 * also be of even size.
 *
 *   If a graph has an even number of edges:
 * Every simple connected graph can be turned into a tree where each edge e of the graph is a node
 * of the tree, and the incident edges of e are either the children of e, or the children of some
 * other part of the tree. This tree can be constructed via for example depth-first search. If
 * every tree of some even size has pairable edges, then every graph also does, since we can just
 * re-attach the edges to form the original graph. The proof is by strong induction over the size
 * of the tree. An even-sized tree of two nodes is trivially pairable. Assume that every even tree
 * of size <= k is pairable, and that we are given a graph of size k+2. We can form a tree with one
 * of the nodes as root, which has a set of subtrees as children. All the even-sized subtrees are
 * pairable by the induction assumption, and except for these even-sized subtrees there is an uneven
 * number of uneven-sized subtrees attached to the root edge, which are attached onto the two
 * vertices of the root edge, with one vertex having an uneven number of uneven-sized trees and the
 * other an even number of uneven-sized trees. The latter constitutes an even-sized tree and is
 * pairable by the induction assumption. The former can be linked together with the root edge into
 * an even sized tree, which is also pairable. Hence, the entire tree of size k+2 is pairable, and
 * by induction all trees, and hence their original corresponding graphs.
 *
 * Performance:
 * O(E) which is the size of the input and hence optimal. The test cases execute in 0.015s and
 * use 288KB of memory.
 */

#include <stdio.h>
#include <vector>
#include <stack>

int main()
{
    std::vector<std::vector<int>> n(1001); // Edge list
    std::vector<bool> v(1000, false); // Visited?

    int x, y;
    while(scanf("%d %d", &x, &y) != EOF)
    {
        n[x].push_back(y);
        n[y].push_back(x);
    }
    for(int i = 1; i < 1000; i++)
    {
        std::stack<int> s;

        if(v[i])
            continue;
        s.push(i);
        int c = 0; // Number of edges in this component
        while(!s.empty())
        {
            auto i = s.top();
            s.pop();
            v[i] = true;
            for(auto it = n[i].begin(); it < n[i].end(); it++)
            {
                if(v[*it])
                    continue;
                c++;
                s.push(*it);
            }
        }
        if(c % 2)
        {
            printf("0");
            return 0;
        }
    }
    printf("1");
    return 0;
}