/* 1364. LaraKiller - http://acm.timus.ru/problem.aspx?num=1364
 *
 * Strategy:
 * Build the tunnel according to the rules, backtrack Lara's position in the tunnel to the given
 * position, and then output all tunnel coordinates up to T elements away.
 *
 * Performance:
 * O(NM), runs the tests in 0.031s using 452KB memory.
 */

#include <iostream>
#include <vector>

struct point {
    int y, x;
    point operator+ (const point& p) { return { y + p.y, x + p.x }; }
    bool operator!= (const point& p) { return y != p.y || x != p.x; }
};

point C[] = { { 0, 1 }, { 1, 0 }, { 0, -1 }, { -1, 0 } }; // Directions
bool visited[101][101];

point r, c;

int main() {
    int N, M, T, p, d = 0;
    std::cin >> N >> M >> r.y >> r.x >> c.y >> c.x >> T;
    point pos = { 1, 1 };

    std::vector<point> v({pos}); // The tunnel
    do { // Build the tunnel
        visited[pos.y][pos.x] = true;
        auto np = pos + C[d];
        // If we're blocked in the given direction, change direction
        if(np.y > N || np.y < 1 || np.x > M || np.x < 1 || visited[np.y][np.x])
            d = (d + 1)%4;
        else // Otherwise add the position to the tunnel
            pos = np, v.push_back(pos);
    } while (pos != r);

    for(p = v.size() - 1; p >= 0 && v[p] != c; p--); // Backtrack until we reach the alarm

    // Output everything T elements away in the tunnel
    for(int i = std::max(p-T, 0); i < std::min((int) v.size(), p+T+1); i++)
        std::cout << v[i].y << " " << v[i].x << std::endl;
}
