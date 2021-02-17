/* 1175. Strange Sequence - http://acm.timus.ru/problem.aspx?num=1175
 *
 * Strategy:
 * Find a recurring pair (x_i, x_{i+1}), (x_j, x_{j+1}) of successive numbers using the Tortoise
 * and Hare algorithm, find the period q of the given sequence by advancing it to the next
 * recurring pair, iterate the sequence again and check (x_i, x_{i+1}) and (x_{q+i}, x_{q+i+1})
 * for equality to find the first p = i.
 *
 * Performance:
 * Hard to derive the complexity, but runs the tests in 0.015s using 396KB memory.
 */

#include <iostream>

using ll = long long;
ll A1, A2, A3, A4, B1, B2, C, p, q, xi, xj, xk, yi, yj, yk;

ll F(ll X, ll Y) {
    ll H = A1*X*Y + A2*X + A3*Y + A4;
    if(H > B1)
        H -= C*((H-B2-1)/C) + C;
    return H;
}

void next() {
    xk = F(xi, xj);
    xi = xj;
    xj = xk;
}

void next2() {
    yk = F(yi, yj);
    yi = yj;
    yj = yk;
}

int main() {
    ll x1, x2;
    std::cin >> A1 >> A2 >> A3 >> A4 >> B1 >> B2 >> C >> x1 >> x2;
    ll num1 = -1, num2 = -1;

    xi = x1, xj = x2;
    yi = x1, yj = x2;

    for(int c = 0; num1 < 0; c++) {
        // Tortoise and hare to find the repeating pair
        next();
        next2(), next2();
        if(xi == yi && xj == yj)
            num1 = xi, num2 = xj;
    }

    // Find the next repeating pair
    next();
    for(q = 1; xi != num1 || xj != num2; q++)
        next();

    // Advance the second sequence to offset q
    yi = x1, yj = x2;
    for(int a = 0; a < q; a++)
        next2();
    
    // Advance both sequences in parallel to find index p
    xi = x1, xj = x2;
    for(p = 1; xi != yi || xj != yj; p++)
        next(), next2();

    std::cout << p << " " << q;
}
