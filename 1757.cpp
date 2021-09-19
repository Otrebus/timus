/* 1757. Gold Bars - http://acm.timus.ru/problem.aspx?num=1757
 *
 * Strategy:
 * Split the bars into two sets, one set less fine than p, and one set more fine. Order the sets by
 * level of matching fineness to p and keep adding mass from both stacks to the resulting bar while
 * keeping the fineness of the bar constant.
 *
 * Performance:
 * O(nlog n), runs the tests in 0.015s using 204KB memory.
 */

#include <stdio.h>
#include <algorithm>
#include <vector>
#include <numeric>

using ld = long double;

struct bar {
    int i;
    ld m, p;
} bars[1001];

ld ans[1001];
std::vector<bar> u, v;

int main() {
    int n;
    ld m, p;
    scanf("%d %Lf %Lf", &n, &m, &p);

    for(int i = 0; i < n; i++) {
        ld m, p;
        scanf("%Lf %Lf", &m, &p);
        bars[i] = { i, m, p };
    }
    for(int i = 0; i < n; i++) // Split into finer and less fine bars
        (bars[i].p < p ? u : v).push_back(bars[i]);

    // Bars of more matching fineness come first
    auto f = [&p] (auto a, auto b) { return std::abs(a.p-p) < std::abs(b.p-p); };
    std::sort(v.begin(), v.end(), f);
    std::sort(u.begin(), u.end(), f);
    u.push_back( { n, -1e-15, -1e-15 } );

    int i = 0, j = 0;
    ld M = m; // How much mass we need to add to the resulting bar

    while(M > 1e-7 && i < u.size() && j < v.size()) { // Go through the bars, one from each set
        auto m = 0.0l, m2 = 0.0l;

        // Determine how much mass we need and can add from both current bars while preserving p
        m = M*(p-v[j].p)/(u[i].p-v[j].p); // How much we need from bar 1 (m1), ideally
        m = std::min(u[i].m, m); // Adjust for how much we actually have in bar 1
        m2 = v[j].p == p ? M : (p*m-m*u[i].p)/(v[j].p-p); // How much m2 needs to be to match
        m2 = std::min(v[j].m, m2); // Might be too much, so adjust
        m = (p*m2-m2*v[j].p)/(u[i].p-p); // After m2 is adjusted, adjust m1 to preserve p

        if(!m && !m2) // No more was needed or possible
            break;

        M -= m+m2;

        // Remove mass from both bars
        u[i].m -= m;
        v[j].m -= m2;

        // Record what we removed
        ans[u[i].i] += m;
        ans[v[j].i] += m2;

        // If any bar ran out of mass, go on to the next one
        if(!v[j].m)
            j++;
        if(!u[i].m)
            i++;
    }
    if(M < 1e-7) {
        printf("YES\n");
        for(int i = 0; i < n; i++)
            printf("%.10Lf\n", ans[i]);
    }
    else printf("NO");
}
