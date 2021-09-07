/* 1043. Cover an Arc - http://acm.timus.ru/problem.aspx?num=1043
 *
 * Strategy:
 * Find the center of the circle and calculate the area of the bounding box of the arc segment. More
 * detail in the comments.
 * 
 * Performance:
 * O(1), runs the tests in 0.015s using 280KB memory.
 */

#include <iostream>

using ld = long double;
const ld eps = 1e-9;

struct point {
    ld x, y;
    point operator-(const point& p) { return { x-p.x, y-p.y }; };
    bool operator%(const point& p) { return x*p.y - p.x*y > 0; }
};

int main() {
    int x1, x2, x3, y1, y2, y3;
    std::cin >> x1 >> y1 >> x2 >> y2 >> x3 >> y3; // aka p1, p2, p3

    // Find the center and radius of the circle. We do that by forming three equations |p_i-c| = r,
    // substituting one equation into the other to remove any squares, solving that system of two
    // equations for (c_x, c_y) and substituting them into the third to get the radius
    int A = 2*(x1-x2), B = 2*(y1-y2), C = 2*(x1-x3), D = 2*(y1-y3);
    int E = x1*x1 - x2*x2 + y1*y1 - y2*y2, F = x1*x1 - x3*x3 + y1*y1 - y3*y3;

    ld cy = (A*F - C*E)/ld(A*D-B*C), cx = (D*E - B*F)/ld(A*D-B*C);
    ld r = std::sqrt(x1*x1 - 2*x1*cx + cx*cx + y1*y1 - 2*y1*cy + cy*cy);

    point c { cx, cy };
    point v1 = { x1-cx, y1-cy }; // Endpoint 1
    point v2 = { x2-cx, y2-cy }; // Endpoint 2
    point v3 = { x3-cx, y3-cy }; // Inner point

    // Turning (taking the cross-product) from v1 to v3 to v2 will turn the same direction (have
    // the cross product point the same direction) both times if v3 is part of of the smaller
    // possible arc (< 180 deg), or flip the turn direction if v3 denotes the larger arc. For any
    // other point in place of v3, if it flips (or doesn't flip) direction the same way as v3 it's
    // within the same arc segment as v3
    auto z1 = v1%v3, z2 = v1%v2, z3 = v3%v2, z4 = v1%v2;
    bool inside = v1%v3 == v1%v2 && v3%v2 == v1%v2;

    // The points of the circle tangenting its axis-aligned bounding box (AABB)
    point pts[4] = { { cx, cy+r }, { cx, cy-r }, { cx-r, cy }, { cx+r, cy } };
    // The corners of the bounding box of the arc, initialized with its endpoints
    point m { std::min(x1, x2), std::min(y1, y2) }, M { std::max(x1, x2), std::max(y1, y2) };

    for(int i = 0; i < 4; i++) {
        // We expand the bounding box by every point of the circle AABB that's within the arc
        if((v1%(pts[i]-c) == v1%v2 && (pts[i]-c)%v2 == v1%v2) == inside) {
            m.x = std::min(m.x, pts[i].x);
            m.y = std::min(m.y, pts[i].y);
            M.x = std::max(M.x, pts[i].x);
            M.y = std::max(M.y, pts[i].y);
        }
    }
    auto W = std::ceil(M.x-eps)-std::floor(m.x+eps), H = (std::ceil(M.y-eps)-std::floor(m.y+eps));
    std::cout << int(W)*int(H);
}
