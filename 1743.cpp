/* 1743. Domino Sorting - http://acm.timus.ru/problem.aspx?num=1743
 *
 * Strategy:
 * Sort all dominoes by their smaller half ascending tie-broken by the greater half descending.
 * Create the final answer from the top and bottom in (top down with a growing sequence by the
 * left number, bottom up with a growing sequence by the right number) by adding each sorted
 * domino to the top sequence if it fits there, otherwise, flipped, to the bottom sequence.
 * If neither work, we fail, or if the sequences can't merge in the middle, we fail.
 *
 * Performance:
 * O(nlog n), runs the test cases in 0.625s using 2632KB memory.
 */

#include <iostream>
#include <algorithm>
#include <vector>

struct pair { int x, y; };

int main() {

    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    std::vector<pair> v;
    int n;
    std::cin >> n;
    while(n--) {
        pair p;
        std::cin >> p.x >> p.y;
        if(p.x > p.y)
            std::swap(p.x, p.y);
        v.push_back(p);
    }

    std::sort(v.begin(), v.end(), [] (auto& a, auto& b) {
        return a.x < b.x || a.x == b.x && a.y > b.y; }
    );

    std::vector<pair> a, b;
    a.push_back(v[0]);

    for(int i = 1; i < v.size(); i++) {
        if(a.back().y < v[i].y) { // If we don't fit into the top sequence
            if(b.empty() || b.back().x >= v[i].y) // Check if we fit into the bottom
                b.push_back({ v[i].y, v[i].x });
            else {
                std::cout << "NO";
                return 0;
            }
        }
        else
            a.push_back(v[i]);
    }

    // Make sure we can merge the sequences
    if(!b.empty() && (b.back().x < a.back().x || b.back().y > a.back().y)) {
        std::cout << "NO";
        return 0;
    }

    std::cout << "YES" << std::endl;
    for(auto p : a)
        std::cout << p.x << " " << p.y << std::endl;
    for(auto it = b.rbegin(); it < b.rend(); it++)
        std::cout << it->x << " " << it->y << std::endl;
}
