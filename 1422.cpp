/* 1422. Fireflies - http://acm.timus.ru/problem.aspx?num=1422
 *
 * Strategy:
 * For each point (firefly), make it the origin and scale each point so its components have no
 * common factor, and map each scaled point to the number of points sharing that scaled coordinate.
 * The answer is the maximum of the latter value, over all origin points.
 *
 * Performance:
 * O(N^2), runs the test cases in 1.39s using 668KB memory.
 */

#include <iostream>
#include <unordered_map>

int gcd(int a, int b) { return a == 0 ? b : gcd(b % a, a); }
int gcd(int a, int b, int c) { return gcd(a, gcd(b, c)); }

struct vector3d {
    int x, y, z;
    vector3d operator-(const vector3d& v2) { return {x-v2.x,y-v2.y,z-v2.z};};
    vector3d operator/(int d) { return {x/d,y/d,z/d};};
    bool operator==(const vector3d& v2) const { return x == v2.x && y == v2.y &&  z == v2.z; };

    struct Hash
    {
        size_t operator()(const vector3d& v) const
        {
            return std::hash<int>()(v.x)^(std::hash<int>()(v.y)<<11)^(std::hash<int>()(v.z)<<22);
        }
    };
};

vector3d A[2000];

int main() {
    int n;
    int r = 1;

    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cin >> n;

    for(int i = 0; i < n; i++)
        std::cin >> A[i].x >> A[i].y >> A[i].z;

    for(int i = 0; i < n; i++) {
        std::unordered_map<vector3d, int, vector3d::Hash> m;
        for(int j = 0; j < n; j++) {
            if(i == j)
                continue;
            vector3d v2 = A[j] - A[i];
            vector3d g = v2/gcd(v2.x, v2.y, v2.z);
            r = std::max(r, 1 + ++m[g]);
        }
    }
    std::cout << r;
}


