/* 1815. Farm in San Andreas - http://acm.timus.ru/problem.aspx?num=1815
 *
 * Strategy:
 * Weiszfeld's algorithm.
 *
 * Performance:
 * O(t), runs the tests in 0.39s using 184KB memory.
 */

#include <stdio.h>
#include <cmath>

using ld = long double;

struct vec {
    ld x, y;
    vec operator- (const vec& v) const { return { x-v.x, y-v.y }; }
    vec operator+ (const vec& v) const { return { v.x+x, v.y+y }; }
    vec operator+= (const vec& v) { return *this = { v.x+x, v.y+y }; }
    vec operator/ (ld f) const { return { x/f, y/f }; }
    vec operator* (ld f) const { return { x*f, y*f }; }
    ld d() { return std::sqrt(x*x+y*y); }
} v[3];
ld c[3];

vec solve() {
    vec m = (v[0] + v[1] + v[2])/3;
    
    while (true) {
        vec A = { 0, 0 };
        ld B = 0;

        for (int i = 0; i < 3; ++i) {
            if((v[i]-m).d() < 1e-9) {
                A = v[i], B = 1;
                break;
            } else {
                A += v[i]*c[i]/((v[i]-m).d());
                B += c[i]/((v[i]-m).d());
            }
        }
        if((m-A/B).d() < 1e-12)
            break;
        m = A/B;
    }
    return m;
}

int main() {
    int t;
    scanf("%d", &t);
    while (t--) {
        for (int i = 0; i < 3; ++i)
            scanf("%lf %lf", &v[i].x, &v[i].y);
        for (int i = 0; i < 3; ++i)
            scanf("%lf", &c[i]);

        vec m = solve();
        ld ans = 0;
        for (int i = 0; i < 3; ++i)
            ans += (m-v[i]).d()*c[i];
        printf("%.20lf\n", ans);
    }
}
