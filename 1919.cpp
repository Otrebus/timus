/* 1919. Titan Ruins: Transformation of Cylinders - http://acm.timus.ru/problem.aspx?num=1919
 *
 * Strategy:
 * We cut a strip of height h (or w, renamed h) from around a cylinder of height H and circumference
 * W. One revolution of such a strip will span a height d of the cylinder height. If we unwrap the
 * cylinder and draw it we find similar triangles d/h = (W-sqrt(h^2 + d^2))/w, and w^w + h^w = W^2,
 * where w is the length of the strip for each span. This solves to d = h*sqrt(W^2-h^2)/W, and we
 * can figure out how many turns around the cylinder the strip spans, along with how long strip
 * the remainder of the cylinder height can yield.
 * 
 * Performance:
 * O(1), runs the tests in 0.015 seconds using 164KB memory.
 */

#include <stdio.h>
#include <cmath>

using ld = long double;
ld w, h, W, H;

bool solve(ld h, ld w) {
    if(w <= W && h <= H) // We can cover the hole with the cylinder without wrapping
        return true;
    ld d = h*std::sqrt(W*W-h*h)/W; // Height of a span
    auto n = std::trunc(H/d)-1;    // Number of spans
    if(n < 0)
        return false;
    auto x = H-(n+1)*d; // The height of the remaining part
    auto R = x*h/std::sqrt(h*h-d*d); // The length of the remaining strip
    auto l = n*W*d/h; // The length of the main strip
    return R + l >= w;
}

int main() {
    int n;
    scanf("%lf %lf %d", &h, &w, &n);
    while(n--) {
        scanf("%lf %lf", &H, &W);
        if(solve(h, w) || solve(w, h)) // Also try exchanging h and w when solving
            printf("Block the hole\n");
        else
            printf("Too small\n");
    }
}
