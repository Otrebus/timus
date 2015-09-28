/* 1115. Ships - http://acm.timus.ru/problem.aspx?num=1115
 *
 * Strategy:
 * Sorting the ships from largest to smallest and the rows from smallest to largest, followed by
 * brute force by dfs and pruning whenever there's not enough space to accomodate the remaining
 * ships. This problem is NP-complete and exactly the same as one of the hardest problems except
 * for the test set, so it is impossible to know whether a solution is fast enough or not prior
 * to submission, which made this by far the worst problem of all the Timus problems I have worked
 * on so far.
 * 
 * Performance:
 * Nominally exponential, but runs in 0.001s because of an intentionally weak test set.
 */

#include <iostream>
#include <algorithm>
#include <vector>
#include <functional>
#include <numeric>

int nships[10];
int rowback[10];
int rows[10];
std::vector<std::pair<int, int> > lengths;
std::vector<int> ships;
int p[101];
int used[101];
int nused;
int spaceleft, shipsleft;

int N, M, x;

bool dfs(int i, int j)
{
    if(spaceleft < shipsleft)
        return false;
    if(i < N && N - nused < M - j)
    {
        int tr = rows[j];
        spaceleft -= tr;
        bool result = dfs(p[j+1], j+1);
        spaceleft += tr;
        return result;
    }
    if(i < N && j == M - 1)
    {
        int sum = 0, nu = 0;
        for(int k = i; k < N; k++)
            if(!used[k])
                nu++, sum += ships[k];
        if(nu + nused == N && rows[j] >= sum)
        {
            for(int k = i; k < N; k++)
                if(!used[k])
                    used[k] = j+1;
            return true;
        }
        return false;
    }
    else if(i == N)
    {
        int tr = rows[j];
        spaceleft -= tr;
        bool result = dfs(p[j+1], j+1);
        spaceleft += tr;
        return result;
    }
    for(int k = i; k < N; k++)
    {
        if(rows[j] >= ships[k] && !used[k])
        {
            nused++;
            used[k] = j+1;
            rows[j] -= ships[k];
            spaceleft -= ships[k];
            shipsleft -= ships[k];
            if(dfs(k+1, j))
                return true;
            nused--;
            used[k] = 0;
            rows[j] += ships[k];
            spaceleft += ships[k];
            shipsleft += ships[k];
        }
    }
    int tr = rows[j];
    spaceleft -= tr;
    bool result = dfs(p[j+1], j+1);
    spaceleft += tr;
    return result;
}

int main()
{
    std::ios::sync_with_stdio(false);
    std::cin >> N >> M;
    std::vector<std::vector<int> > ans(M);
    std::vector<int> input(N);
    for(int i = 0; i < N; i++)
    {
        std::cin >> x;
        shipsleft += x;
        ships.push_back(x);
        input[i] = x;
    }
    for(int i = 0; i < M; i++)
    {
        std::cin >> x;
        spaceleft += x;
        lengths.push_back({ x, i });
    }
    std::sort(ships.begin(), ships.end(), std::greater<int>());
    std::sort(lengths.begin(), lengths.end());
    for(int i = 0; i < M; i++)
    {
        rows[i] = lengths[i].first;
        rowback[i] = lengths[i].second;
    }

    for(int j = 0; j < M; j++)
        for(int i = N-1; i >= 0; i--)
            if(ships[i] <= lengths[j].first)
                p[j] = i;
    dfs(p[0], 0);
    for(int i = 0; i < N; i++)
        ans[rowback[used[i]-1]].push_back(ships[i]);
    for(int i = 0; i < M; i++)
    {
        std::cout << ans[i].size() << std::endl;
        for(int j = 0; j < ans[i].size(); j++)
            std::cout << ans[i][j] << " ";
        std::cout << std::endl;
    }
    return 0;
}