/* 2162. Graphmania - http://acm.timus.ru/problem.aspx?num=2162
 *
 * Strategy:
 * The salient part of the problem is: for a range of integers (we can map each edge - integer pair
 * into some integer), check if the range of integers only consists of unique integers, which is
 * the same as checking that the number of unique integers is the same as the length of the range.
 * To solve this problem we order the queries that we need to make (one for each occurrence of M)
 * by its right endpoint. As the right endpoints progress, we continuously keep track of the
 * rightmost occurrence of each digit and have this index equal to 1 in a Fenwick tree. This
 * allows us to quickly count the number of unique digits given some left endpoint from the current
 * right endpoint of a range. Together with some additional basic logic we can then scan the entire
 * input for valid graph representations.
 *
 * Performance:
 * O(Klog K), runs the tests in 0.078s using 14,884KB memory.
 */

#include <unordered_map>
#include <tuple>
#include <vector>
#include <algorithm>

const int maxk = 100001;

struct hash {
    size_t operator()(const std::pair<int, int>& p) const {
        return p.second*1000000 + p.first;
    }
};

std::unordered_map<std::pair<int, int>, int, hash> M; // Maps edges to integers
int A[maxk], K, m = 1; // The input
int R[maxk]; // Keeps track the rightmost occurrences
int B[maxk]; // The Fenwick tree to counts unique digits
int S[17][maxk]; // Sparse table, helps us quickly calculate the maximum in a range
const int inf = 1e8;

int max(int l, int r) { // Uses the sparse table to return the maximum in [l, r]
    int x = 1+r-l, i = 0;
    while (x >>= 1)
        i++;
    return std::max(S[i][l], S[i][1+r-(1<<i)]);
}

int sum(int i) { // Fenwick tree sum
    int sum = 0;
    for(i++; i > 0; i -= i & -i) 
        sum += B[i];
    return sum;
}

void add(int i, int k) { // Fenwick tree add
    for (i++; i <= K; i += i & -i) 
        B[i] += k;
}

int count(int a, int b) { // Fenwick tree range sum
    return sum(b) - sum(a-1);
}

struct query { // Represents a range of digits that represents edges to count uniqueness of
    int l, r;
    bool operator<(const query& q2) {
        return r < q2.r;
    }
};

int get(int i) { // Returns the integer representation of the edge at A[i]
    auto a = A[i], b = A[i+1];
    if(a == 0 || b == 0 || a == b) // Some edges are not valid (we use 0 as that representation)
        return 0;
    if(a > b)
        std::swap(a, b);
    return M[{ a, b }];
}

void insert(int a, int b) { // Creates a new integer representation of an edge
    if(a > b)
        std::swap(a, b);
    if(auto edge = std::make_pair(a, b); M.find(edge) == M.end())
        M[edge] = m++;
}

int solve(int s) { // Counts the number of solutions, starting at an odd or even digit
    int res = 0;

    std::vector<query> Q;

    std::memset(R, 0, sizeof(R));
    std::memset(B, 0, sizeof(B));

    // Add all the edge lists as a query if they fit
    for(int i = s; i + 1 < K; i += 2) {
        int N = A[i], M = A[i+1];
        if(i+1+2*M < K)
            Q.push_back({ i+2, i+2+2*M });
    }
    std::sort(Q.begin(), Q.end()); // Sort by their right endpoints

    int r = s;
    for(auto q : Q) {
        while(r < q.r) {
            auto p = get(r);
            if(R[p]) // If we've encountered this edge before, erase its last rightmost encounter
                add(R[p], !p ? -inf : -1); // (and if invalid, add a poison number to nuke the algo)
            R[p] = r; // Add this as the rightmost encounter
            add(r, !p ? inf : 1);
            r += 2; // Advance to the next edge
        }
        int N = A[q.l-2];
        // If the range is empty or the edge count jives with the range size, and the maximum vertex
        // referenced is inside the graph and we have a graph size, we have a valid graph
        if((q.l == q.r || count(q.l, q.r-1) == (q.r-q.l)/2 && max(q.l, q.r-1) <= N) && N)
            res++;
    }
    return res;
}

int main() {
    scanf("%d", &K);

    for(int i = 0; i < K; i++)
        scanf("%d", &A[i]);

    for(int i = 0; i < K-1; i += 1) // Create the edge->integer mapping
        insert(A[i], A[i+1]);

    // Initialize the sparse table
    for(int i = 0; i < K; i++)
        S[0][i] = A[i];
    for(int i = 1; (1 << i) <= K; i++)
        for(int j = 0; (j + (1 << i) - 1) < K; j++)
            S[i][j] = std::max(S[i-1][j], S[i-1][j+(1<<(i-1))]);

    // To not mix edges we iterate by 2 as we move through the array, so we loop twice with
    // different parities
    printf("%d", solve(0) + solve(1));
}
