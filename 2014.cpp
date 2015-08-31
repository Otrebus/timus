/* 2014. Zhenya moves from parents. - http://acm.timus.ru/problem.aspx?num=2014
 *
 * Strategy:
 * When putting the withdrawals and deposits into a graph, it can be seen that the total amount of
 * debt accrued on the father is the minimum value below 0 on the graph. To quickly update and
 * retrieve such a minimum value, an RMQ structure is created whose associative operation in effect
 * combines the sums and minimum values of two subgraphs to quickly determine the sum and minimum
 * values of these combined two subgraphs.
 *
 * Performance:
 * O(n log n), running the tests in 0.577s (an average time) using 5612KB memory.
 */

#include <tuple>
#include <algorithm>
#include <vector>
#include <unordered_set>
#include <string>

struct Entry
{
    long long sum, min;
    Entry() : sum(0), min(0) { }
    Entry(long long sum, long long min) : sum(sum), min(min) { }
    Entry operator+ (const Entry& rhs)
    {
        return Entry(sum + rhs.sum, std::min(min, sum+rhs.min)); // The associative operation
    }
};

std::vector<Entry> A;
int size = 2, height = 0;
std::vector<int> input, times, vs;

void init(int s)
{
    for(--s; s; s >>= 1)
        size *= 2; // Total size of the complete heap needed to contain the values in its leafs
    A = std::vector<Entry>(size);
    while(size >>= 1)
        height++;
}

void update(int i, int val)
{
    A[i] = Entry(val, val);
    while(i >>= 1)
        A[i] = A[i*2] + A[i*2+1]; // Update the parent node of these two subnodes
}

int main()
{
    int day, month, hour, minute, N, v;
    scanf("%d", &N);
    init(N);
    input.reserve(N); times.reserve(N); vs.reserve(N);
    for(int i = 0; i < N; i++)
    {   // We map the input dates to a linear input array
        scanf("%d %d.%d %d:%d", &v, &day, &month, &hour, &minute);
        input.push_back((((month*31 + day)*24 + hour)*60 + minute));
        times.push_back(input.back());
        vs.push_back(v);
    }
    std::sort(times.begin(), times.end());
    for(int i = 0; i < N; i++)
    {   // Find the actual position in the RMQ array of the given date
        int pos = std::lower_bound(times.begin(), times.end(), input[i]) - times.begin();
        update(pos + (1 << (height - 1)), vs[i]);
        printf("%lld\n", std::min(A[1].min, 0ll));
    }
    return 0;
}