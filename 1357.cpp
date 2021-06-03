/* 1357. Teakettle 1.0 for Dummies - http://acm.timus.ru/problem.aspx?num=1357
 *
 * Strategy:
 * Straightforward simulation according to the given rules.
 *
 * Performance:
 * O(N), runs the tests in 0.015s using 132KB memory.
 */

#include <stdio.h>
#include <queue>
#include <cmath>

using ld = long double;
const ld C = 4.19, P = 400;
const int S = 200, T0 = 20;
const ld inf = std::numeric_limits<ld>::infinity();

int main() {
    ld b = inf, T = 20; // Boil time, current temperature
    int N, Q = 0; // Number of students, number waiting for tea
    std::vector<ld> v;
    scanf("%d", &N);
    for(int i = 0; i < N; i++) {
        int h, m, s;
        scanf("%d%*c%d%*c%d", &h, &m, &s);
        v.push_back(h*60*60 + m*60 + s);
    }

    int w = 0, i = 0; // Kettle contents, current student
    ld dt = 0, t = v.size() ? v[0] : 0;
    while(i < N || b < inf) {
        ld t1 = t; // Previous time
        if(i >= N || b < v[i]) { // If the kettle boils before the next riser
            int B = int(b);
            int s = B%60, m = B/60%60, h = B/(60*60)%24;
            for(;w; w -= 200, Q--) // While kettle is not empty, fill cup
                printf("%02d:%02d:%02d\n", h, m, s); 
            t = b, b = inf, T = T0;
        } else { // Student woke up, add to queue
            t = v[i++];
            Q++;
        }
        dt = t - t1;
        if(w) // Increase temperature of the kettle
            T += P*dt/(C*w);
        while(Q*200 - w > 0 && w < 1000) {
            // If there are people in the queue ready to add water and there's room in the kettle
            T = (w*T + S*T0)/(w + S); // New temperature
            w += S; // New kettle volume
            b = t + (100 - T)*(C*w)/P; // Recalculate boiling time
        }
    }
}
