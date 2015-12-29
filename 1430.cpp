/* 1430. Crime and Punishment - http://acm.timus.ru/problem.aspx?num=1430
 *
 * Strategy:
 * Fill the n with multiples of a until what remains mod b is minimized.
 *
 * Performance:
 * O(min(a, b, n/a, n/b) = O(sqrt(n)), runs the test cases in 0.015s using 408KB memory.
 */

#include <iostream>
#include <limits>
#include <algorithm>

int main()
{
    long long a, b, n;
    bool swapped = false;
    std::cin >> a >> b >> n;
    if(a < b)
        std::swap(a, b), swapped = true;
    int mi = 0, m = std::numeric_limits<int>::max();
    for(int i = 0; i*a <= n; i++) // We loop at most min(n/a, n/b) times
        if((n-(i*a))%b < m) // Minimize the remainder of n-a*i
            m = (n-(i*a))%b, mi = i;
        else if((i*a) % b == a%b && i > 1)     // Early termination, we won't find any more results;
            break;                             // there are only b different possible remainders to
    std::pair<int, int> p = { mi, (n-a*mi)/b };// reach here, so together with the previous comment
    if(swapped)                                // we get the complexity of the algorithm.
        std::swap(p.first, p.second);
    std::cout << p.first << " " << p.second;
    return 0;
}