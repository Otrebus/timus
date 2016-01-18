/* 1069. Prufer Code - http://acm.timus.ru/problem.aspx?num=1069
 *
 * Strategy:
 * For any Prufer code (input), the leaves of the graph that corresponds to that input are exactly
 * those vertices that are not part of the input. We know that the first vertex of this input is
 * attached to the smallest leaf, so we make that connection which corresponds to a branch of the
 * tree. The rest of the input is now the Prufer code for the same graph excluding the leaf, so we
 * can proceed recursively, always keeping track of the current leaves of the graph that represents
 * the remaining Prufer code, together with the branches that we have constructed so far. 
 * This is carried out iteratively in practice, using a priority queue to keep track of the leaves, 
 * and using a frequency table for each vertex to figure out when a vertex becomes a leaf (is not 
 * mentioned in the remainder of the input).
 *
 * Performance:
 * O(n log n), runs the test cases in 0.031s using 696 KB memory.
 */

#include <stdio.h>
#include <queue>
#include <functional>
#include <algorithm>

const int maxn = 7500;

int input[maxn-1];
int freq[maxn+1]; // Frequency of each vertex in the (remaining) input
std::vector<int> v[maxn+1]; // Edge lists

int main()
{
    int N = 0, x;
    std::priority_queue<int, std::vector<int>, std::greater<int>> q; // The leaves as far as we know
    while(scanf("%d", &x) != EOF)
        freq[input[N++] = x]++;
    for(int i = 1; i <= N; i++) // Initialize the leaves
        if(!freq[i])
            q.push(i);
    for(int i = 0; i < N; i++)
    {
        int x = input[i];
        v[q.top()].push_back(x); // Attach this vertex to the smallest leaf and vice versa
        v[x].push_back(q.top());
        q.pop();
        if(freq[x]) // This vertex is mentioned one less time
            freq[x]--;
        if(!freq[x]) // This vertex is never mentioned again, becomes a leaf
            q.push(x);
    }
    for(int i = 1; i <= N+1; i++)
    {
        printf("%d: ", i);
        std::sort(v[i].begin(), v[i].end());
        for(auto c : v[i])
            printf("%d ", c);
        printf("\n");
    }
}