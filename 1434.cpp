/* 1434. Buses in Vasyuki - http://acm.timus.ru/problem.aspx?num=1434
 *
 * Strategy:
 * For each bus stop, store the bus lines it's part of, and for each bus line, store the bus
 * stops reachable from it. Then bfs by, from the stop in the front of the queue, adding to
 * the queue all the stops reachable from the lines going through said stop (and repeat).
 *
 * Performance:
 * O(NM), runs the tests in 0.203s using 7,408KB memory.
 */

#include <iostream>
#include <queue>

std::vector<int> S[1001];

struct {
    bool v; // Visited?
    std::vector<int> s; // Lines going through this stop
    int p; // Predecessor in bfs
} nodes[100001];

int main() {
    int N, M, k, x, y, a, b;
    std::cin >> N >> M;

    for(int i = 1; i <= N; i++) {
        std::cin >> k;
        for(int j = 1; j <= k; j++) {
            int x;
            std::cin >> x;
            nodes[x].s.push_back(i); // Add this line to the lines going through the stop
            S[i].push_back(x); // Add the stop to the line
        }
    }
    std::cin >> a >> b;

    std::queue<int> Q({a});
    nodes[a].v = true;

    while(!Q.empty()) {
        auto j = Q.front();
        Q.pop();
        for(auto s : nodes[j].s) { // For all the lines through the stop
            for(auto x : S[s]) { // For all the stops in those lines
                if(!nodes[x].v) { // If the stop not already added, add to Q
                    Q.push(x);
                    nodes[x].p = j; // Set previous front of the queue as predecessor
                    nodes[x].v = true;
                }
            }

            S[s].clear(); // No need to visit these lines again
        }
    }

    if(!nodes[b].v) {
        std::cout << -1;
        return 0;
    }

    std::vector<int> out;
    for(auto p = b; p; p = nodes[p].p) // Run up the predecessors from the target and form the ans
        out.push_back(p);

    std::cout << (out.size() - 1) << std::endl; 
    for(auto i = out.rbegin(); i < out.rend(); i++)
        std::cout << *i << " ";
}
