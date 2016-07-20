/* 1772. Ski-Trails for Robots - http://acm.timus.ru/problem.aspx?num=1772
 *
 * Strategy:
 * If we let the array A denote the optimal result for the set of track after some obstacle,
 * whenever we add an obstacle, the result values next to the obstacle increase in a linear
 * fashion from the outer values towards some maximum value in the middle.
 *
 *    A_k   A_{k+1}
 *     5      5
 *     4      4
 *     3   |  5 
 *     2   |  6
 *     3   |  7
 *     4   |  6
 *     5      5
 * 
 * To quickly calculate these values, a range-update point-query structure is used, built on the
 * typical heap structure of a segment tree and which is updated similarly.
 *
 * Performance:
 * O(klog n), runs the tests in 0.14s using 4,296KB memory.
 */

#include <stdio.h>
#include <vector>
#include <algorithm>

class SegmentTree
{
    // The first entry, in { -1, 1 } denotes an increasing or decreasing sequence of the width of
    // this node and the second entry denotes the leftmost value of the range
    std::vector<std::pair<char, long long>> A;
    int size;

public:
    SegmentTree(int n = 1) : size(n)
    {
        int pow2 = 1;
        while((pow2*=2) < n);
        pow2 *= 2;
        A.resize(pow2);
    }

    template<int d> void set(int node, int left, int right, int i, int j, long long n)
    {
        // The update range is within the range that this nodes represents
        if(i <= left && j >= right)
        {
            A[node] = { d, n + d*(left-i) };
            return;
        }
        int mid = (right+left)/2;
        // We update a subset of the range of this node so we need to mark this node as
        // non-representative and send its value to the children instead
        if(A[node].second >= 0)
        {
            A[node*2] = A[node];
            A[node*2+1] = A[node];
            A[node*2+1].second += A[node].first*(1+mid-left);
        }
        A[node].second = -1;

        // Recurse to the children and update their ranges
        if(j > mid)
            set<d>(node*2+1, mid+1, right, i, j, n);
        if(i <= mid)
            set<d>(node*2, left, mid, i, j, n);
    }

    long long query(int node, int left, int right, int i)
    {
        if(i >= left && i <= right && A[node].second >= 0)
            return A[node].second + A[node].first*(i-left);
        int mid = (right+left)/2;
        if(i > mid)
            return query(node*2+1, mid+1, right, i);
        else if(i <= mid)
            return query(node*2, left, mid, i); 
    }

    long long query(int i)
    {
        return query(1, 1, A.size()/2, i);
    }

    template<int dir> void set(int i, int j, long long n)
    {
        set<dir> (1, 1, A.size()/2, i, j, n );
    }
};

int main()
{
    long long n, s, k, l, r;
    scanf("%lld %lld %lld", &n, &s, &k);
    SegmentTree st(n);
    st.set<-1>(1, s, s-1);
    st.set<1>(s, n, 0);

    while(k--)
    {
        scanf("%lld %lld", &l, &r);
        // We update the range as shown in the header comment
        long long a = l > 1 ? st.query(l-1) : -1, b = r < n ? st.query(r+1) : -1;
        long long t = (b-a+l+r)/2;
        if(l > 1 && r < n)
        {
            st.set<1>(l-1, t, a);
            st.set<-1>(t, r+1, a+t-l+1);
        }
        else if(l == 1)
            st.set<-1>(l, r, r+1-l+b);
        else if(r == n)
            st.set<1>(l, r, a+1);
    }
    long long min = std::numeric_limits<long long>::max();
    for(int i = 1; i <= n; i++)
        min = std::min(min, st.query(i));
    printf("%lld", min);
}