/* 1863. Peaceful Atom - http://acm.timus.ru/problem.aspx?num=1863
 *
 * Strategy:
 * Meet-in-the-middle. First, enumerate all the possible insertion depths for half of the input
 * by brute-force, then do the same for the second half, but for each value also associate the
 * maximum and minimum insertion depth for the sequence that produced the depth. For each of the
 * depths produced in the second half, use the associated min (max) depth to do a binary
 * search over the first half of the data to find the least (greatest) insertion depth we can
 * append to, and update the answer accordingly.
 *
 * Performance:
 * O(2^(k/2)k), runs the tests in 0.156s using 36,576KB memory.
 */

#include <iostream>
#include <vector>
#include <algorithm>
#include <functional>

struct lr { int n, l, r; }; // Depth, max and min depths

int main() {
    int n, s, k, x;
    std::cin >> n >> s >> k;

    std::vector<int> A = { s };

    // Produce all possible depths for the first half
    for(int i = 0; i < k/2; i++) {
        std::cin >> x;
        std::vector<int> v;
        for(auto a : A) {
            for(auto y : { a - x, a + x })
                if(y >= 0 && y <= n)
                    v.push_back(y);
        }
        A = v;
    }
    std::sort(A.begin(), A.end()); // Sort for the binary search
    A.erase(std::unique(A.begin(), A.end()), A.end()); // Remove duplicates

    // Produce the possible depths for the second half along with the max and min depths
    std::vector<lr> B = {{ 0, 0, 0 }};
    for(int i = k/2; i < k; i++) {
        std::cin >> x;
        std::vector<lr> v;
        for(auto p : B)
            for(auto y : { p.n - x, p.n + x })
                if(y >= -n && y <= n)
                    v.push_back({ y, std::min(p.l, y), std::max(p.r, y) });
        B = v;
    }

    // Do binary search over the first half to find what depth we can append to
    int min = n, max = 0;
    for(auto p : B) {
        auto it = std::lower_bound(A.begin(), A.end(), -p.l);
        if(it != A.end() && *it + p.r <= n)
            min = std::min(min, *it + p.n);

        auto iu = std::lower_bound(A.rbegin(), A.rend(), n-p.r, std::greater<int>());
        if(iu != A.rend() && *iu + p.l >= 0)
            max = std::max(max, *iu + p.n);
    }
    std::cout << min << " " << max;
}
