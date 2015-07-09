/* 1029. Ministry - http://acm.timus.ru/problem.aspx?num=1029
 *
 * Strategy:
 * Dijkstra.
 *
 * Performance:
 * O(MN log(MN)). This is not optimal since there is an O(MN) solution using simple
 * dynamic programming.
 */

#include <limits>
#include <cstdio>
#include <vector>
#include <set>

struct room
{
    int fee;  // Fee of the official in this room
    int cost; // The lowest cost (so far) during the Dijkstra search
    std::pair<int, int> parent; // Previous (or next) room of the solution
    bool visited;
};

int main()
{
    int N, M;

    scanf("%d %d", &M, &N);

    std::vector<std::vector<room>> A(M);

    class Func // Function object for the set ordering
    {
        std::vector<std::vector<room>>* _a;
    public:
        Func(std::vector<std::vector<room>>* a) : _a(a)
        {
        }

        bool operator() (const std::pair<int, int>& a, const std::pair<int, int>& b)
        {   // Order according to room cost first, indices second
            if(((*_a)[a.first][a.second]).cost == ((*_a)[b.first][b.second]).cost)
                return a < b;
            else
                return ((*_a)[a.first][a.second]).cost < ((*_a)[b.first][b.second]).cost;
        }

    };

    std::set<std::pair<int, int>, Func> m((Func(&A))); // Set used as the priority queue
                                                     // in Dijkstra's algoritm
    for(int i = 0; i < M; i++)
    {
        A[i].resize(N);
        for(int j = 0; j < N; j++)
        {
            int x;
            scanf("%d", &x);
            A[i][j].fee = x;
            A[i][j].cost = std::numeric_limits<int>::max();
            A[i][j].visited = false;
        }
    }
    for(int j = 0; j < N; j++)
    {   // We start from the top floor (see the final comment in this file)
        A[M-1][j].cost = A[M-1][j].fee;
        m.insert(std::make_pair(M-1, j));
    }
    std::pair<int, int> ans;
    while(!m.empty())
    {
        auto it = m.begin(); // Greedily pick the cheapest room
        int i = it->first;
        int j = it->second;
        A[i][j].visited = true;

        if(i == 0) // We reached the bottom floor
        {
            ans = std::make_pair(i, j);
            break;
        }
        // Iterate over rooms up, left and right
        std::vector<std::pair<int, int>> dirs = { {i-1,j}, {i, j+1}, {i,j-1} };
        for(auto p : dirs)
        {
            auto i2 = p.first, j2 = p.second;
            if(i2 >= 0 && j2 >= 0 && j2 < N)
            {
                // Relaxation step
                if(A[i2][j2].cost > A[i2][j2].fee + A[i][j].cost)
                {
                    // Possibly replace this room if it already was in the priority queue
                    m.erase(std::make_pair(i2, j2));
                    A[i2][j2].cost = A[i2][j2].fee + A[i][j].cost;
                    m.insert(std::make_pair(i2, j2));
                    // Solution parent
                    A[i2][j2].parent = std::make_pair(i, j);
                }
            }
        }
        m.erase(it);
    }
    do // Print out the solution - we ran the algorithm from the top floor to the bottom
    {  // so this prints it out "backwards", now in the right direction
        printf("%d ", ans.second+1);
        ans = A[ans.first][ans.second].parent;
    } while(ans.first != 0);
}
