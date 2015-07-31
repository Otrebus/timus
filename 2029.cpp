/* 2029. Towers of Hanoi Strike Back - http://acm.timus.ru/problem.aspx?num=2029
 *
 * Strategy:
 * For any given input, the reverse steps give the same result, so we can consider the problem
 * of moving the given input to all A's. For any given subproblem of size N where we have the
 * largest disk at the bottom of pin p1 and need to move all disks to pin p2, we use the answer
 * to the subproblem of size N-1 involving all disks but the largest of subproblem N, where we 
 * move all N-1 disks first to pin p3. After this we move the largest disk N to pin p2, and
 * finish by doing a normal Hanoi move of all disks on p3 to pin p2.
 *
 * Performance:
 * O(n), running the tests in 0.015s using 276KB memory.
 */

#include <stdio.h>
#include <vector>
#include <algorithm>

long long hanoi[52] = {0}; // The number of moves for the normal Hanoi puzzle

long long f(std::vector<std::vector<int>>& v, std::vector<int> i, int d)
{   // The vector i describes the indices into v where the subproblem begins
    int s, max = -1;
    for(int j = 0; j < 3; j++) // Find the biggest disk
        if(i[j] < v[j].size() && v[j][i[j]] > max)
            max = v[s=j][i[j]]; // Sneaky assignment but I'm saving so many braces
    if(max == -1)
        return 0; // Empty subproblem
    i[s]++; // Consider the problem where we remove the biggest disk
    if(d == s)
        return f(v, i, d);
    // 3-s-d is the index of the empty pin; the rest of the equation is described in the header
    return f(v, i, 3 - s - d) + hanoi[v[0].size()+v[1].size()+v[2].size()-i[0]-i[1]-i[2]] + 1;
}

int main()
{
    std::vector<std::vector<int>> v(3); // The pins
    int dummy;
    char input[51];
    scanf("%d %s", &dummy, input);

    for(int i = 0; input[i] != 0; i++)
    {
        v[input[i]-'A'].push_back(i);
        hanoi[i+1] = hanoi[i]*2+1;
    }
    for(auto it = v.begin(); it < v.end(); it++)
        std::reverse(it->begin(), it->end());
    printf("%lld\n", f(v, std::vector<int> {0, 0, 0}, 0));
    return 0;
}

// Since f is tail recursive and the sorting is given by the input, here is a much shorter but 
// somewhat less pedagogical(?) solution:

long long pow2[51] = {1};

int bettershorterfastermain()
{
    char inp[51], *input = inp; int len; long long ans = 0;
    scanf("%d %s", &len, inp);
    for(int i = 1; i < len; i++)
        pow2[i] = pow2[i-1]*2;
    for(int d = 0; len; len--)
    {
        int s = *(input+len-1) - 'A';
        if(d == s)
            continue;
        ans += pow2[len-1];
        d = 3-d-s;
    }
    printf("%lld\n", ans);
    return 0;
}