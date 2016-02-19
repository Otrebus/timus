/* 1803. The Czechs' Rifles - http://acm.timus.ru/problem.aspx?num=1803
 *
 * Strategy:
 * Represent each price as an array of denominations, and when summing two prices together, add
 * together the amount of bills in each denomination, replacing to higher denominations whenever
 * we can. Optimize by combining the notes [1, p^1, p^2, p^3, p^4, ..., p^x ] for some
 * x into a new "superbasis" which replaces our earlier p. Precalculate a map which translates
 * how many real bills each number of bills within this superbasis correspond to.
 *
 * Performance:
 * O(n^2), runs the test cases in 0.452s using 956KB memory (rated 4/371).
 */

#include <algorithm>

std::pair<int, int> ans[50000] = { { 1, 1 }, { 1, 2 } }; // The answer array { #notes, id }

int A1[35000], A2[35000]; // The prices as represented above
int pre[100001]; // Number of real notes used for a number of notes in the "superbasis" denomination
int Ap[20]; // The number of actual bills within each real denomination used for some specific
            // number of notes in the superbasis denomination, used to calculate the above array
int l, n, k, p; // Number of denominations used for our values, the input, and the superbasis

int add(int* a, int* b) // Adds together two terms and calculates the amount of notes used
{
    int ret = 0;
    for(int i = 0; i < l; i++)
    {
        b[i] += a[i];
            
        if(b[i] >= p) // For example, we have 10 $10 notes, so replace them with a $100 note
        {
            b[i] -= p; // Remove the basis number of notes from this bracket,
            b[i+1]++;  // and add a single note to the next bigger bracket
            l = std::max(i+2, l);
        }
        ret += pre[b[i]]; // Since we're dealing with the superbasis here we need to translate the
    }                     // number of notes used in this denomination to the real world
    return ret;
}

void calcpre() // Calculates the mapping of #superbasis notes -> real notes of every denomination
{
    Ap[0] = pre[0] = 0;
    for(int i = 1; i < p; i++)
    {
        int j = 0;
        pre[i] = pre[i-1]; // Use the previous value calculated as a base
        Ap[j]++; // Add 1 to the lowest basis of notes

        while(true)
        {
            pre[i]++; // Like in add() above
            if(Ap[j] >= k)
            {
                Ap[j] -= k;
                Ap[j+1]++;
                pre[i] -= k;
                j++;
            }
            else // No real bill denomination overflows so the rest will be the same
                break;
        }
    }
}

int main()
{
    scanf("%d %d", &k, &n);
    A1[0] = A2[0] = l = 1;
    int* a = &A1[0], *b = &A2[0];
    p = k;
    while(p*k < 100000) // Form our superbasis
        p *= k;
    calcpre();
    for(int i = 2; i <= n; i++)
    {
        ans[i].first = add(a, b);
        ans[i].second = i+1;
        std::swap(a, b);
    }
    std::sort(ans, ans + n);
    for(int i = 0; i < n; i++)
        printf("%d ", ans[i].second);
}