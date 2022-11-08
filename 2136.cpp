/* 2136. John vs Santa - http://acm.timus.ru/problem.aspx?num=2136
 *
 * Strategy:
 * Hug the right wall, by iterating over the height of the chimney, while at each step doing a
 * range-minimum query over the height of Santa to find the most protruding spot on the right
 * and on the left.
 *
 * Performance:
 * O(hlog h), runs the tests in 0.125s using 2,632KB memory.
 */

#include <stdio.h>
#include <vector>

const int inf = 2000000000;

// Range-minimum-query structure - we could just have used a C++ multiset instead for a more elegant
// solution but that's a bit slower
struct Rmq {
    std::vector<int> A;
    int size;

    // Recursive helper function for query below
    int query(int l, int r, int i, int left, int right) {
        if(l <= left && r >= right)
            return A[i];
        int maxleft = 0, maxright = 0, mid = (left+right)/2;
        if(l <= mid)
            maxleft = query(l, r, i*2, left, mid);
        if(r >= mid + 1)
            maxright = query(l, r, i*2+1, mid + 1, right);
        return std::max(maxleft, maxright);
    }

    // Initializes the Rmq from an array
    Rmq(std::vector<int>& input) {
        int vSize = 2;
        for(int s = input.size()-1; s; s >>= 1)
            vSize *= 2;
        A = std::vector<int>(vSize, 0);
        std::copy(input.begin(), input.end(), A.begin() + A.size()/2);
        for(int i = A.size()/2 - 1; i; i--)
            A[i] = std::max(A[i*2], A[i*2+1]);
    }

    // Queries for the minimum between two indices
    int query(int l, int r) {
        return query(l+1, r+1, 1, 1, A.size()/2);
    }
};

int main() {
    int h, w, a, b, ans = inf;
    scanf("%d %d %d %d", &h, &w, &a, &b);

    std::vector<int> l, r;

    for(int i = 0; i < h; i++) {
        int li, ri;
        scanf("%d %d", &li, &ri);
        l.push_back(li);
        r.push_back(ri);
    }
    Rmq L(l), R(r);

    for(int i = 0; i < h; i++) {
        // On the left we query one step retrospectively, and on the right one step ahead, since
        // we have to move up to and down from the protrusions before and after them
        auto maxl = L.query(std::max(0, i-a+1), i);
        auto maxr = R.query(std::max(0, i-a), std::min(i+1, h-1));

        // The remaining space after the protrusions and santa
        ans = std::min(ans, 1 + w - maxl - maxr - b);
    }
    printf("%d", std::max(ans, 0));
}
