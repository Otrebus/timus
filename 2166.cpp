/* 2166. Two Roads - http://acm.timus.ru/problem.aspx?num=2166
 *
 * Strategy:
 * The contour lines of |x| + |y| are 45 degree-rotated squares around the origin, so we rotate the
 * input lines by 45 degrees and then find the farthest intersection from the origin along the
 * positive x-axis. We find that intersection by ordering the lines by slope and then checking each
 * neighboring pair of lines (in the order) for their point of intersection. We then rotate the
 * lines 90 degrees and repeat three more times. Vertical lines are handled separately.
 *
 * Performance:
 * O(Nlog N), which runs the test suite in 0.25s using 12,368KB memory.
 */

#include <stdio.h>
#include <algorithm>
#include <vector>
#include <iterator>
#include <cmath>

using ld = long double;

const int maxn = 100000;
const ld pi = std::acos(-1);

struct line {
    ld dx, dy, x, y;
} L[maxn];

int N;

ld sgn(ld x) {
    return x < 0 ? -1 : 1;
}

bool fn(const line& a, const line& b) { // Ordering function for slopes
    ld r = a.dy*b.dx - b.dy*a.dx;
    return sgn(a.dx*b.dx)*r < 0;
}

line rotate(const line& L, ld theta) {
    ld cos = std::cos(theta), sin = std::sin(theta);
    ld newX = L.x * cos - L.y * sin, newY = L.x * sin + L.y * cos;
    ld newDx = L.dx * cos - L.dy * sin, newDy = L.dx * sin + L.dy * cos;
    return { newDx, newDy, newX, newY };
}

ld calc() { // Finds the farthest intersection of any line along the x-axis
    std::vector<line> v;
    // Skip vertical lines and sort the rest according to slope
    std::copy_if(L, L+N, std::back_inserter(v), [](const line& l) { return std::abs(l.dx) > 0.1; });
    std::sort(v.begin(), v.end(), fn);

    ld max = 0;
    for(int i = 0; i < v.size()-1; i++) {
        // Some renaming of variables
        ld y1 = v[i].y, x1 = v[i].x, y2 = v[i+1].y, x2 = v[i+1].x;
        ld dy1 = v[i].dy, dy2 = v[i+1].dy, dx1 = v[i].dx, dx2 = v[i+1].dx;
        ld k1 = dy1/dx1, k2 = dy2/dx2;

        // Find the intersection between L[i] and L[i+1]
        auto x = (y1 - y2 + k2*x2 - k1*x1)/(k2 - k1), y = y1 + k1*(x - x1);
        line lr = { 0, 0, x, y };

        // Rotate it back to find the original Manhattan distance
        lr = rotate(lr, pi/4);
        max = std::max(std::abs(lr.x)+std::abs(lr.y), max);
    }
    return max;
}

int main() {
    line hl = { 0, 1, 0, 0 }, vl = { 1, 0, 0, 0 };
    scanf("%d", &N);
    for(int i = 0; i < N; i++) {
        ld x1, y1, x2, y2;
        scanf("%lf %lf %lf %lf", &x1, &y1, &x2, &y2);
        L[i] = { x2-x1, y2-y1, x1, y1 };
        if(L[i].dx == L[i].dy) // Vertical (after rotating)
            vl = rotate(L[i], pi/4);
        else if (L[i].dx == -L[i].dy) // Horizontal (same)
            hl = rotate(L[i], pi/4);
    }

    for(int i = 0; i < N; i++) // Rotate everything by 45deg
        L[i] = rotate(L[i], pi/4);

    // Pre-handle the distance between the vertical and horizontal lines if any
    ld max = (!hl.dy && !vl.x) ? std::abs(hl.y) + std::abs(hl.x) : 0;
    for(int i = 0; i < 4; i++) { // Find the farthest intersection for each 90deg rotation of lines
        for(int i = 0; i < N; i++)
            L[i] = rotate(L[i], pi/2);
        max = std::max(max, calc());
    }
    printf("%.15lf", max);
}
