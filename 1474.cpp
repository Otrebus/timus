/* 1474. About Frogs - http://acm.timus.ru/problem.aspx?num=1474
 *
 * Strategy:
 * The general pattern of movement was found using a brute-force program (included below) and is
 * written out by the given program. The pattern looks like this (look at the empty space):
 *
 *    Frogs     Move
 *   --------
 *   www bbb
 *   ww wbbb     2
 *   wwbw bb     4
 *   wwbwb b     5
 *   wwb bwb     3
 *   w bwbwb     1
 *    wbwbwb     0
 *   bw wbwb     2
 *   bwbw wb     4
 *   bwbwbw      6
 *   bwbwb w     5
 *   bwb bww     3
 *   b bwbww     1
 *   bb wbww     2
 *   bbbw ww     4
 *   bbb www     3
 *
 * Performance:
 * O(N^2), runs the tests in 0.046s using 2,200KB memory.
 */

#include <stdio.h>
#include <vector>

int main()
{
    int N;
    scanf("%d", &N);
    std::vector<int> v;
    int d = -1, w = 0, s = 1, p = N;
    while(true)
    {
        for(int j = 0; j < w && p + d*2 >= 0 && p + d*2 < N*2+1; j++)
        {
            p += d*2;
            v.push_back(p);
        }
        if(p == 0 || p == N*2)
        {
            p -= d;
            if(s == 1)
                s = -1;
        }
        else if(w || s > 0)
            p += d*s;
        else
            break;
        v.push_back(p);
        d *= -1;
        w += s;
    }
    printf("%d\n", v.size());
    for(auto n : v)
        printf("%d ", n);
}

/*

bool isValid(char* a, int i, int j)
{
    if(i < 0 || a[i] != ' ' || i == j)
        return false;    
    if(a[j] == 'b')
        return j == i + 1 || j == i+2 && a[j-1] == 'w';
    else if(a[j] == 'w')
        return j == i - 1 || j == i-2 && a[j+1] == 'b';
}

// Frogs as input string, middle position, full length, #frogs in right position, solution output
bool solve(char* a, int i, int N, int done, std::stack<int>& solution)
{
    if(done == N-1)
        return true;
    for(int j = std::max(0, i-2); j <= std::min(N-1, i+2); j++)
    {
        if(i != j && isValid(a, i, j))
        {
            std::swap(a[i], a[j]);
            auto b = solve(a, j, N, done + (i < j && i < N/2 && j >= N/2 
                                           || i > j && j <= N/2 && i > N/2), solution);
            std::swap(a[i], a[j]);
            if(b)
            {
                solution.push(j);
                return true;
            }
        }
    }
    return false;
}
*/