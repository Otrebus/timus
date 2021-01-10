/* 2135. Pauline and commits - http://acm.timus.ru/problem.aspx?num=2135
 *
 * Strategy:
 * Dynamic programming; let C[k] be the solution to the problem for k < n, calculate C[k+1] by
 * "growing" a commit that extends back to C[k-i] for i's >= 0 and choosing the commit that
 * minimizes the total number of commits if i allows for a commit in the given range.
 * 
 * Performance:
 * O(n log r), runs the tests in 0.062 using 724KB.
 */

#include <stdio.h>
#include <vector>
#include <cstring>

unsigned int C[50002];

int main() {
    long long l, r;
    int x, n;
    std::vector<int> v(2, 1);
    std::memset(C, 0x0F, sizeof(C));
    C[0] = 0;

    scanf("%d %lld %lld", &n, &l, &r);

    while(n--) {
        scanf("%d", &x);
        if(x > 1)
            v.push_back(x);
    }

    for(int i = 0; i < v.size(); i++)
        for(unsigned long long j = i, d = 1; v[j]*d <= r && j && (v[j]*d)/v[j] == d; d *= v[j--])
            C[i] = l <= v[j]*d ? std::min(C[i], C[j-1] + 1) : C[i];

    int ans = C[v.size()-1];
    printf("%d", (ans <= 50000 ? ans : -1));
}
