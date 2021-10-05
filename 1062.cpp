/* 1062. Triathlon - http://acm.timus.ru/problem.aspx?num=1062
 *
 * Strategy:
 * If we take the reciprocal of the inputs (into time per distance), then for each contestant i
 * we want a_j*x + b_j*y + c_j*z > a_i*x + b_i*y + c_i*z for every contestant j, meaning we want
 * (a_j-a_i)x + (b_j-b_i)y + (c_j-c_i)z > 0 for all j, which geometrically means we take the
 * intersection of all the given halfspaces and check if what remains is within the quadrant
 * x > 0, y > 0, z > 0 (since the section distances need to be positive according to the problem
 * description). Since the boundary of these halfspaces pass through the origin, we can instead
 * intersect those boundary planes with the plane x + y = 1 to form an equivalent 2d problem, where
 * we iterate through the projected constraints and clip away the corresponding halfplane from the
 * solution polygon (initialized to the equivalent of the above mentioned quadrant), and check that
 * there's some point left at the end.
 *
 * Performance:
 * O(N^3), runs the tests in 0.015s using 200KB memory
 */

#include <stdio.h>
#include <vector>

using ld = long double;

struct vec {
    ld x, y, z;
    vec operator-(const vec& v) { return { x-v.x, y-v.y, z-v.z }; };
    vec operator+(const vec& v) { return { x+v.x, y+v.y, z+v.z }; };
    ld operator%(const vec& v) { return x*v.y - v.x*y; };
    vec operator*(ld t) { return { t*x, t*y, t*z }; }
} v[100];
int n, x, y, z;

bool intersect(vec p0, vec p, vec a, vec b, vec& isec) {
    // Intersects the line through p0 with direction p with the line segment (a, b)
    auto v = p, w = a-b, c = a-p0;
    auto det = v.x*w.y - w.x*v.y, s = (v.x*c.y-c.x*v.y)/det;
    isec = a+(b-a)*s;
    return (det && s >= 0 && s <= 1);
}

std::vector<vec> clip(vec p0, vec p, std::vector<vec> r) {
    // Clips the polygon defined by the vertices in r by the line through p0 in direction p
    std::vector<vec> res;
    vec is;
    for(int i = 1, n = r.size(); i <= r.size(); i++) {
        if(intersect(p0, p, r[i-1], r[i%n], is)) // p intersects this segment, add new point
            res.push_back(is);
        if((r[i%n]-p0)%p > 0) // We include everything to the right of p
            res.push_back(r[i%n]);
    }
    return res;
}

bool test(int i) { // Checks if we can make i win against everyone
    std::vector<vec> r = { { 0, 0 }, { 1, 0 }, { 1, 1e9 }, { 0, 1e9 } }; // The winnable set
    for(int j = 0; j < n; j++) {
        if(i == j)
            continue;
        vec w = v[j]-v[i];
        // Here w.x etc are actually the inverted speeds, next up x/y/z will be the distances
        ld A = w.x-w.y, B = w.z, C = -w.y; // Prepare to intersect with x + y = 1
        if(!A && !B) { // This plane is parallel to x + y = 1, could clip away everything
            if((w.x <= 0 || w.y <= 0))
                return false;
        } else {
            // Intersect the constraint with x + y = 1 and clip away the equivalent 2d halfplane
            vec n = { A, B }, p = { -B, A };
            vec p0 = B ? vec { 0, C/B } : vec { C/A, 0 };
            r = clip(p0+n*1e-9, p, r);
        }
    }
    return !r.empty(); // If there are any winnable projected section distances left, we win
}

int main() {
    scanf("%d", &n);
    for(int i = 0; i < n; i++) {
        scanf("%d %d %d", &x, &y, &z);
        v[i] = { 10000/ld(x), 10000/ld(y), 10000/ld(z) };
    }
    for(int i = 0; i < n; i++)
        printf("%s\n", test(i) ? "Yes" : "No");
}
