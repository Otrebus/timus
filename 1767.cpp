/* 1767. The House of Doctor Dee - http://acm.timus.ru/problem.aspx?num=1767
 *
 * Strategy:
 * The two pairs of locations in effect form two rectangles. If we perform the union of both
 * rectangles, producing another rectangle, the answer is then either the width plus height, or the
 * maximum of the width and height of that union rectangle, depending on if the two paths between
 * the locations both go along a northwest/southeast slope (the direction doesn't matter) or if one
 * goes northwest/southeast and the other southwest/northeast.
 * 
 * Performance:
 * Constant, runs the test cases in 0.015s using 256KB memory.
 */

#include <iostream>
#include <algorithm>

struct Point { long long x, y; };

std::pair<Point, Point> toBox(Point in1, Point in2)
{
    return { { std::min(in1.x, in2.x), std::min(in1.y, in2.y) },
             { std::max(in1.x, in2.x), std::max(in1.y, in2.y) } };
}

long long intersectLine(long long s1, long long s2, long long t1, long long t2)
{
    return s1 > t1 ? intersectLine(t1, t2, s1, s2) : std::min( { s2-t1, t2-t1, s2-s1 } );
}

long long calc(std::pair<Point, Point> b1, std::pair<Point, Point> b2, bool opposite)
{
    long long l1 = intersectLine(b1.first.x, b1.second.x, b2.first.x, b2.second.x);
    long long l2 = intersectLine(b1.first.y, b1.second.y, b2.first.y, b2.second.y);
    if(l1 < 0 || l2 < 0)
        return 0;
    return opposite ? std::max(l1, l2) : l1 + l2;
}

int main()
{
    Point in[4];
    for(long long i = 0; i < 4; i++)
        std::cin >> in[i].x >> in[i].y;
    long long dys = (in[3].y-in[2].y)*(in[1].y-in[0].y);
    long long dxs = (in[3].x-in[2].x)*(in[1].x-in[0].x);
    std::cout << calc(toBox(in[0], in[1]), toBox(in[2], in[3]), dxs < 0 ? (dys > 0) : dys < 0);
}