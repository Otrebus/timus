/* 1154. Mages Contest - http://acm.timus.ru/problem.aspx?num=1154
 *
 * Strategy:
 * The function to be maximized is piecewise linear so we only need to evaluate it at the
 * inflection points, and at zero (since if the leftmost piece is constant we need the first
 * possible instant).
 *
 * Performance:
 * Constant complexity, runs the tests in 0.015s using 156KB memory.
 */

#include <stdio.h>
#include <algorithm>
#include <vector>

char A[256]; // Maps elements to indices

using ll = long long;
using ld = long double;
const ll day = 24*60*60;

ll T[4][2], V[4][2]; // The two times and powers of the elements
ld L[4], D[4]; // The number of mages of the light and dark of each element
char s[1001];

ld eval(int i, ld tm) {
    // Evaluates the power of element i at time tm
    ll *v = V[i], *t = T[i];

    ll i1 = T[i][0] < T[i][1] ? 0 : 1;
    ll i2 = T[i][0] > T[i][1] ? 0 : 1;

    ld dy = v[i2]-v[i1], dx = t[i2]-t[i1];
    if(tm > t[i2])
        return v[i2] + (tm-t[i2])*(-dy)/(day-dx);
    else if (tm < t[i1])
        return v[i2] + (day-dx-(t[i1]-tm))*(-dy)/(day-dx);
    return v[i1] + (tm-t[i1])*dy/dx;
}

int main() {
    int i = 0;
    for(auto c : "AEFW") // Map elements to indices
        if(c)
            A[c] = i++;

    std::vector<int> ts = { 0 }; // The times

    for(int i = 0; i < 4; i++) {
        char c[2];
        int h1, m1, s1, p1, h2, m2, s2, p2;
        scanf("%s %d:%d:%d %d %d:%d:%d %d", &c, &h1, &m1, &s1, &p1, &h2, &m2, &s2, &p2);
        int j = A[c[0]];
        T[j][0] = 60*(60*h1 + m1) + s1;
        T[j][1] = 60*(60*h2 + m2) + s2;
        ts.push_back(T[j][0]);
        ts.push_back(T[j][1]);
        V[j][0] = p1;
        V[j][1] = p2;
    }

    scanf("%s", s);
    for(int i = 0; s[i]; i++)
        L[A[s[i]]]++;
    scanf("%s", s);
    for(int i = 0; s[i]; i++)
        D[A[s[i]]]++;

    std::sort(ts.begin(), ts.end());

    int maxt = 0;
    ld max = -10e100;
    for(auto t : ts) {
        ld res = 0;
        for(int i = 0; i < 4; i++)
            res += L[i]*eval(i, t) - D[i]*eval(i, t); // Evaluate outcome of the battle (function)
        if(res > max + 1e-9) {
            maxt = t;
            max = res;
        }
    }

    if(max > 1e-9) {
        printf("%02d:%02d:%02d\n", maxt/(3600), (maxt%3600)/60, maxt%60);
        printf("%.2Lf\n", max);
    } else {
        printf("We can't win!\n");
    }
}
