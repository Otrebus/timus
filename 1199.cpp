/* 1199. Mouse - http://acm.timus.ru/problem.aspx?num=1199
 *
 * Strategy:
 * Relatively straightforward - calculate the pairwise distance between the polygons (exploded
 * outwards 10cm) and then run a shortest-paths algorithm - but as always, the devil is in the
 * details and in this problem there's a lot of them, which means they are better explained
 * in the code below.
 *
 * Performance:
 * O(N^3 + N^2*M^2), where M is the number of points per polygon, runs the tests in 0.281s using
 * 772KB memory.
 */

#include <stdio.h>
#include <cmath>
#include <vector>
#include <algorithm>

using ld = long double;

struct point {
    ld x, y;
    point operator+ (const point& a) const { return { x+a.x, y+a.y }; }
    point operator- (const point& a) const { return { x-a.x, y-a.y }; }
    ld operator* (const point& a) const { return x*a.x + y*a.y; }
    ld operator% (const point& a) const { return x*a.y - y*a.x; }
    point operator/ (ld f) const { return { x/f, y/f }; }
    point operator* (ld f) const { return { x*f, y*f }; }
    point operator += (point p) { return *this = { p.x+x, p.y+y }; }
    point operator /= (ld f) { return *this = { x/f, y/f }; }
    point n() { return (*this)/std::sqrt(x*x+y*y); }
    ld d2() { return x*x+y*y; }
    ld d() { return std::sqrt(x*x+y*y); }
    point r() { return { y, -x }; }
};

const ld inf = 1e300;
const ld W = 10; // The width of the safe zone

int K[100]; // The number of points in each polygon
point v[100][10]; // The polygons and their points
ld D[102][102]; // The distance between the polygons
int next[102][102]; // Next point on a shortest path

int I[102][102]; // I[i][j] = a visible point on polygon i when leaving polygon i for polygon j
int J[102][102]; // Same as above but on j, arriving at shape j
point R1[102][102]; // R[i][j] = the best point leaving the offset of polygon i moving from i to j
point R2[102][102]; // Same as above, but arriving at polygon j

bool inArc(point p, point v, point w) {
    // Checks if point p is inside the arc spanning between point v and w
    if(v%w >= 0)
        return v%p > 0 && p%w > 0;
    return !inArc(p, w, v); // Probably not necessary since outside angles can't be >=180 degrees
}

ld distPointLine(point p, point q1, point q2, point& r) {
    // The distance from point p to the line segment (q1, q2), r = the closest point on the segment
    auto w = p-q1, v = q2-q1;

    point p2 = v*(v*w)/v.d2(); // Projected point

    if(v*w < 0 || v*w > v.d2()) { // Projected point is outside the segment, check the endpoints
        ld l1 = (p-q1).d();
        ld l2 = (p-q2).d();

        r = l1 < l2 ? q1 : q2;
        return std::min(l1, l2);
    }
    r = q1+p2;
    return (p2-w).d();
}

ld distPointArc(point p, point q1, point q2, point q3, point& r) {
    // The distance from point p to the arc formed by the points q1, q2, q3 -- r as above
    point q = q2 + (p - q2).n()*W;

    if(!inArc(p-q2, q1-q2, q3-q2))
        return inf;

    r = q;
    return (p-q).d();
}

ld distLineArc(point p1, point p2, point q1, point q2, point q3, point& r1, point& r2) {
    // The distance from the line segment (p1, p2) to the arc specified as above
    // where r1 is the closest point on the line segment and r2 is the closest point on the arc
    auto w = q2-p1, v = p2-p1;
    ld l = inf, lm = inf;

    point ra, rb;

    if(v*w >= 0 && v*w <= v.d2()) { // Closest point in segment, do projection to find r1
        point p = p1 + v*(v*w)/v.d2();
        point q = q2 + (p-q2).n()*W; // The point on the arc

        if(!inArc(p-q2, q1-q2, q3-q2))
            return inf;

        lm = (p-q).d();
        r1 = p, r2 = q;
    }

    // Closest point not on segment, must be on the endpoints
    l = distPointArc(p1, q1, q2, q3, ra);
    if(l < lm) {
        lm = l;
        r1 = p1;
        r2 = ra;
    }
    l = distPointArc(p2, q1, q2, q3, rb);
    if(l < lm) {
        lm = l;
        r1 = p2;
        r2 = rb;
    }
    return lm;
}

ld distLineLine(point p1, point p2, point q1, point q2, point& r1, point& r2) {
    // Returns the distance between two line segments and their closest points
    point v[4][3] = { { p1, q1, q2 }, { p2, q1, q2 }, { q1, p1, p2 }, { q2, p1, p2 } };
    ld lm = inf;

    // It's one of four line/point distances
    for(int i = 0; i < 4; i++) {
        point ra;
        ld l = distPointLine(v[i][0], v[i][1], v[i][2], ra);
        if(l < lm) {
            r1 = i < 2 ? v[i][0] : ra;
            r2 = i < 2 ? ra : v[i][0];
            lm = l;
        }
    }
    return lm;
}

ld distArcArc(point p1, point p2, point p3, point q1, point q2, point q3, point& r1, point& r2) {
    // Returns the distance between two arcs (if the line between them goes through both)
    r1 = p2 + (q2 - p2).n()*W;
    r2 = q2 + (p2 - q2).n()*W;

    if(!inArc(r1-p2, p1-p2, p3-p2) || !inArc(r2-q2, q1-q2, q3-q2))
        return inf;

    return (r1-r2).d();
}

void trans(point p1, point p2, point& r1, point& r2) {
    // Expands the line segment (p1, p2) 10cm "outwards" in a right-handed manner
    r1 = p1 + (p2-p1).r().n()*W;
    r2 = p2 + (p2-p1).r().n()*W;
}

ld dist(point p1, point p2, point p3, point q1, point q2, point q3, point& r1, point& r2) {
    // Calculates the distance between shape p1, p2, p3 (meaning line segment (p1, p2) and
    // corresponding arc specified by p3) and shape q1, q2, q3
    point ra, rb;
    ld lm = inf, l;

    // Expand the shapes outwards (we're dealing with furniture and safe zones, remember)
    point pt1, pt2, pt3, pt4, qt1, qt2, qt3, qt4;
    trans(p1, p2, pt1, pt2);
    trans(p2, p3, pt3, pt4);
    trans(q1, q2, qt1, qt2);
    trans(q2, q3, qt3, qt4);

    // This is all pretty self-descriptive, we just need to get the closest points right (ra, rb)
    l = distLineLine(pt1, pt2, qt1, qt2, ra, rb);
    if(l < lm)
        lm = l, r1 = ra, r2 = rb;

    l = distLineArc(pt1, pt2, qt2, q2, qt3, ra, rb);
    if(l < lm)
        lm = l, r1 = ra, r2 = rb;

    l = distLineArc(qt1, qt2, pt2, p2, pt3, ra, rb);
    if(l < lm)
        lm = l, r1 = rb, r2 = ra;

    l = distArcArc(pt2, p2, pt3, qt2, q2, qt3, ra, rb);
    if(l < lm)
        lm = l, r1 = ra, r2 = rb;

    return lm;
}

ld dist(point p, point q1, point q2, point q3, point& r) {
    // Calculates the distance between point p and the shape q1, q2, q3 (as specified above)
    ld lm = inf, l;
    point ra;

    point qt1, qt2, qt3, qt4;
    trans(q1, q2, qt1, qt2); // Explode the line outwards (account for the safe zone)
    l = distPointLine(p, qt1, qt2, ra);
    if(l < lm)
        lm = l, r = ra;

    trans(q2, q3, qt3, qt4);
    l = distPointArc(p, qt2, q2, qt3, ra);
    if(l < lm)
        lm = l, r = ra;

    return lm;
}

ld calc(int a, int b, int& ri, int& rj, point& r1, point& r2) {
    // Returns the closest distance between polygon a and b, and returns corresponding visible
    // points on the polygons, ri and rj, as well as the points of departure and arrival on the safe
    // zones (r1 and r2)
    ld min = inf;
    int m = K[a], n = K[b];
    point ra, rb;

    for(int i = 0; i < m; i++) {
        for(int j = 0; j < n; j++) {
            // Test every line segment + arc against the others
            ld l = dist(v[a][i], v[a][(i+1)%m], v[a][(i+2)%m],
                        v[b][j], v[b][(j+1)%n], v[b][(j+2)%n], ra, rb);
            if(l < min)
                min = l, r1 = ra, r2 = rb, ri = (i+1)%m, rj = (j+1)%n;
        }
    }
    return min;
}

ld calc(point p, int a, int& ri, point& r) {
    // Calculates the distance from point p to shape a, and return the values as specified above
    int m = K[a];
    ld l, lm = inf;
    point ra;

    // First check if we're actually inside the safe zone, and if so, the closest segment
    for(int i = 0; i < m; i++) {
        l = distPointLine(p, v[a][i], v[a][(i+1)%m], ra);
        if(l < lm) {
            lm = l;
            ri = (i+1)%m;
            r = ra;
        }
    }

    if(lm <= W) // We're inside the safe zone, meaning distance 0 to this polygon
        return 0;
    lm = inf;

    // Otherwise, calculate the closest segment as usual
    for(int i = 0; i < m; i++) {
        l = dist(p, v[a][i], v[a][(i+1)%m], v[a][(i+2)%m], ra);
        if(l < lm) {
            lm = l;
            r = ra;
            ri = (i+1)%m;
        }
    }
    return lm;
}

void addPath(std::vector<point>& out, int i, int j, int k) {
    // When arriving from polygon i, going through polygon j and leaving for polygon k, output the
    // path we need to take around polygon j.
    int a = J[i][j], b = I[j][k];
    int m = K[j];

    int d = (b-a+m)%m < m/2 ? 1 : -1; // Make sure we take the shortest side of the polygon
    for(int r = a; r != b; r = (r+d+m)%m)
        out.push_back(v[j][r]);
    out.push_back(v[j][b]);
}

int main() {
    ld xm, ym, xc, yc;
    int N;

    scanf("%lf %lf %lf %lf %d", &xm, &ym, &xc, &yc, &N);
    xm *= 100, ym *= 100, xc *= 100, yc *= 100;

    // Initialize the distance matrix
    for(int i = 0; i < 102; i++)
        for(int j = 0; j < 102; j++)
            D[i][j] = D[j][i] = inf;

    // Read the values
    for(int i = 0; i < N; i++) {
        scanf("%d", &K[i]);
        for(int j = 0; j < K[i]; j++) {
            scanf("%lf %lf", &v[i][j].x, &v[i][j].y);
            v[i][j].x *= 100;
            v[i][j].y *= 100;
        }
    }

    // Sort the polygon points counterclockwise
    for(int i = 0; i < N; i++) {
        point c = { 0, 0 };
        for(int j = 0; j < K[i]; j++) // Calculate the centroid
            c += v[i][j];
        c /= K[i];
        std::sort(v[i], v[i]+K[i], [&c] (auto a, auto b) {
            return std::atan2(a.y-c.y, a.x-c.x) < std::atan2(b.y-c.y, b.x-c.x);
        });
    }

    // Calculate all the poly/poly distances
    for(int i = 0; i < N; i++) {
        for(int j = 0; j < N; j++) {
            if(i != j) {
                int ri, rj;
                point r1, r2;
                D[i][j] = calc(i, j, ri, rj, r1, r2);
                I[i][j] = ri;
                J[i][j] = rj;
                R1[i][j] = r1;
                R2[i][j] = r2;
            }
        }
    }
    
    // Special case 1: calculate the point/point distances
    D[100][101] = (point { xm, ym } - point { xc, yc }).d();
    R1[100][101] = { xm, ym };
    R2[100][101] = { xc, yc };
    next[100][101] = 101;
    D[101][100] = inf;

    // Special case 2: calculate the point/polygon distances
    for(int i = 0; i < N; i++) {
        int ri, rj;
        point r, s;

        // Mouse/polygon
        D[100][i] = calc({ xm, ym }, i, ri, r);
        R1[100][i] = { xm, ym };
        R2[100][i] = r;
        J[100][i] = ri;
        D[i][100] = inf;

        // Cheese/polygon
        D[i][101] = calc({ xc, yc }, i, rj, s);
        R1[i][101] = s;
        R2[i][101] = { xc, yc };
        I[i][101] = rj;
        D[101][i] = inf;
    }

    // Floyd-Warshall init ...
    for(int i = 0; i < 102; i++)
        for(int j = 0; j < 102; j++)
            next[i][j] = j;

    // .. and loop
    for(int k = 0; k < 102; k++)
        for(int i = 0; i < 102; i++)
            for(int j = 0; j < 102; j++)
                if(D[i][j] > D[i][k] + D[k][j])
                    D[i][j] = D[i][k] + D[k][j], next[i][j] = next[i][k];

    // Go through the shortest-paths path and add the corresponding enter/exit vertices and the
    // corresponding points along the polygons
    int prev = -1;
    std::vector<point> out;
    for(int i = 100; i != 101; ) {
        int j = next[i][101];

        // Add the points on the boundary of polygon i leading from polygon prev to polygon j
        if(prev != -1)
            addPath(out, prev, i, j);

        // Add the points on the boundaries of the safe zones of polygon i to polygon j
        out.push_back(R1[i][j]);
        out.push_back(R2[i][j]);

        prev = i;
        i = j;
    }

    // Remove duplicated points
    std::vector<point> out2;
    for(auto& o : out)
        if(out2.empty() || (out2.back()-o).d() > 1e-9)
            out2.push_back(o);

    // Output
    printf("%d\n", out2.size());
    for(auto& o : out2)
        printf("%.10lf %.10lf\n", o.x/100, o.y/100);
}
