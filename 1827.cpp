/* 1827. Indigenous Wars - http://acm.timus.ru/problem.aspx?num=1827
 *
 * Strategy:
 * Insert the given conflicts into a hash set, and for each day and duration, check if there is a
 * conflict that matches, and if so, mark that span of days as containing a conflict.
 * 
 * Performance:
 * O(nd), runs the tests in 0.312s using 7,828KB memory.
 */

#include <stdio.h>
#include <unordered_set>

using ull = unsigned long long;

struct conflict {
    ull x, y;
    char d;
    ull rol(ull val) const { return (val << 10) | (val >> 54); } // Rotate left
    ull ror(ull val) const { return (val << 33) | (val >> 31); } // Rotate right
    auto operator()(const conflict& a) const {
        // Not sure if anti-hash tests are used but we feebly try to evade them anyway
        return std::hash<ull>()(rol(a.x + 0xf93489) ^ ror(a.y + 0x83fee32f) ^ a.d);
    }
    bool operator==(conflict i) const { return x == i.x && y == i.y && d == i.d; }
};

std::unordered_set<conflict, conflict> M; // The conflict set
ull A[100001]; // The stars
int B[100001]; // Marked values (using "impulses" that we "integrate", not sure of the terminology)

int main() {

    int n, m, d;
    ull a, x, y;
    scanf("%d", &n);

    for(int i = 1; i <= n; i++)
        scanf("%llu", &A[i]);
    scanf("%d", &m);
    for(int i = 0; i < m; i++) {
        scanf("%llu %llu %d", &x, &y, &d);
        M.insert({ y, x, (char) d });
    }

    for(int i = 1; i <= n; i++) // For each day
        for(char d = 2; d <= 50; d++) // And each duration
            if(i + d - 1 <= n && M.find({ A[i], A[i+d-1], d}) != M.end()) // Conflict corresponds?
                B[i]++, B[i+d]--; // Mark those days as having a conflict

    for(int h = 0, i = 1; i <= n; i++)
        printf("%d", !!(h += B[i]));
}
