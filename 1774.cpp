/* 1774. Barber of the Army of Mages - http://acm.timus.ru/problem.aspx?num=1774
 *
 * Strategy:
 * Set up a flow network and solve it using the Ford-Fulkerson algorithm using depth-first search
 * to find each augmenting path. The flow network is constructed as follows:
 * Make one node for each magician and one node for each minute of time, and source and sink nodes.
 * Connect the source to each magician by an edge of capacity 2, each magician i with each time
 * node in s_i + t_i - 1 by an edge of capacity 1, and each time node to the sink by an edge of 
 * capacity k.
 *
 * Performance:
 * O((max(t-s)+n)n), which runs the test suite in 0.031s using 6632KB memory.
 */

#include <stdio.h>
#include <vector>
#include <algorithm>

struct node;

struct edge
{
    edge* sibling, *parent; // Parent node of a found augmenting path
    bool residual; // Part of the residual network?
    int flow, cap;
    node* to;
};

struct node
{
    int id = 0, visits = 0;
    std::vector<edge*> edges;
};

node nodes[2102];
edge edges[404202];
edge* edgep = edges;

// Positions in the node list for various things, like time nodes, etc
node* ptime = nodes + 101, *psource = nodes, *psink = nodes + 2101;

int visits = 0;

// Constructs the flow network as described in the header comment
void makeNetwork(int k, std::vector<std::pair<int, int>>& v)
{
    int mint = 10000, maxt = 0;
    for(int i = 0; i < v.size(); i++)
        maxt = std::max(v[i].first + v[i].second - 1, maxt), mint = std::min(v[i].first, mint);

    int totaug = 0;

    for(int i = 1; i <= v.size(); i++) 
    {
        edge* edgea = edgep++;
        edge* edgeb = edgep++;
        psource->edges.push_back(edgea);
        nodes[i].edges.push_back(edgeb);
        *edgea = edge { edgeb, 0, false, 0, 2, nodes+i };
        *edgeb = edge { edgea, 0, true, 0, 2, psource };

        int& t = v[i-1].first, s = t + v[i-1].second;
        for(int j = t; j < s; j++)
        {
            edge* edgea = edgep++;
            edge* edgeb = edgep++;
            *edgea = edge { edgeb, 0, false, 0, 1, ptime + j };
            *edgeb = edge { edgea, 0, true, 0, 1, psource + i };
            nodes[i].edges.push_back(edgea);
            (ptime + j)->edges.push_back(edgeb);
        }
    }
    for(int i = mint; i <= maxt; i++)
    {
        edge* edgea = edgep++;
        edge* edgeb = edgep++;
        *edgea = edge { edgeb, 0, false, 0, k, psink };
        *edgeb = edge { edgea, 0, true, 0, k, ptime + i };
        (ptime + i)->id = i;
        (ptime + i)->edges.push_back(edgea);
        psink->edges.push_back(edgeb);
    }
}

// Does a depth first search from the source to find an augmenting path; the last edge of
// that path is returned along with the augmenting flow size
std::pair<int, edge*> dfs()
{

    std::vector<std::pair<edge*, int>> s; // Stack for the dfs (much faster than std::stack)
    s.reserve(800000);
    const int inf = 100000;

    visits++; // New dfs search, this is instead of going through and unmarking a bunch of bools
    psource->visits = visits;
    for(int i = 0; i < psource->edges.size(); i++) // Push edges from the source
    {
        if(psource->edges[i]->cap > psource->edges[i]->flow)
            s.push_back({psource->edges[i], psource->edges[i]->cap - psource->edges[i]->flow });
    }
    int augcap = 0; // The capacity of the augmenting path found
    while(!s.empty())
    {
        auto e = s.back().first; // Edge leading to this node
        auto maxaug = s.back().second; // The minimally augmentable amount so far so far
        auto n = e->to; // Current node
        n->visits = visits; // Mark visited
        if(n == psink) // We reached the sink, terminate
        {
            augcap = maxaug;
            break;
        }
        s.pop_back();

        // Search outgoing edges from this node for possible augmenting edges
        for(auto it = n->edges.cbegin(); it < n->edges.cend(); it++)
        {
            edge* edg = *it;
            if(edg->to->visits == visits) // Already visited
                continue;
            int mn = edg->residual ? std::min(edg->sibling->flow, maxaug) 
                                   : std::min(edg->cap-edg->flow, maxaug);
            if(!mn) // Augmenting edge must actually be augmenting
                continue;
            s.push_back({ edg, mn });
            edg->parent = e;
        }
    }
    return { augcap, s.empty() ? 0 : s.back().first };
}

// Constructs the solution from the optimized flow network
std::vector<std::pair<int, int>> makeSolution(int n)
{
    std::vector<std::pair<int, int>> ans;
    ans.reserve(n);

    // Search for flows > 0 between magicians and time slots
    for(int i = 1; i <= n; i++)
    {
        std::pair<int, int> a {0, 0};

        for(int j = 0; j < nodes[i].edges.size(); j++)
            if(nodes[i].edges[j]->flow)
                (!a.first ? a.first : a.second) = nodes[i].edges[j]->to->id;
        if(a.first > a.second)
            std::swap(a.first, a.second);
        ans.push_back(a);
    }
    return ans;
}

std::vector<std::pair<int, int>> solve(int k, std::vector<std::pair<int, int>>& v)
{
    makeNetwork(k, v); // Make the corresponding flow network

    int totaug = 0, pathaug;

    do
    {
        auto p = dfs(); // Try to find an augmenting path
        auto lastNode = p.second;
        pathaug = p.first;
        totaug += pathaug;

        for(edge* e = lastNode; e; e = e->parent) // Apply the path to the network
            if(!e->residual)
                e->flow += pathaug;
            else
                e->sibling->flow -= pathaug;
    } while(pathaug);

    // When done finding all augmented paths, the total flow increase compared to the empty network
    // should be equal to two times the amount of magicians; otherwise we couldn't satisfy our
    // customer demand
    if(totaug != v.size()*2)
        return std::vector<std::pair<int, int>>();
    
    return makeSolution(v.size()); // Construct the solution vector from the flow network
}

int main()
{
    int n, k;
    scanf("%d %d", &n, &k);
    std::vector<std::pair<int, int>> v;
    for(int i = 0; i < n; i++)
    {
        int x, y;
        scanf("%d %d", &x, &y);
        v.push_back(std::make_pair(x, y));
    }
    auto ps = solve(k, v);
    if(ps.empty())
        printf("No\n");
    else
    {
        printf("Yes\n");
        for(int i = 0; i < ps.size(); i++)
            printf("%d %d\n", ps[i].first, ps[i].second);
    }
}