/* 1606. Slalom - http://acm.timus.ru/problem.aspx?num=1606
 *
 * Strategy:
 * Bottom-up greedy dynamic programming-like solution.
 *
 * Performance:
 * Runs in 0.046s using 1,928KB memory.
 */

#include <algorithm>
#include <stdio.h>

const int maxn = 50000;

struct Solution
{
    int i;
    int length;
    Solution* prev;
} left[maxn], right[maxn];

struct Pos
{
    int x, y, original;
} poles[maxn];

int main()
{
    int N;
    scanf("%d", &N);
    for(int i = 0; i < N; i++)
        poles[i].original = i, scanf("%d %d", &poles[i].x, &poles[i].y);
    std::sort(poles, poles+N, [] (Pos a, Pos b) { return a.y < b.y; } );
   
    auto setPrev = [] (Solution& sol, Solution& prev, int i) { sol = {i, prev.length + 1, &prev}; };
    left[0].length = right[0].length = 1;
    for(int n = 1; n < N; n++)
        if(poles[n-1].x < poles[n].x)
            setPrev(left[n], right[n-1], n), right[n] = right[n-1];
        else if(poles[n-1].x > poles[n].x)
            setPrev(right[n], left[n-1], n), left[n] = left[n-1];
    Solution* s = left[N-1].length > right[N-1].length ? &left[N-1] : &right[N-1];
    printf("%d\n", s->length);
    for(s; s; s = s->prev)
        printf("%d ", (poles[s->i].original+1));
}