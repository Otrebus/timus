/* 1514. National Park - http://acm.timus.ru/problem.aspx?num=1514
 *
 * Strategy:
 * Divide-and-conquer. First, partition the trees by the median and solve the problem for both
 * sides. Then, if we denote the best solution from either side to have circumference C, we merge
 * the solutions by forming all possible triangles by trees within distance C/2 from the middle
 * (and from each other) and update the best solution.
 *
 * Performance:
 * Linearithmic in N, runs the tests in 0.078s using 5,584KB memory.
 */

#include <stdio.h>
#include <vector>
#include <algorithm>
#include <cmath>

using ld = long double;
int N;

struct tree { // A tree represented by a vector
    ld x, y;
    int i, s; // The index of the tree, the side it's on in the partition
    tree operator/(ld f) { return { x/f, y/f }; }
    void operator+=(const tree& t) { x+=t.x, y+=t.x; }
    tree operator-(const tree& t) const { return { x-t.x, y-t.y }; }
    ld d() const { return std::sqrt(x*x+y*y); }
};

struct solution { tree a, b, c; };
ld circ(tree& a, tree& b, tree& c) { // The circumference of a triangle
    return (a-b).d() + (a-c).d() + (b-c).d();
}

solution solve(std::vector<tree>& v) {
    if(v.size() < 3)
        return { { -1e30, 1e30 }, { 1e30, 1e30 }, { 0, -1e15 } };

    // Calculate the median
    tree mid = { 0, 0 };
    for(auto& t : v)
        mid += t;
    mid = mid/v.size();
    
    std::vector<tree> a, b; a.reserve(v.size()), b.reserve(v.size());
    for(auto& t : v) // Partition the trees around the median
        (t.x <= mid.x && a.size() < v.size()/2 ? a : b).push_back(t);

    // Solve the left and right sides
    auto sa = solve(a), sb = solve(b);
    auto da = circ(sa.a, sa.b, sa.c), db = circ(sb.a, sb.b, sb.c);

    // Get the largest solution
    ld m = db;
    solution sol = sb;
    if(da < db)
        m = da, sol = sa;

    // Get all the trees within half the optimal solution from the middle
    std::vector<tree> w; w.reserve(v.size());
    for(auto& t : v)
        if(std::abs(t.x - mid.x) < m/2)
            w.push_back(t);

    // Form triangles from all the points inside the strip in the middle and update the solution
    for(int i = 0; i < w.size(); i++)
        for(int j = i+1; j < w.size() && w[j].y < w[i].y + m/2; j++)
            for(int k = j+1; k < w.size() && w[k].y < w[i].y + m/2; k++)
                if(std::abs(w[i].s + w[j].s + w[k].s) < 3)
                    if(circ(w[i], w[j], w[k]) < m)
                        m = circ(w[i], w[j], w[k]), sol = { w[i], w[j], w[k] };
    return sol;
}

int main() {
    std::vector<tree> T;
    scanf("%d", &N);
    for(int i = 0; i < N; i++) {
        ld x, y;
        scanf("%lf %lf", &x, &y);
        T.push_back({ x, y, i+1, 0 });
    }

    // Sort the trees vertically to help the merge step
    auto fn = [] (const tree& a, const tree& b) { return a.y==b.y ? a.x<b.x : a.y<b.y; };
    std::sort(T.begin(), T.end(), fn);

    auto sol = solve(T);
    printf("%.10lf\n", circ(sol.a, sol.b, sol.c));
    printf("%d %d %d", sol.a.i, sol.b.i, sol.c.i);
}
