/* 2116. He is not a knight for you - http://acm.timus.ru/problem.aspx?num=2116
 *
 * Strategy:
 * For each unvisited square of a small board (like 10x10), let a knight do a dfs of its domain and
 * mark all reached squares visited and increment the answer.
 *
 * Performance:
 * O(1), runs the tests in 0.015s using 172KB memory.
 */

#include <stdio.h>
#include <array>

bool V[10][10];
int N, A, B, ans;

void dfs(int x, int y) {
    V[x][y] = true;
    for(auto dx : { A, B })
        for(int px : { -1, 1 })
            for(int py : { -1, 1 })
                if(auto x2=x+dx*px, y2=y+(A^dx^B)*py; x2>=0 && x2<N && y2>=0 && y2<N && !V[x2][y2])
                    dfs(x2, y2);
}

int main() {
    scanf("%d %d %d", &N, &A, &B);
    if(A == 0 && B == 0) // Only special case
        return printf("%lld", (long long)N*(long long)N), 0;

    N = std::min(N, 10);
    for(int x = 0; x < N; x++) {
        for(int y = 0; y < N; y++) {
            if(!V[x][y])
                ans++;
            dfs(x, y);
        }
    }
    printf("%d", ans);
}
