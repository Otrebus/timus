/* 1790. Searching for the Truth - http://acm.timus.ru/problem.aspx?num=1790
 *
 * Strategy:
 * Represent the possible dodecahedron positions as ranges which grow to the left and right after
 * every touch. Also note that these ranges can either be sparse or dense - for example, the range
 * (4, 5) grows as (3, 4, 5, 6), (2, 3, 4, 5, 6, 7), etc, where as the range (4) grows as (3, 5),
 * (2, 4, 6), etc. The strategy then is simply to simulate - start with all pillars as the range,
 * expand and merge all existing ranges, cut the range at the touch, and repeat.
 *
 * Performance:
 * O(m), runs the tests in 0.093s using 164KB memory.
 */

#include <vector>
#include <stdio.h>
#include <random>

struct range {
    int a, b;
    bool odd; // If the interior is sparse
};

std::vector<range> v;

std::vector<range> merge(range u, range v) { // Possibly merges two adjacent ranges 
    if(!u.odd && !v.odd) {
        if(v.a <= u.b+1) // Overlap, merge
            return { { u.a, v.b, false } };
        return { u, v };
    } else if(u.odd && v.odd) {
        if(u.b + 1 == v.a) // We overlap so we get a dense range in the middle
            return { { u.a, u.b-2, true }, { u.b, v.a, false }, { v.a+2, v.b, true } };
        else if(u.b == v.a || u.b + 2 == v.a) // We overlap so we get one non-dense range
            return { { u.a, v.b, true }};
        return { u, v };
    } else if(!u.odd && v.odd) {
        if(u.b <= v.a && v.a <= u.b + 1) // The dense range grows into the non-dense one
            return { { u.a, std::max(v.a, u.b), false }, { u.b+2, v.b, true } };
        return { u, v };
    } else if(u.odd && !v.odd) { // Similar to the above case
        if(v.a - 1 <= u.b && u.b <= v.a )
            return { { u.a, u.b-2, true }, { std::min(u.b, v.a), v.b, false } };
        return { u, v };
    }
}

std::vector<range> clip(int k, range& u) { // Removes a position from a range
    if(u.odd) {
        if((k-u.a)%2) // The position to remove falls into a hole in the range
            return { u };
        return { { u.a, k-2, true }, { k+2, u.b, true } }; // Otherwise cut
    } else
        return { { u.a, k-1, false }, { k+1, u.b, false } } ;
}

int main() {

    int n, m;
    scanf("%d %d", &n, &m);
    v.push_back({1, n, false});

    for(int i = 0; i < m; i++) {
        int k;
        scanf("%d", &k);

        std::vector<range> w; // The new set of ranges

        // Clip the range we touch
        for(auto& s : v) {
            if(s.a <= k && k <= s.b) {
                auto z = clip(k, s);
                w.insert(w.end(), z.begin(), z.end());
            } else {
                w.push_back(s);
            }
        }
        v.clear();

        // Make any singleton range odd
        for(auto& s : w)
            if(s.b-s.a == 0)
                s.odd = true;

        // Grow the ranges
        for(auto& s : w)
            if(s.b >= s.a)
                v.push_back({s.a-1, s.b+1, s.odd});
        w.clear();

        // Clip the ranges to the ends (make the odd ranges bounce)
        for(auto& s : v) {
            if(!s.odd) {
                s.a = std::max(s.a, 1);
                s.b = std::min(s.b, n);
                w.push_back(s);
            } else {
                if(s.a < 1)
                    s.a += 2;
                if(s.b > n)
                    s.b -= 2;
                w.push_back(s);
            }
        }
        v.clear();

        // Oddify the singletones
        for(auto& s : w)
            if(s.b-s.a == 0)
                s.odd = true;

        // Merge ranges
        if(w.size() > 1) {
            v = { w[0] };
            for(int i = 1; i < w.size(); i++) {
                auto z = merge(v.back(), w[i]);
                v.pop_back();
                v.insert(v.end(), z.begin(), z.end());
            }
        } else
            v = w;
    }

    // If after all that, there are any ranges left ..
    printf(v.size() ? "NO" : "YES");
}
