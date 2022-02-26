/* 2108. Oleg and Little Ponies - http://acm.timus.ru/problem.aspx?num=2108
 *
 * Strategy:
 * Check all trigger patterns against the currently possessed set. if any pattern matches, then
 * expand the set with its paired desired pattern and remove the pair, otherwise stop.
 *
 * Performance:
 * O(mn^2), Runs the tests in 0.14s using 1,460KB memory.
 */

#include <iostream>
#include <bitset>
#include <numeric>
#include <list>

std::bitset<1000> A;
std::bitset<1000> C[4000][2];
int n, m, i, j;

int main() {
    std::ios::sync_with_stdio(false), std::cin.tie(nullptr);

    std::cin >> n >> m;
    for(i = 0; i < m; i++)
        std::cin >> C[i][0] >> C[i][1];
    std::cin >> A;

    std::list<int> l(m);
    std::iota(l.begin(), l.end(), 0);

    for(bool b = true; b; ) {
        b = false;
        for(auto it = l.begin(); it != l.end();) {
            if((A & C[*it][0]) == C[*it][0]) {
                A |= C[*it][1];
                it = l.erase(it);
                b = true;
            } else it++;
        }
    };

    for(int i = n-1; i >= 0; i--)
        std::cout << A[i];
}
