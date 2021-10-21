/* 1281. River Basin - http://acm.timus.ru/problem.aspx?num=1281
 *
 * Strategy:
 * Join river segments using disjoint sets; for each set, calculate the area of its convex hull.
 * 
 * Performance:
 * O(N log k), runs the tests in 0.015s using 284KB memory.
 */

#include <vector>
#include <unordered_map>
#include <algorithm>
#include <iterator>
#include <numeric>

using ld = long double;

struct point {
    ld x, y;
    bool operator< (point& v) { return std::make_pair(x, y) < std::make_pair(v.x, v.y); };
};

// Disjoint-set operations
int P[11]; // Set parents
void link(int a, int b) { P[a] = b; }
int find(int s) { return s != P[s] ? find(P[s]) : P[s]; }
void join(int a, int b) { link(find(a), find(b)); }

std::unordered_map<ld, int> M; // Maps coordinate to river
std::vector<point> R[11]; // The points of each input river

bool turnsRight(point a, point b, point c) { return (b.x-a.x)*(c.y-a.y) - (c.x-a.x)*(b.y-a.y) < 0; }

ld area(std::vector<point> v) { // Calculates the area of a river
    std::vector<point> ps, ps2; // Convex hull parts

    std::sort(v.begin(), v.end());

    for(auto it = v.begin(); it < v.end(); ps.push_back(*it), it++) // Lower hull part
        for(int i = ps.size() - 2; i >= 0 && turnsRight(ps[i], ps[i+1], *it); i--)
            ps.pop_back();

    for(auto it = v.rbegin(); it < v.rend(); ps2.push_back(*it), it++) // Upper hull part
        for(int i = ps2.size() - 2; i >= 0 && turnsRight(ps2[i], ps2[i+1], *it); i--)
            ps2.pop_back();

    ps.insert(ps.end(), ps2.begin(), ps2.end()); // Merge the convex hull parts

    ld r = 0; // Calculate the area using the shoelace formula and return
    for(int i = 0; i < ps.size(); i++)
        r += ps[i].x*ps[(i+1)%ps.size()].y - ps[i].y*ps[(i+1)%ps.size()].x;
    return r*0.5;
}

int main() {
    int N, k;
    ld x, y, max = 0;
    std::iota(P, P+11, 0);

    scanf("%d", &N);
    for(int i = 1; i <= N; i++) {
        scanf("%d", &k);
        for(int j = 0; j < k; j++) {
            scanf("%Lf %Lf", &x, &y);
            if(M.find(y*2001+x) == M.end()) // Add segment to river
                R[M[y*2001+x] = i].push_back({ x, y });
            else
                join(i, M[y*2001+x]); // Join rivers
        }
    }
    std::vector<point> w[11];
    for(int i = 1; i <= N; i++) // Determine the vertex sets of the joined rivers
        for(auto p : R[i])
            w[find(i)].push_back(p);

    for(int i = 1; i <= N; i++)
        max = std::max(area(w[i]), max);
    printf("%Lf", max);
}
