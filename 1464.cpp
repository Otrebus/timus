/* 1464. Light - http://acm.timus.ru/problem.aspx?num=1464
 *
 * Strategy:
 * Sort the endpoints of the segments whose normals are facing the lamp point c by angle and sweep
 * a ray from c around those points while keeping the closest segment in the current ray direction
 * with the help of a heap. Whenever the closest segment changes, we note how much of the previously
 * closest segment is obscured by the new closest one, or vice-versa; at the end the lighted area
 * is the sum of all the triangles consisting of the vertices c and the unobscured endpoints of
 * each segment.
 *
 * Performance:
 * O(nlog n), runs the tests in 0.281s using 3,640KB memory.
 */

#include <stdio.h>
#include <cmath>
#include <algorithm>
#include <vector>
#include <random>

using ld = long double;

struct vec {
    ld x, y;
    vec operator+ (const vec& a) const { return { x+a.x, y+a.y }; }
    vec operator- (const vec& a) const { return { x-a.x, y-a.y }; }
    ld operator* (const vec& a) const { return x*a.x + y*a.y; }
    ld operator% (const vec& a) const {  return x*a.y - y*a.x; }
    vec operator/ (ld f) const { return { x/f, y/f }; }
    vec operator* (ld f) const { return { x*f, y*f }; }
    vec n() { return (*this)/std::sqrt(x*x+y*y); }
    ld d() const { return std::sqrt(x*x+y*y); }
    vec r() { return { -y, x }; }
};

vec A[50000], c; // The input and center
int N;
const ld inf = 1e9, pi = std::acos(-1);

struct Clip { // Parametric clipping coordinates of each segment
    ld m, M;
} v[50001];
int P[50001], H[50001], h = 0; // Heap, heap position, heap size
int Hp;

struct Event { // Start/end events of segments
    int i;
    bool s;
};

std::vector<Event> E;

ld ang(vec a, vec b) {
    // Calculates the angle between vectors a and b in [0, 2pi]
    ld angle = std::atan2(a.n()%b.n(), a.n() * b.n());
    return angle < 0 ? angle + 2*pi : angle;
}

ld intersect(vec p0, vec p1, vec q0, vec q1) {
    // Intersects two segments
    auto v = q0 - q1, u = p1 - p0, b = q0 - p0;
    auto d = (u.x*v.y - v.x*u.y);
    if(!d)
        return -1;

    auto s = (b.x*v.y - v.x*b.y)/d;
    auto t = (u.x*b.y - b.x*u.y)/ld(d);
    return t > 0 ? s : -1;
}

ld dist(int i, vec r) {
    // The distance to segment i in the direction of r
    auto s = intersect(A[i], A[(i+1)%N], c, r);
    auto in = A[i] + (A[(i+1)%N] - A[i])*s;
    return (in-c).d();
}

void heapswap(int i, int j) {
    // Swaps positions i and j in the heap
    std::swap(P[H[i]], P[H[j]]);
    std::swap(H[i], H[j]);
}

void insert(int a, vec r) {
    // Inserts segment into the heap given viewing direction r
    H[++h] = a;
    P[a] = h;
    for(int i = h; i > 1; i /= 2)
        if(dist(H[i/2], r) > dist(H[i], r))
            heapswap(i/2, i);
}

void heapify(int j, vec r) {
    // Heapifies the heap given viewing direction r
    int m = j;
    for(auto pm : { j*2, j*2+1 })
        if(pm <= h && dist(H[pm], r) < dist(H[m], r))
            m = pm;
    if(m != j) {
        heapswap(m, j);
        heapify(m, r);
    }
}

void remove(int i, vec r) {
    // Removes segment i given viewing direction r
    P[H[h]] = P[i];
    H[P[i]] = H[h--];
    heapify(P[i], r);
    P[i] = 0;
}

int main() {
    std::default_random_engine generator(0);
    std::uniform_real_distribution<ld> ngen(-1, 1);

    scanf("%lf %lf %d", &c.x, &c.y, &N);
    for(int i = 0; i < N; i++)
        scanf("%lf %lf", &A[i].x, &A[i].y);

    int s = 0;
    ld mind = inf;

    for(int i = 0; i < N; i++) {
        if((A[(i+1)%N]-A[i]).r().n()*(A[i]-c).n() < -1e-6) {
            E.push_back({ i, true });
            E.push_back({ (i+1)%N, false });
        }
    }

    // A random starting direction to sweep from
    auto r0 = vec{ ngen(generator), ngen(generator) }.n()*10000.0;

    // Sort events by angle
    std::sort(E.begin(), E.end(), [r0] (auto a, auto b) {
        auto a1 = ang(r0-c, A[a.i]-c), a2 = ang(r0-c, A[b.i]-c);
        if(std::abs(a1-a2) < 1e-6) // Tiebreak by start/end
            return a.s > b.s;
        return a1 < a2;
    });

    // Initialize the heap with the segment ordering in the start direction
    for(auto& e : E) {
        auto s = intersect(A[e.i], A[(e.i+1)%N], c, r0);
        if(e.s && s > 0 && s < 1)
            insert(e.i, r0);
    }

    // Initialize the visible parts of each segment
    for(int i = 0; i < N; i++)
        v[i].m = -inf, v[i].M = inf;

    Hp = H[1]; // Previous closest segment

    // Sweep through the events
    for(auto& e : E) {
        auto j = e.i, jp = (j-1+N)%N;

        if(!e.s && P[jp])
            remove(jp, A[j]);
        if(e.s && !P[j])
            insert(j, A[e.i]);

        // If the closest segment changed as a result of the new event, update the
        // information about the visible part of each segment
        if(h && H[1] != Hp) {
            // The previous closest segment was possibly occluded from the left side, intersect it
            // by the current direction to calculate its new maximum parameter
            auto s = intersect(A[Hp], A[(Hp+1)%N], c, A[j]);
            v[Hp].M = std::min(v[Hp].M, s);
            Hp = H[1];

            // The new closest segment was possibly occluded from the right side, update its minimum
            s = intersect(A[Hp], A[(Hp+1)%N], c, A[j]);
            v[Hp].m = std::max(v[Hp].m, s);
        }
    }

    // The visible area admitted by each unobscured part of a segment is the far edge of a triangle
    // with vertex at the center so we sum all of those together
    ld a = 0;
    for(int i = 0; i < N; i++) {
        if(v[i].m > -inf && v[i].M < inf) {
            auto p = A[i], w = A[(i+1)%N] - A[i];
            a += std::abs((p + w*v[i].m - c)%(p + w*v[i].M - c))/2;
        }
    }

    printf("%.10lf\n", a);
}
