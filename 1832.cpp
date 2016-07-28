/* 1832. Arirang Show - http://acm.timus.ru/problem.aspx?num=1832
 *
 * Strategy:
 * Maintain an array A that for each entry encodes the currently allowed characters for that
 * position in the string as the algorithm progresses. For each position i in the output
 * string from left to right, assign the smallest allowed character to the current position
 * and then remove that character from A in positions i+d and i-d for every divisor d of n.
 *
 * Performance:
 * O(nd(n)) where d is the divisor function, runs in 0.499s (whew) using 2,596KB memory.
 */

#include <cmath>
#include <vector>
#include <algorithm>
#include <cstring>
#include <stdio.h>

unsigned int line[300001];
int n, A[300001]; // Allowed chars, as bitmask

int main()
{
    scanf("%d", &n);
    std::memset(A, 0xff, sizeof(A));
    std::vector<int> ds; // Divisors
    ds.reserve(300);
    int top = std::max((int)sqrt(n)+1,n);
    for(int i = 1; i < top; i++)
        if(n % i == 0)
            ds.push_back(i);
    for(int i = 0; i < n; i++)
    {
        line[i] = A[i] & -A[i]; // Set line[i] to the lowest set bit of A[i]
        for(auto d : ds)
        {
            A[(i+n-d)%n] &= ~line[i]; // Disallow this from the adjacent characters
            A[(i+d)%n] &= ~line[i];
        }
    }
    for(int i = 0; i < n; i++)
        putchar(std::log2(line[i])+0.001+'a');
    return 0;
}