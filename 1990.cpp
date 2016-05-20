/* 1990. Podracing - http://acm.timus.ru/problem.aspx?num=1990
 *
 * Strategy:
 * Order all points - cameras and vertices - by their y coordinate and iterate over them; for every
 * row of points sharing the same y coordinate, we find the widest space between any two adjacent 
 * points (and the horizontal distance to the walls), and take the minimum of all those widest
 * spaces.
 *
 * Performance:
 * Linearithmic in the size of the input, runs the test cases in 0.624s using 15,680KB memory.
 */

#include <iostream>
#include <iomanip>
#include <queue>

struct Point
{
    double x, y;
    enum Type { Left, Camera, Right } type;
    bool operator() (Point& a, Point& b) { return a.y == b.y ? a.x > b.x : a.y > b.y; }
    bool operator== (Point& b) { return x == b.x && y == b.y && type == b.type; }
};

struct Line
{
    Point p1, p2;
};

double interp(Point& p1, Point& p2, double y)
{
    return (y-p1.y)*((p2.x-p1.x)/(p2.y-p1.y)) + p1.x;
}

int main()
{
    std::ios::sync_with_stdio(false);
    std::priority_queue<Point, std::vector<Point>, Point> q;
    double min = std::numeric_limits<double>::max(), max = -1;
    int n, x, y;
    std::cin >> n;
    std::vector<Point> leftside, rightside;

    for(int i = 0; i < n; i++)
    {
        std::cin >> x >> y;
        leftside.push_back( { x, y, Point::Left });
        q.push( { x, y, Point::Left } );
    }
    int l = 0, r = 0; // Point to the current segment of the wall
    std::cin >> n;
    for(int i = 0; i < n; i++)
    {
        std::cin >> x >> y;
        rightside.push_back( { x, y, Point::Right });
        q.push( { x, y, Point::Right } );
    }
    std::cin >> n;
    while(n--)
    {
        std::cin >> x >> y;
        q.push( { x, y, Point::Camera } );
    }
    // We push a couple of dummy points to simplify the loop somewhat
    leftside.push_back( { leftside.back().x, leftside.back().y+1, Point::Left } );
    rightside.push_back( { rightside.back().x, rightside.back().y+1, Point::Right } );

    Point lastp { -1e10, -1e10, Point::Left }; // Previous point processed
    double left, right = lastp.x; // Left and right wall positions
    max = rightside[0].x - leftside[0].x ; // The current row's maximum space
    while(!q.empty())
    {
        Point p = q.top();
        q.pop();

        if(p.y != lastp.y) // New row, process what happened last row
        {
            if(max != -1)
                min = std::min(min, std::max(max, right-lastp.x));
            left = interp(leftside[l+1], leftside[l], p.y);
            right = interp(rightside[r+1],rightside[r], p.y);
            max = -1;
        }
        if(p == leftside[l+1]) // We reached a vertex on the left side, update the wall pointer on
            l++;
        else if(p == rightside[r+1]) // Same on the right side
            r++;
        else if(p.x < left || p.x > right // Camera outside the track, ignore
                || p.y < leftside[0].y || p.y > rightside[rightside.size()-2].y)
            continue;
        lastp = p;
        max = std::max(max, p.x-left);
        left = p.x;
    }
    if(max != -1)
        min = std::min(min, max);
    std::cout << std::fixed << std::setprecision(8) << min;
}