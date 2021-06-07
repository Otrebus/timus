/* 1445. Christmas gifts - http://acm.timus.ru/problem.aspx?num=1445
 *
 * Strategy:
 * The worst case happens when the members of the biggest group all come first, in which case
 * one gift for each member is required. In the best case, we keep exchange presents between
 * the two current biggest remaining groups (we remove one person from each group each time we
 * exchange presents), in which case we end up with zero remaining members at the end unless
 * the biggest group contains more members than all other groups combined. In the latter case,
 * the result is the difference in the number of members between the initial size of the majority
 * group and the rest of the groups.
 *
 * Performance:
 * Linear in the size of the input, runs in 0.015s using 256KB memory.
 */

#include <iostream>
#include <algorithm>

int main()
{
    int N, x, max = 0, sum = 0;
    std::cin >> N;
    while(N--)
    {
        std::cin >> x;
        sum += x;
        max = std::max(x, max);
    }
    std::cout << std::max(2*max-sum, 1) << " " << max;
}