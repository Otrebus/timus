/* 1323. Classmates - http://acm.timus.ru/problem.aspx?num=1323
 *
 * Strategy:
 * Brute-force, where we memoize the optimal subsolution for each subset of classmates that have
 * already received the message. From each such subset, we generate every valid combination of
 * calls through depth-first-search to update the optimal answer for each new subset.
 *
 * Performance:
 * Exponential in N, runs the tests in 0.062s using 372KB memory.
 */

#include <functional>
#include <iostream>
#include <vector>
#include <string>
#include <map>

struct Node
{
    std::string name;
    std::vector<Node*> v;
    bool visited;
} nodes[10];

int n;
// For the subset given by the bitmask, the corresponding element of this array contains the minimum
// time to reach the remainder of the pupils, together with the list of calls made next minute on
// the path to that solution
std::pair<int, std::vector<std::pair<int, int>>> ans[1 << 10];

int dfs(int A)
{
    if(ans[A].first >= 0)
        return ans[A].first;
    std::vector<std::pair<int, int>> v;
    std::vector<std::pair<int, int>> minv;
    int min = 10000;

    // Used to generate all possible outgoing sets (each described by mask) of calls this minute
    // from the subset of pupils that got the message described by A. The bit of the current pupil
    // to either make or not make a is given by p (this is 'iterated' over to produce all possible
    // call orders); v contains the sequence of calls made so far by the previous p's
    std::function<void(int, int, std::vector<std::pair<int, int>>& v)> f = 
        [&A, &min, &minv, &f] (int mask, int p, std::vector<std::pair<int, int>>& v)
    {
        if(p == n) // End of recursion, this is now a complete subset
        {
            if(mask) // We have to have made at least one call
            {
                int r = 1 + dfs(A | mask);
                if(r < min)
                    min = r, minv = v;
            }
            return;
        }
        f(mask, p + 1, v); // This pupil does not make a call
        for(int i = 0; i < nodes[p].v.size(); i++)
        {   // Try each possible person to call from this pupil
            if((A & (1 << p)) == 0) // This pupil didn't get the memo yet
                continue;
            auto j = nodes[p].v[i] - nodes;
            if(!(mask & (1 << j)) && !(A & (1 << j))) // Receiver must be ignorant of the message
            {
                v.push_back( { p, j } );
                f(mask | (1 << j), p + 1, v);
                v.pop_back();
            }
        }
    };
    f(0, 0, v);
    ans[A] = { min, minv };
    return min;
}

int main()
{
    std::map<std::string, Node*> nameMap;
    std::vector<Node*> A, B;
    int m, t = 0;
    std::string a, b;
    std::cin >> n >> m;
    // Initialize the answer array
    for(int i = 0; i < 1 << n; i++)
        ans[i] = { -1, {} };
    ans[(1 << n) - 1] = {0, {}};

    // Read and map the input from strings to numbers
    auto add = [&nameMap, &t] (std::string s) { if(!nameMap[s]) (nameMap[s] = &nodes[t++])->name = s; };
    for(int i = 0; i < m; i++)
    {
        std::cin >> a >> b;
        for(auto s : { a, b } )
            add(s);
        nameMap[a]->v.push_back(nameMap[b]);
        nameMap[b]->v.push_back(nameMap[a]);
    }
    std::cin >> a;
    add(a);
    nameMap[a]->visited = true;
    int mask = 1 << (nameMap[a] - nodes);
    dfs(mask); // Calculate the answer
    std::vector<std::vector<std::pair<int, int>>> v;

    // Unwind the optimal answer into something we can print out
    while(mask != (1 << n) - 1)
    {
        v.push_back({});
        auto& w = ans[mask].second;
        for(int i = 0; i < w.size(); i++)
        {
            v.back().push_back( { w[i].first, w[i].second } );
            mask |= (1 << w[i].second);
        }
    }
    std::cout << v.size() << "\n";
    for(int i = 0; i < v.size(); i++)
    {
        std::cout << v[i].size() << "\n";
        for(int j = 0; j < v[i].size(); j++)
            std::cout << nodes[v[i][j].first].name << " " << nodes[v[i][j].second].name << "\n";
    }
}