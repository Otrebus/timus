/* 2103. Corporate Mail - http://acm.timus.ru/problem.aspx?num=2103
 *
 * Strategy:
 * If we consider the reverse problem of going from the upper number t to the lower number s
 * by means of division or subtractions, we notice that for any span of s consecutive numbers
 * there is some number x in that span divisible by s, so we can divide x by (x/s) to yield s.
 * In this problem we consider s up to 7, so we only need to check 3 employees on each side of
 * any employee in our search which is carried out using our good friend Dijkstra.
 *
 * Performance:
 * The complexity is tricky to derive but it runs the tests in 0.343s using 6,616KB memory.
 */

#include <iostream>
#include <unordered_map>
#include <set>
#include <vector>

struct node {
    node() { v = h = p = 0; c = 100000; }
    bool v, h; // Visited, allowed to visit from sideways
    int c, p; // Cost, predecessor
};

std::unordered_map<int, node> nodes;

void relax(std::set<std::pair<int, int>>& Q, int p, int x, int c, bool b) {
    // Relaxes the edge p, x in Q with cost c; b means we can check it sideways in the future
    if(c >= nodes[x].c || nodes[x].v)
        return;

    Q.erase({ nodes[x].c, x });
    nodes[x].h = b;
    nodes[x].c = c;
    nodes[x].p = p;
    Q.insert({ nodes[x].c, x });
}

int search(int t, int s) { // Dijkstra
    std::set<std::pair<int, int>> Q({{0, s}});
    nodes[s].h = true;
    nodes[s].c = 0;

    while(!Q.empty()) {
        auto p = *Q.begin();
        int i = p.second, c = p.first;
        nodes[i].v = true;

        if(nodes[i].h) // Sideways search if we've not done that already
            for(int x = std::max(i-3, 1); x <= std::max(i-3, 1) + 7; x++)
                if(x != i)
                    relax(Q, i, x, nodes[i].c + std::abs(x-i), false);

        for(int r = 1; r <= std::sqrt(i); r++) { // Downwards
            if(i % r == 0) {
                relax(Q, i, i/r, nodes[i].c, true);
                relax(Q, i, r, nodes[i].c, true);
            }
        }
        Q.erase({nodes[i].c, i});
    }
    return nodes[t].c;
}

int main() {
    int s, t;
    std::cin >> s >> t;

    std::cout << search(s, t) << std::endl;

    std::vector<int> v;
    for(int i = s; i; i = nodes[i].p)
        v.push_back(i);

    std::cout << v.size() << std::endl;
    for(auto n : v)
        std::cout << n << " ";
}
