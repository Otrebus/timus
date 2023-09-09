/* 1721. Two Sides of the Same Coin - http://acm.timus.ru/problem.aspx?num=1721
 *
 * Strategy:
 * Assign the problem setters to two groups where the left group contains the setters with ids
 * 1,2,5,6,9,10, etc, and the right group the rest of the ids. This way, no setter can ever be
 * paired up with another setter in its own group. If we consider each setter as a node, and
 * each possible pairing as an edge, we get a bipartite graph which we solve with Hopcroft-Karp.
 *
 * Performance:
 * O(N^2.5), runs the test cases in 0.031s using 5508KB memory.
 */

#include <algorithm>
#include <vector>
#include <queue>
#include <string>
#include <iostream>

const int INF = 100000000;

bool HopcroftKarpBfs(int n, int m, const std::vector<std::vector<int>>& E, std::vector<int>& D, 
                     std::vector<int>& M)
{
    std::queue<int> Q;
    bool added = false;

    for(int i = 0; i < n; i++)
    {
        if(M[i] == -1)
        {
            D[i] = 0;
            Q.push(i);
        }
        else
            D[i] = INF;
    }
    for(int i = n; i < m+n; i++)
        D[i] = INF;

    int max = INF;
    while(!Q.empty())
    {
        int u = Q.front();
        Q.pop();
        for(auto it = E[u].begin(); it != E[u].end(); it++)
        {
            int v = *it;
            if(D[v] > D[u] + 1 && (u < n && M[u] != v || u >= n && M[v] == u))
            {
                if(v >= n && M[v] == -1)
                {
                    max = D[u]+1;
                    added = true;
                }
                D[v] = D[u] + 1;
                if(D[u]+1<=max)
                    Q.push(v);
            }
        }
    }
    return added;
}

bool HopcroftKarpDfs(int n, int m, const std::vector<std::vector<int>>& E, 
                     std::vector<int>& D, std::vector<int>& M, int r, int parent)
{
    if(D[r] == 0)
    {
        D[r] = INF;
        return true;
    }
    for(auto it = E[r].begin(); it < E[r].end(); it++)
    {
        int s = *it;
        if((r < n && M[r] == s || r >= n && M[r] != s) && (D[s] == D[r] - 1 
            && HopcroftKarpDfs(n, m, E, D, M, s, r)))
        {
            if(r >= n)
            {
                M[r] = s;
                M[s] = r;
            }
            else
            {
                M[r] = parent;
                M[parent] = r;
            }
            D[r] = INF;
            return true;
        }
    }
    return false;
}

std::vector<std::pair<int, int>> HopcroftKarp(const int n, const int m, 
                                              std::vector<std::pair<int, int>> e)
{
    std::vector<std::vector<int>> E(n+m);
    std::vector<int> D(n+m);
    std::vector<int> M(n+m);
    std::fill(M.begin(), M.end(), -1);

    for(auto it = e.begin(); it < e.end(); it++)
    {
        E[it->first-1].push_back(it->second-1);
        E[it->second-1].push_back(it->first-1);
    }

    while(HopcroftKarpBfs(n, m, E, D, M))
    {
        std::vector<int> r;
        for(int i = n; i < n+m; i++)
            r.push_back(i);
        std::sort(r.begin(), r.end(), [&D] (int x, int y) { return D[x] < D[y]; });
        for(int i = 0; i < m; i++)
            if(M[r[i]] == -1)
                HopcroftKarpDfs(n, m, E, D, M, r[i], -1);
    }
    std::vector<std::pair<int, int> > ans;
    for(int i = 0; i < n; i++)
        if(M[i] != -1)
            ans.push_back(std::make_pair(i+1, M[i]+1));
    return ans;
}

struct entry { std::string name; char spec; int rank; };

int main()
{
    std::vector<entry> l, r;
    int n;

    std::cin >> n;
    for(int i = 0; i < n; i++)
    {
        entry e;
        std::string spec;
        std::cin >> e.name;
        std::cin >> spec;
        e.spec = spec[0];
        std::cin >> e.rank;
        (e.rank % 4 < 2 ? l : r).push_back(e);
    }
    std::vector<std::pair<int, int>> in;
    for(int i = 0; i < l.size(); i++)
        for(int j = 0; j < r.size(); j++)
            if(std::abs(l[i].rank-r[j].rank) == 2 && (l[i].spec == r[j].spec && l[i].spec == 'a' || l[i].spec != r[j].spec))
                in.push_back( { i+1, l.size()+j+1 } );
    auto p = HopcroftKarp(l.size(), r.size(), in);
    std::cout << p.size() << "\n";
    for(int i = 0; i < p.size(); i++)
    {
        auto a = l[p[i].first-1];
        auto b = r[p[i].second-1-l.size()];
        if(a.spec == 't' || b.spec == 's')
            std::swap(a, b);
        std::cout << a.name << " " << b.name << "\n";
    }
    return 0;
}