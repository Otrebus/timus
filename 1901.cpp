/* 1901. Space Elevators - http://acm.timus.ru/problem.aspx?num=1901
 *
 * Strategy:
 * Sort the list and greedily choose the largest and smallest values to go together. This works
 * since if there is another value compatible with the largest, this one can still be paired up
 * with any other value.
 *
 * Performance:
 * O(n log n). This is not optimal since we can radix sort the numbers. I can't test the
 * performance of this algorithm since Timus is down.
 */

#include <stdio.h>
#include <vector>
#include <algorithm>

int main()
{
    int n, s;
    scanf("%d %d", &n, &s);
    std::vector<int> ans;
    std::vector<int> v; // Input
    ans.reserve(n);
    v.reserve(n);
    while(n--)
    {
        int x;
        scanf("%d", &x);
        v.push_back(x);
    }
    int t = 0;
    std::sort(v.begin(), v.end());
    int front = 0;
    int back = v.size() - 1;
    while(back >= front)
    {
        t++;
        if(v[front] + v[back] <= s && back > front)
        {
            ans.push_back(v[front++]);
            ans.push_back(v[back--]);
        }
        else
            ans.push_back(v[back--]);
    }
    printf("%d\n", ans.size());
    for(auto it = ans.begin(); it < ans.end(); it++)
        printf("%d ", *it);
}
