/* 2157. Skydiving - http://acm.timus.ru/problem.aspx?num=2157
 *
 * Strategy:
 * Dynamic programming. For each x coordinate, keep an ordered array of points (clouds) that exist
 * within that vertical segment, and associate every point with the maximum time-to-fall calculated
 * so far. For each point p, bottom up, update the maximum time-to-fall of any points q from whence
 * we can fall to point p, starting at zero velocity. Note that we only need to update the lowest
 * point that reaches p for any given x coordinate since any higher points with that x coordinate 
 * will receive a higher value from such a point than from p directly.
 * 
 * Performance:
 * O(sqrt(max(y))*n*log n), runs the tests in 0.375s using 4,468KB memory.
 */

#include <stdio.h>
#include <unordered_map>
#include <vector>
#include <algorithm>

double A[50001]; // Optimal time-to-fall from each point, not counting the rest time of that point
struct point { int x, y, c, i; } points[50001]; // Points { coordinates, rest time, original index }

std::unordered_map<int, std::vector<point>> m; // Maps each x coordinate to its points

int main() {
    int n, x, y;

    scanf("%d %d %d", &n, &x, &y);
    for(int i = 0; i < n; i++) {
        int xi, yi, ci;
        scanf("%d %d %d", &xi, &yi, &ci);
        points[i] = point { xi, yi, ci, i };
    }
    points[n] = point { x, y, 0, n };

    auto lt = [] (const point& a, const point& b) { return a.y < b.y; };

    std::sort(points, points+n, lt);
    for(int i = 0; i <= n; i++) // Insert into x-coordinate lists
        m[points[points[i].i = i].x].push_back(points[i]);

    for(int i = 0; i <= n; i++) // Init time-to-fall with sqrt(2y/g)
        A[i] = std::sqrt(points[i].y/5.0);

    for(int i = 0; i <= n; i++) { // For each point
        int y = points[i].y;
        for(int j = 0; ; j++) { // Search outwards horizontally from this point
            int dx = (1-2*(j%2))*(j+1)/2; // 0, 1, -1, 2, -2, ...
            if(5*dx*dx > points[n].y-y) // Can't fall from any points beyond this horizontally
                break;                  // (using the standard gt^2/2 formula)

            int x = points[i].x + dx;
            // Find the lowest point at this x coordinate that can reach us
            int y2 = y + std::max(5*dx*dx, 1);
            auto it = std::lower_bound(m[x].begin(), m[x].end(), point { 0, y2 }, lt);
            if(it != m[x].end())
                // Update the max time-to-fall from the other point with the time to fall between
                // the points + max time to fall from this point + resting time in this cloud
                A[it->i] = std::max(std::sqrt((it->y - y)/5.0) + A[i] + points[i].c, A[it->i]);
        }
    }
    printf("%.10lf", A[n]);
}
