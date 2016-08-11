/* 1169. Pairs - http://acm.timus.ru/problem.aspx?num=1169
 *
 * Strategy:
 * The solution is always a long chain of connected cycles (additional internal edges would be
 * redundant in any cycle):
 *
 *                                         c1-c2-c3-c4
 *
 * This is because if they were ordered in a tree-like structure, 
 *
 *                                             c4
 *                                             |
 *                                          c1-c2-c3
 *
 * the process of cutting one branch of the tree and placing it anywhere else would not change
 * the number of critical pairs, so by repeating that action, we could always turn it into a chain
 * with the same number of critical pairs. Of course, if the cycles were ordered in anything
 * resembling a cycle as opposed to a tree, this larger cycle would then override any smaller cycles
 * and the critical paths between them, and would be equivalent to some tree of cycles.
 *       
 * Hence, only chains of cycles matter; the below program finds such a chain by chopping off a cycle
 * from a given problem size and then recursing while doing this in a breadth-first fashion over
 * the cycle sizes.
 *
 * Performance:
 * Hard to analyze exactly for any given n and k, but runs the tests in 0.015s using 180KB memory.
 */

#include <numeric>
#include <vector>
#include <stdio.h>

// Args: minimum size of cycle to chop off, problem size, k, size of the cycles chopped off so far
bool solve(int min, int n, int k, std::vector<int>& solution)
{
    if(k == 0 && n != 2) // Graphs of size 2 can't be cycles
        return true;
    // Attempt to chop off a cycle of size x and see if we can recurse to the problem size
    // of y = n - x, with a new k accordingly
    for(int x = min; x <= n - min; x++) // Looping above n-min would need us to find new minimum
    {                                   // cycles lower than min later, which we can't
        int y = n - x, k2 = k - x*y;
        if(x != 2 && k2 >= 0 && y*(y-1)/2 >= k2)
        {
            solution.push_back(x); // Tentatively presume this cycle is part of the solution
            if(solve(x, y, k2, solution))
                return true;
            solution.pop_back(); // Turns out this wasn't part of the solution, remove it
        }
    }
    return false;
}

int main()
{
    int n, k;
    scanf("%d %d", &n, &k);
    std::vector<int> solution; // Sizes of the cycles of the solution
    if(solve(1, n, k, solution))
    {
        // Push the size last cycle into the solution
        solution.push_back(n - std::accumulate(solution.begin(), solution.end(), 0));
        int cur = 1;
        for(int j = 0; j < solution.size(); j++)
        {
            int s = solution[j]; // Size of the current cycle
            if(s > 2) // Cycles of size 2 don't exist and cycles of size 1 don't have edges
                for(int i = 0; i < s; i++)
                    printf("%d %d\n", cur + i, cur + (i+1)%s); // Print the cycle
            cur += s;
            if(j < solution.size() - 1)
                printf("%d %d\n", cur-1, cur); // The edge to the next cycle
        }
    }
    else
        printf("-1");
}