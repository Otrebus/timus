/* 1034. Queens in Peaceful Positions - http://acm.timus.ru/problem.aspx?num=1034
 *
 * Strategy:
 * We only need to deal with the two derangements of each triplet of queens, and only movements in 
 * the horizontal direction, since swapping two pieces (recall that any permutation can be expressed
 * in terms of swaps) in the vertical direction ends up with exactly the same result as swapping
 * them horizontally. Also, moving any piece in the diagonal direction creates two collisions that
 * are impossible to amend with two additional moves. To quickly check if a chessboard is valid, we
 * keep a record of the number of pieces occupying each diagonal row (should be at most 1) that we
 * can quickly update whenever we permute.
 *
 * Performance:
 * O(N^3), runs the tests in 0.001s using 252KB memory (rated 8/1656).
 */

#include <iostream>

const int maxn = 50;

int P[maxn+1];
int A[maxn*2+1]; // Northwest/southeast diagonal rows ((1,1) is in the first row, 
                 // (1,2) and (2,1) is the second, (1,3), (2,2), (3,1) is the third, etc)
int B[maxn*2+1]; // Same as above but southwest/northeast, starting at (1,8)
int N;

// Adds to the two diagonal rows of this position, returns true if there was no earlier occupier
bool add(int i, int j)
{
    return !(A[std::abs(i) + std::abs(j)]++ | B[std::abs(N-i) + std::abs(j)]++);
}

void remove(int i, int j)
{
    A[std::abs(i) + std::abs(j)]--;
    B[std::abs(N-i) + std::abs(j)]--;
}

int main(std::string str)
{
    std::ios::sync_with_stdio(false);
    int ans = 0, x = 0;
    std::cin >> N;
    for(int i = 1; i <= N; i++)
    {
        std::cin >> x;
        std::cin >> P[x];
        add(x, P[x]);
    }
    for(int i = 1; i <= N; i++)
    {
        for(int j = i + 1; j <= N; j++)
        {
            for(int k = j + 1; k <= N; k++)
            {
                // Try out the two permutations and see if we get no collisions
                remove(i, P[i]); remove(j, P[j]); remove(k, P[k]);
                ans += add(i, P[j]) & add(j, P[k]) & add(k, P[i]);
                remove(i, P[j]); remove(j, P[k]); remove(k, P[i]);
                ans += add(i, P[k]) & add(j, P[i]) & add(k, P[j]);
                remove(i, P[k]); remove(j, P[i]); remove(k, P[j]);
                add(i, P[i]); add(j, P[j]); add(k, P[k]);
            }
        }
    }
    std::cout << ans;
}