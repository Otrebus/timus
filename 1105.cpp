/* 1105. Observers Coloring - http://acm.timus.ru/problem.aspx?num=1105
 *
 * Strategy:
 * Filter the input so there are only at most two overlapping segments, and where there is at
 * least one segment covering any part of the time line. Then, we have three subsequences of
 * segments that we can use as candidate sequences: every segment with an odd index, every segment
 * with an even index, and every segment. If either of the two former sequences have a total length
 * of more than 2/3 of the total length we can use that one as an answer since the segments are
 * guaranteed not to overlap. If they both have less than 2/3 of the total length, the sum of
 * the two (all the segments in the filtered sequence) will be less than 4/3 of the total length
 * which means they overlap at a maximum of 1/3 of the sequence which means the remaining >=2/3
 * have a single segment covering them, leading to our answer.
 *
 * Performance:
 * O(N log N), Runs the tests in 0.031s using 652KB memory.
 */

#include <iostream>
#include <algorithm>
#include <vector>
#include <numeric>

struct Segment {
    int i;
    double l, r;
};

Segment A[10000];

int output(std::vector<Segment>& ans) {

    std::cout << ans.size() << std::endl;
    for(auto& x : ans) {
        std::cout << (x.i + 1) << std::endl;
    }
    return 0;
}

int main() {
    double T0, T1;
    int N;
    std::cin >> T0 >> T1 >> N;

    for(int i = 0; i < N; i++) {
        A[i].i = i;
        std::cin >> A[i].l >> A[i].r;
    }

    // Sort the input by leftmost starting point
    std::sort(A, A + N, [](Segment& a, Segment& b) { return a.l == b.l ? a.r < b.r : a.l < b.l; });
    std::vector<Segment> B;

    int jmax = 0;
    B.push_back(A[0]);
    // Filter the input into a sequence B which is minimally overlapping. The last added segment to
    // that sequence has index i, and we iterate through the sorted segments to maintain the segment
    // that starts within segment i and extends furthest to the right as segment jmax. Whenever we
    // find a segment that starts outside i, we add jmax to our sequence and name it our new i.
    for(int i = 0, j = 1; j < N; j++) {
        if(A[j].l > A[i].r)
            B.push_back(A[i=j=jmax]);
        if (A[j].r > A[jmax].r)
            jmax = j;
        if(j == N-1 && jmax != i)
            B.push_back(A[jmax]);
    }

    std::vector<Segment> ans[3]; // Our candidate sequences
    for(int i = 0; i < B.size(); i++) {
        ans[i%2].push_back(B[i]);
        ans[2].push_back(B[i]);
    }

    for(auto v : ans) {
        auto f = [](double a, Segment& b) { return a + b.r - b.l; };
        double sum = std::accumulate(v.begin(), v.end(), 0.0, f);

        if(sum > 2*(T1-T0)/3)
            return output(v);
    }
}
