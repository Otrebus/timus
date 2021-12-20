/* 1848. Fly Hunt - http://acm.timus.ru/problem.aspx?num=1848
 *
 * Strategy:
 * Sweep-line; first sort all the events (start/end of segments, and points) vertically, and
 * then maintain a set of segments that currently cross the sweepline as we descend to help us
 * determine whether any point encountered is inside the polygon.
 *
 * Performance:
 * Linearithmic in n+m, runs the tests in 0.281s using 19,544KB memory.
 */

#include <vector>
#include <algorithm>
#include <stdio.h>
#include <set>

using ll = long long;
ll n, m, x, y, k;

struct vec {
    ll x, y;
    ll operator% (const vec& v) { return x*v.y - y*v.x; }
    vec operator- (const vec& v) const { return { x-v.x, y-v.y }; }
    bool operator== (const vec& v) const { return x == v.x && y == v.y; }
};

enum Type { Left, Right, Point, End };
int c[5] = { 0, 2, 1, 3 }; // The sort order of the above types used for tiebreaking
int R[30000]; // The result

struct event {
    vec u, v; // Top and bottom points of the event, if applicable
    Type t;   // The type of the event
    short i;  // The associated segment, if not a point
};

bool func(const event& a, const event& b) {
    // The horizontal ordering of the events of the sweepline. Events that intersect to the left
    // are considered less than points on the right on the scanline, and as a tiebreaker,
    // left segments are smaller than points are smaller than right segments
    if(a.t != Point && b.t != Point) { // We compare two segments
        if(a.u == b.u) // If their top points are the same, check rotation
            return (b.v-b.u)%(a.v-b.u) < 0;
        if(a.u.y == b.u.y) // If the top points are on the same level, compare x coordinate
            return a.u.x < b.u.x;
        else if(b.u.y > a.u.y)
            // If segment b is above a, the highest point of segment
            // a needs to be in the left halfspace it defines, for a to be smaller
            return (b.v-b.u)%(a.u-b.u) < 0;
        return !func(b, a);
    } else if(a.t == Point && b.t != Point) { // We compare a point and segment
        // A point compares less if it's in the left halfspace of the segment it compares to
        if((a.u-b.u)%(b.v-b.u) == 0) // If it straddles, check if segment is horizontal
            return (b.u.y == b.v.y) ? (a.u.x <= b.v.x) : (c[a.t] < c[b.t]);
        else
            return (b.v-b.u)%(a.u-b.u) < 0;
    }
    return !func(b, a);
}

std::vector<event> E;
std::set<event, decltype(&func)> S(func);

int main() {
    scanf("%lld", &n);
    for(int i = 0; i < n; i++) {
        scanf("%lld %lld", &x, &y);
        E.push_back( { vec { x, y }, vec { x, y }, Point, 0 } );
    }

    scanf("%lld", &m);
    for(short i = 0; i < m; i++) {
        scanf("%lld", &k);
        std::vector<vec> v;
        for(int i = 0; i < k; i++) {
            scanf("%lld %lld", &x, &y);
            v.push_back({ x, y });
        }
        for(int j = 0; j < v.size(); j++) {
            vec a = v[j], b = v[(j+1)%v.size()];
            // Make sure point a is above a, and check if the segment is part of the left or right
            // side of a polygon
            if(a.y <= b.y) {
                if(a.y == b.y && a.x < b.x) // Horizontal segments are considered right
                    std::swap(a, b);
                E.push_back({ b, a, Right, i });
                E.push_back({ a, b, End, i });
            } else if(a.y > b.y) {
                E.push_back({ a, b, Left, i });
                E.push_back({ b, a, End, i });
            }
        }
    }
    
    // Sort events top-down and left-right
    std::sort(E.begin(), E.end(), [] (const event& a, const event& b) {
        return (a.u.y == b.u.y) ? (a.t < b.t) : (a.u.y > b.u.y);
    });

    for(auto& e : E) {
        if(e.t == Point) {
            // Find the event larger than the point and check if it's the right side of a polygon.
            // If so, we're inside the polygon
            auto b = S.upper_bound(e);
            if(b != S.end() && b->t == Right)
                R[b->i]++;
        }
        else if(e.t == End) // Erase the corresponding segment
            S.erase({ e.v, e.u });
        else
            S.insert(e);
    }
    for(int i = 0; i < m; i++)
        printf("%d\n", R[i]);
}
