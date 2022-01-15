/* 1046. Geometrical Dreams - http://acm.timus.ru/problem.aspx?num=1046
 *
 * Strategy:
 * Express the problem in complex coordinates. We are then looking for a solution to the equation
 * 
 *   (((p - M_1)*R_1 + M_1) - M_2)*R_2 + M_2 ... = p,
 * 
 * Where the R_i's are the unit rotation vectors cos(α_i) + i*sin(α_i), and we express p as x + yi.
 * Iterate the above expression over all M's by collecting the terms that involve p and the constant
 * separately, and solve the resulting equation using Cramer's rule. Once we have p, we can
 * construct the resulting polygon.
 *
 * Performance:
 * O(N), runs the tests in 0.031s using 184KB memory.
 */

#include <stdio.h>
#include <cmath>

using ld = long double;
ld pi = std::acos(-1);

struct complex {
    ld a, b;
    complex operator- (const complex& v) { return { a-v.a, b-v.b }; }
    complex operator+ (const complex& v) { return { a+v.a, b+v.b }; }
    complex operator* (const complex& v) { return { a*v.a-b*v.b, a*v.b + v.a*b }; }
    complex rot(ld ang) { return *this*complex { std::cos(ang), std::sin(ang) }; }
};

int N;
complex M[50], v[50];
ld A[50];

int main() {
    scanf("%d", &N);
    for(int i = 0; i < N; i++)
        scanf("%lf %lf", &M[i].a, &M[i].b);

    for(int i = 0; i < N; i++)
        scanf("%lf", &A[i]), A[i] *= (pi/180); // Turn degrees into rad

    complex x = { 1, 0 }, y = { 0, 1 }, c = { 0, 0 };
    for(int i = 0; i < N; i++) { // Iterate over the expression, collecting terms
        c = c - M[i];
        x = x.rot(A[i]), y = y.rot(A[i]), c = c.rot(A[i]);
        c = c + M[i];
    }

    // Solve the equation using Cramer's rule
    ld det = (x.a-1)*(y.b-1) - x.b*y.a;
    v[0] = { (c.b*y.a - c.a*(y.b-1))/det, (x.b*c.a - (x.a-1)*c.b)/det };

    // Construct the polygon and output
    for(int i = 0; i < N-1; i++)
        v[i+1] = (v[i]-M[i]).rot(A[i]) + M[i];
    for(int i = 0; i < N; i++)
        printf("%.10lf %.10lf\n", v[i].a, v[i].b);
}
