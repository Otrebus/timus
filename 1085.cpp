/* 1085. Meeting - http://acm.timus.ru/problem.aspx?num=1085
 *
 * Strategy:
 * Floyd-Warshall. Each tram line makes the traveling length (number of tickets required) between
 * each node connected by that line 1, which we enter into the cost matrix, and we use the
 * aforementioned algorithm to relax the matrix over longer routes. Then we can go through each 
 * node and check the sum of the costs between that node and all friends easily.
 *
 * Performance:
 * O(N^3), runs the test cases in 0.015s using 296KB memory.
 */

#include <iostream>
#include <vector>
#include <algorithm>

const int inf = 0x1FFFFFFF; // Signifies unconnectedness
const int maxn = 100, maxk = 100;
int A[maxn+1][maxn+1]; // Cost matrix
struct Friend
{
    int home, cash;
    bool ticket; // Month ticket
} friends[maxk+1];

int cost(int i, int K) // Calculates the total cost of this meeting place
{
    int cost = 0;
    for(int k = 1; k <= K; k++)
    {
        int h = friends[k].home;
        if(A[h][i] == inf) // No connection to that friend
            return -1;
        else if(friends[k].ticket) // Person has month ticket, costs nothing
            continue;
        else if(A[h][i]*4 > friends[k].cash) // Too little money
            return -1;
        cost += A[h][i]*4;
    }
    return cost;
}

int main()
{
    int N, M, K, L, x;
    std::cin >> N >> M;

    for(int i = 1; i <= N; i++)
        for(int j = 1; j <= N; j++)
            A[i][j] = i == j ? 0 : inf;

    for(int i = 0; i < M; i++)
    {
        std::cin >> L;
        std::vector<int> v(L);
        for(int i = 0; i < L; i++)
        {
            std::cin >> x;
            v[i] = x;
        }
        for(int i = 0; i < L; i++) // All stations in this line reach each other in one step
            for(int j = 0; j < L; j++)
                if(v[i] != v[j])
                    A[v[i]][v[j]] = A[v[j]][v[i]] = 1;
    }
    std::cin >> K;
    for(int i = 1; i <= K; i++)
        std::cin >> friends[i].cash >> friends[i].home >> friends[i].ticket;
    for(int k = 1; k <= N; k++) // Floyd-Warshall
        for(int i = 1; i <= N; i++)
            for(int j = 1; j <= N; j++)
                A[i][j] = A[j][i] = std::min(A[i][j], A[i][k] + A[k][j]);
    int mincost = inf, mins = 0;
    for(int i = 1; i <= N; i++) // Go through each meeting place candidate and choose the minimum
    {
        int c = cost(i, K);
        if(c != -1 && c < mincost)
            mins = i, mincost = c;
    }
    if(mincost == inf)
        std::cout << 0;
    else
        std::cout << mins << " " << mincost;
}