/* 1429. Biscuits - http://acm.timus.ru/problem.aspx?num=1429
 *
 * Strategy:
 * Calculate all the circle-circle intersections, count the number of connected components using
 * disjoint-sets, and use Euler's formula f = 1+e-v+k, where e is the number of edges (arcs between
 * the intersections), v is the number of vertices (unique intersections), k is the number of
 * connected components, and f is the number of faces (the answer).
 *
 * Performance:
 * O(N^2log N), runs the tests in 0.453s using 13,676KB memory.
 */

#include <stdio.h>
#include <algorithm>
#include <vector>
#include <cmath>

int X[500], Y[500], R[500];
int N, V = 0, E = 0;

using ld = long double;

struct vec { // Standard vector stuff
    ld x, y;
    vec operator+ (const vec& v) { return { x+v.x, y+v.y }; };
    vec operator- (const vec& v) { return { x-v.x, y-v.y }; };
    vec operator* (ld f) { return { f*x, f*y }; }
    vec operator/ (ld f) { return { x/f, y/f }; }
    ld d() { return std::sqrt(x*x+y*y); }
    vec n() { return *this/d(); };
    vec r() { return { -y, x }; }
};

struct node { // Disjoint-set node
    node* p; // Parent
    int r; // Rank
} nodes[500];

bool A[500]; // Components
std::vector<vec> v[500]; // Intersections
 
node* find(node* s) { // Returns the representative node of set s
    if(s != s->p)
        s->p = find(s->p);
    return s->p;
}

void link(node* a, node* b) { // Joins representative nodes a and b
    if(a->r > b->r)
        b->p = a;
    else {
        a->p = b;
        if(a->r == b->r)
            b->r++;
    }
}

void join(int a, int b)  { // Joins the sets of node a and b
    link(find(nodes+a), find(nodes+b));
}

void isec(int i, int j) {
    // Adds the intersections of circle i and j to their intersection arrays
    if(R[i] < R[j])
        std::swap(i, j);

    int dx = X[i] - X[j], dy = Y[i] - Y[j];
    int ri = R[i], rj = R[j], dx2 = dx*dx, dy2 = dy*dy, ri2 = ri*ri, rj2 = rj*rj, d2 = dx2 + dy2;

    // The circles cannot intersect if their radiuses don't agree with the distance between them
    if(X[i] == X[j] && Y[i] == Y[j] || d2 < ri2 - 2*ri*rj + rj2 || d2 > ri2 + 2*ri*rj + rj2)
        return;

    // Make a coordinate basis with one axis being the line between the origins, we have
    // (d-x)^2 + h^2 = ri^2 and x^2 + h^2 = r^2, where h is half the length between the
    // intersection points, d is the distance between the origins, and x is the part of d
    // that makes the triangles not right.
    vec p1, p2, ci = { X[i], Y[i] }, cj = { X[j], Y[j] }, dv = cj - ci;
    auto d = (cj-ci).d();
    dv = dv.n(); // X axis
    auto r = dv.r().n(); // Y axis

    // We solve the above equation here
    auto x = (ri2 - rj2 - d*d)/(2*d);
    auto h = std::sqrt(rj2 - x*x);
    p1 = ci + dv*(d+x) + r*h, p2 = ci + dv*(d+x) - r*h;

    // Push the intersections to both circles
    v[i].push_back(p1), v[i].push_back(p2);
    v[j].push_back(p1), v[j].push_back(p2);

    join(i, j); // The circles intersect so they are part of the same component
}

void remove_dup(int i) {
    // Removes duplicated intersections from circle i
    if(v[i].size() < 2)
        return;

    // Sort intersections by angle
    vec v0 = { X[i], Y[i] };
    std::sort(v[i].begin(), v[i].end(), [&v0] (auto& a, auto& b) {
        auto v1 = a-v0, v2 = b-v0;
        return std::atan2(v1.y, v1.x) < std::atan2(v2.y, v2.x);
    });
    std::vector<vec> w;
    w.reserve(v[i].size());

    // Remove any intersection close enough to the previous
    for(int j = 0; j < v[i].size(); j++) {
        auto v2 = v[i][(j+1)%v[i].size()], v1 = v[i][j];
        if((v2-v1).d() > 1e-9)
            w.push_back(v1);
    }
    if(!w.size()) // If there's just one cluster of duplicated intersections
        w.push_back(v[i][0]);
    v[i] = w;
}

int main() {
    scanf("%d", &N);
    for(int i = 0; i < N; i++) {
        scanf("%d %d %d", &X[i], &Y[i], &R[i]);
        for(int j = 0; j < i; j++) { // Remove duplicates
            if(X[i] == X[j] && Y[i] == Y[j] && R[i] == R[j]) {
                i--, N--;
                break;
            }
        }
        nodes[i].p = nodes+i;
    }

    for(int i = 0; i < N; i++) {
        remove_dup(i);
        int t1 = v[i].size(); // Intersections with circles up to this one
        for(int j = i+1; j < N; j++)
            isec(i, j);
        remove_dup(i);
        int t2 = v[i].size(); // Total intersections

        V += t2-t1; // Subtract the two to add the new unique intersections
        E += t2;    // The number of edges is the number of intersections on each circle
    }
    for(int i = 0; i < N; i++) // Mark every component
        A[find(nodes+i)-nodes] = true;
    int k = 0;
    for(int i = 0; i < N; i++)
        k += A[i];
    printf("%d", 1 + E - V + k);
}
