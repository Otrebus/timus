/* 1229. Strong Brickwork - http://acm.timus.ru/problem.aspx?num=1229
 *
 * Strategy:
 * Translate the area into a checkered field of graph vertices (like a chess board), so that the
 * graph is bipartite (divided into white and black vertices). Add an edge at every valid placement
 * of a brick and calculate the maximum bipartite matching of the graph.
 * 
 * Performance:
 * O((NM)^1.5), since the Hopcroft-Karp algorithm runs in O(E sqrt(V)) and the number of edges and
 * vertices are both proportional to NM. The solution runs in 0.062s and uses 1544KB.
 */

#include <algorithm>
#include <iostream>
#include <queue>
#include <cctype>

const int INF = 100000000;

bool HopcroftKarpBfs(int n, int m, const std::vector<int>* E, std::vector<int>& D, 
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

bool HopcroftKarpDfs(int n, int m, const std::vector<int>* E, 
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
    std::vector<int>* E = new std::vector<int>[n+m];
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

int V[100][100];
int ans[100][100];
int input[100][100];
std::pair<int, int> P[100*100+1];

int main()
{
    int N, M;
    scanf("%d %d", &N, &M);
    std::vector<std::pair<int, int> > ps;
    int nu = (N*M)/2 + (N*M)%2;
    int nv = (N*M)/2;
    int u = 0;
    int v = 0;
    for(int i = 0; i < N; i++)
    {
        for(int j = 0; j < M; j++)
        {
            if((i%2 + j%2)%2 == 0)
            {
                V[i][j] = nv + ++u;
                P[nv + u] = std::make_pair(i, j);
            }
            else
            {
                V[i][j] = ++v;
                P[v] = std::make_pair(i, j);
            }
        }
    }
    
    for(int i = 0; i < N; i++)
        for(int j = 0; j < M; j++)
            scanf("%d", &input[i][j]);
    for(int i = 0; i < N; i++)
    {
        for(int j = 0; j < M; j++)
        {
            if(i-1 >= 0 && input[i][j] != input[i-1][j] && V[i][j] < V[i-1][j])
                ps.push_back(std::make_pair(V[i][j], V[i-1][j]));
            if(j+1 < M && input[i][j] != input[i][j+1] && V[i][j] < V[i][j+1])
                ps.push_back(std::make_pair(V[i][j], V[i][j+1]));
            if(i+1 < N && input[i][j] != input[i+1][j] && V[i][j] < V[i+1][j])
                ps.push_back(std::make_pair(V[i][j], V[i+1][j]));
            if(j-1 >= 0 && input[i][j] != input[i][j-1]  && V[i][j] < V[i][j-1])
                ps.push_back(std::make_pair(V[i][j], V[i][j-1]));
        }
    }
    ps = HopcroftKarp(nu, nv, ps);
    int b = 1;
    for(int i = 0; i < ps.size(); i++, b++)
        ans[P[ps[i].first].first][P[ps[i].first].second] =
        ans[P[ps[i].second].first][P[ps[i].second].second] = b;
    if(ps.size()*2 != M*N)
        printf("-1\n");
    else
    {
        for(int i = 0; i < N; i++)
        {
            for(int j = 0; j < M; j++)
                printf("%d ", ans[i][j]);
            printf("\n");
        }
    }
    return 0;
}