/* 1181. Cutting a Painted Polygon - http://acm.timus.ru/problem.aspx?num=1181
 *
 * Strategy:
 * First, let's prove by induction over the amount of vertices that any graph with the given
 * properties is cuttable into triangles. Let the base case be for n = 3; this is a triangle
 * and vacuously cuttable. Let n = k and assume that all convex polygons with size < k are
 * cuttable. If there is only one vertex of some given color, we can simply draw one cutting line
 * from this vertex to all other vertices. Since all other vertex colors must be alternating,
 * we can form N-3 triangles this way. Otherwise, if all colors have at least two vertices,
 * there is, at some edge, three consecutive vertices each of different colors (otherwise
 * the vertices would be of strictly alternating colors and no third color would be represented).
 * If we cut off the triangle formed by these three consecutive vertices, we are left with
 * the remaining convex polygon of n-1 vertices that has all colors represented, and that
 * has no two consecutive vertices of the same color, which is cuttable by the induction
 * assumption, so the statement is true for n = k and all sizes n >= 3 by induction.
 *  This is a constructive proof so we can simply iteratively perform the induction step:
 * until there is only one vertex left of some color, we find an ear to cut off.
 *  There is also a dynamic programming solution down below, which is of higher complexity.
 *
 * Performance:
 * O(N), running the tests in 0.015s using 212KB memory.
 */

#define solution1
#ifdef solution1

#include <stdio.h>

char c[1001];
int d[1000];
int count[256];

int main()
{
    int N;
    scanf("%d %s", &N, c);
    printf("%d\n", N-3);

    for(int i = 0; i < N; i++)
         count[c[i]]++;
    int p = 0, ans = 0;
    for(int i = 0; i < N; i++, p++)
    {
        d[p] = i;
        while(p >= 3 && c[d[p]] != c[d[p-1]] && c[d[p-1]] != c[d[p-2]] 
                     && c[d[p-2]] != c[d[p]] && count[c[d[p-1]]] > 1)
        {
            count[c[d[p-1]]]--;
            d[p-1] = d[p];
            printf("%d %d\n", d[p-2]+1, d[p]+1);
            p--, ans++;
        }
    }
    int pos;
    for(int i = 0; i < p; i++)
        if(count[c[d[i]]] == 1)
            pos = i;
    for(int i = 0; ans < N - 3; i++)
        if((N + d[i]-d[pos]) % N > 1)
            printf("%d %d\n", d[i]+1, d[pos]+1), ans++;
}

#elif solution2

#include <stdio.h>
#include <vector>
#include <algorithm>

char c[1001];
int N;
int split[1001][1001];

inline bool unequal(int i, int j, int k)
{
    return c[i] != c[j] && c[j] != c[k] && c[i] != c[k];
}

bool solve(int i, int j)
{
    if((N + j - i) % N == 2)
        return(unequal(i, (i+1) % N, j));
    if(split[i][j] == -1)
        return false;
    else if(split[i][j])
        return true;

    int t = i == j ? 1 : 0;
    for(int k = (i + 2)%N; (k+t)%N != j; k = (k+1)%N)
    {
        if((i == j || unequal(i, k, j)) && solve(i, k) && ((k + 1) % N == j || solve(k, j)))
        {
            split[i][j] = k;
            return true;
        }
    }
    if(N + j - i % N >= 3 && unequal(i, (i+1)%N, j) && solve((i+1)%N, j))
    {
        split[i][j] = (i+1) % N;
        return true;
    }
    split[i][j] = -1;
    return false;
}

void construct(int i, int j, std::vector<std::pair<int, int>>& ans)
{
    int k = split[i][j];
    if(!k)
        return;
    if((N + k - i) % N == 1)
    {
        ans.push_back( { (i+1)%N, j} );
        construct((i+1)%N, j, ans);
        return;
    }
    else if(i != j)
    {
        ans.push_back( { i, k } );
        if((k+1) % N != j)
            ans.push_back( { k, j } );
    }
    else
        ans.push_back( { i, k } );
    construct(i, k, ans);
    construct(k, j, ans);
}

std::vector<std::pair<int, int>> solve(int N)
{
    ::N = N;
    if(solve(0, 0))
    {
        std::vector<std::pair<int, int>> ans;
        construct(0, 0, ans);
        return ans;
    }
    else if(solve(1, 1))
    {
        std::vector<std::pair<int, int>> ans;
        construct(1, 1, ans);
        return ans;
    }
    return std::vector<std::pair<int, int>>();
}

int main()
{
    int N;
    scanf("%d", &N);
    scanf("%s", c);
    auto v = solve(N);
    printf("%d\n", v.size());
    for(auto& p : v)
        printf("%d %d\n", p.first+1, p.second+1);
}

#endif