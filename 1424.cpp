/* 1424. Minibus - http://acm.timus.ru/problem.aspx?num=1424
 *
 * Strategy:
 * Sort the passengers by the bus stop they get on, and iterate that sorted list while maintaining
 * a set of all passengers currently on the bus. Whenever a new passenger gets on and the bus is not
 * full, just add them to the passenger set, but if the set is full, boot the passenger with the
 * greatest bus stop number (and pretend they never got on the bus at all). Whenever the end bus
 * stop of a passenger in the set is reached in the iteration, add that passenger to the answer.
 *
 * Performance:
 * O(k log k), runs the tests in 0.062s using 1,424KB memory.
 */

#include <iostream>
#include <set>
#include <vector>
#include <algorithm>
#include <numeric>

std::pair<int, int> A[50000]; // The passengers' entry and exit stops

int main()
{
    std::ios::sync_with_stdio(false);
    int N, M, K, P;
    std::cin >> N >> M >> K >> P;
    for(int i = 0; i < K; i++)
        std::cin >> A[i].first >> A[i].second;

    // Sort the passengers by entrance stops
    std::vector<int> v(K), ans;
    std::iota(v.begin(), v.end(), 0);
    std::sort(v.begin(), v.end(), [] (int a, int b) { return A[a] < A[b]; });

    // The passenger set sorted by the exit stop so we can quickly boot people
    auto lt = [](int a, int b) { return A[a].second < A[b].second || (A[a].second == A[b].second) &&
                                  (A[a].first < A[b].first || A[a].first == A[b].first && a < b); };
    std::set<int, decltype(lt)> q(lt);

    for(auto i : v)
    {
        // Add all successful journeys to the answer
        while(!q.empty() && A[*(q.begin())].second <= A[i].first)
            ans.push_back(*q.begin()), q.erase(q.begin());
        q.insert(i);
        if(q.size() > M) // Not enough room, boot the most blocking passenger
            q.erase(--q.end());
    }
    ans.insert(ans.end(), q.begin(), q.end()); // Add the remaining passengers at the last entrystop
    std::cout << P*ans.size() << "\n";
    for(auto a : ans)
        std::cout << a + 1 << " ";
}