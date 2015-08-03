/* 1901. Space Elevators - http://acm.timus.ru/problem.aspx?num=1901
 *
 * Strategy:
 * Sort the list and keep comparing the largest and smallest numbers. If they exceed s, add them to
 * the beginning of the solution sequence, otherwise add the smallest number last. Calculate
 * the number of trips in constant time by using the fact that all smallest numbers are pairwise
 * lower to or equal than s (match), since they were deemed to match with larger numbers.
 *
 * Performance:
 * O(n log n), but can be improved to O(n) using radix sort; still, the solution is accepted in
 * 0.062 seconds (and uses 1044 KB) which is very fast compared to almost all other solutions.
 */

#include <stdio.h>
#include <vector>
#include <algorithm>

int main()
{
    int n, s, t = 0;
    scanf("%d %d", &n, &s);

    std::vector<int> v;
    std::vector<int> ans(n);
    v.reserve(n);

    for(int i = 0; i < n; i++)
    {
        int x;
        scanf("%d", &x);
        v.push_back(x);
    }
    std::sort(v.begin(), v.end());
    int front = 0, back = n - 1; // Input iterators
    int sfront = 0, sback = n - 1; // Output iterators

    while(front <= back)
    {
        if(v[front] + v[back] > s && front != back)
        {   // Put numbers that don't go together in the beginning, with the smallest first
            t += 2;
            ans[sfront++] = v[front++];
            ans[sfront++] = v[back--];
        }
        else
            ans[sback--] = v[front++];
    }

    // The numbers that we put in last all go together, but in the seam between the clashing and the
    // last numbers we could get an additional match:
    int l;
    if(sback < n-1 && sback >= 0 && ans[sback] + ans[sback+1] <= s)
        l = (n-1-sback)-1;
    else
        l = n-1-sback;
    t += l/2 + l%2;
    printf("%d\n", t);
    for(auto it = ans.begin(); it < ans.end(); it++)
        printf("%d ", *it);
    return 0;
}
