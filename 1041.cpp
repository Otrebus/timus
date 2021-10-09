/* 1041. Nikifor - http://acm.timus.ru/problem.aspx?num=1041
 *
 * Strategy:
 * Reduce the matrix to echelon form in order to find sets of dependent vectors. For each such set,
 * pick the s cheapest ones where s is the number of pivot points of those vectors in the matrix.
 *
 * Performance:
 * O(MN^2), runs the tests in 0.031s using 976KB memory.
 */

#include <stdio.h>
#include <algorithm>
#include <vector>
#include <unordered_set>
#include <set>

using ll = long long;

ll A[50][2000]; // The matrix
int C[2000];    // The costs
bool e[2000];   // Was column picked already?
int M, N;       // Matrix dimensions
const ll P = 93983; // We do operations modulo this

bool bring(int r, int c) { // Brings up a pivot row to position (r, c)
    for(int i = r+1; i < N; i++) {
        if(A[i][r]) {
            for(int j = 0; j < M; j++)
                std::swap(A[i][j], A[r][j]);
            return true;
        }
    }
    return false;
}

void reduce(int i, int r, int p) {
    // Reduces rows below the pivot
    if(!A[i][p])
        return;
    auto b = A[i][p], a = A[r][p];
    for(int j = 0; j < M; j++)
        // To combat floating point errors, we do this instead of dividing to 1 and then multiply
        A[i][j] = ((A[i][j]*a)%P - (b*A[r][j])%P)%P;
}

int main() {
    std::set<int> p;
    scanf("%d %d", &M, &N);
    for(int i = 0; i < M; i++)
        for(int j = 0; j < N; j++)
            scanf("%d", &A[j][i]);
    for(int i = 0; i < M; i++)
        scanf("%d", &C[i]);
    
    int r = 0, c = 0, pr = 0;
    std::vector<int> w;

    // Bring the matrix to echelon form
    while(r < N && c < M) {
        if(!A[r][c]) {
            if(!bring(r, c)) { // Try bring up some pivot row, otherwise we can't be pivot
                c++;
                continue;
            }
        }

        for(int i = 0; i < N; i++) // Reduce everything below the pivot
            if(i != r)
                reduce(i, r, c);
        r++, c++;
    }

    // Find linearly dependant sets of vectors
    for(int r = N-1, c = M-1; r >= 0; r--) {
        for(; c >= 0; c--) {
            std::unordered_set<int> s, o;
            auto add = [&s] (int a) { if(!e[a]) e[a] = true, s.insert(a); }; // Adds to the ldep set
            add(c);

            for(int j = r; j >= 0; j--)
                if(A[j][c]) // Nonzero element
                    for(int a = c; a >= 0; a--) // Any column that has a nonzero cell on this
                        if(A[j][a])             // row will be dependant with this vector
                            add(a), o.insert(j);

            // We have o rows and s columns of the independent vectors, we pick cheapest o of them
            std::vector<int> v(s.begin(), s.end());
            std::sort(v.begin(), v.end(), [] (int a, int b) {
                return C[a] == C[b] ? a < b : C[a] < C[b];
            });
            if(v.size())
                p.insert(v.begin(), v.begin() + o.size());
        }
    }

    if(p.size() < N)
        printf("0");
    else {
        int s = 0;
        for(auto a : p)
            s += C[a];
        printf("%d\n", s);
        for(auto a : p)
            printf("%d\n", a+1);
    }
}
