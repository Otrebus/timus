/* 1500. Pass Licenses - http://acm.timus.ru/problem.aspx?num=1500
 *
 * Strategy:
 * For each of the 2^K different combinations of licenses, do normal depth-first search to see if
 * the combination provides an admissible route and then report a minimal of all those combinations.
 * To speed things up, partition the combinations by the number of licenses required and do binary
 * search.
 *
 * Performance:
 * O(N^2*2^K), runs the tests in 0.171s using 4,992KB memory.
 */

#include <stdio.h>
#include <vector>

const int maxn = 30, maxk = 20;

struct Node
{
    int visited;
    std::vector<Node*> v;
} nodes[maxn];

std::vector<int> sc[maxk+1]; // sc[i] contains all the bitmasks that have i bits set
int A[maxn][maxn]; // Adjacency matrix containing the admissible bits between two nodes

bool dfs(Node* node, int mask)
{
    if(node == &nodes[1])
        return true;
    for(auto n : node->v)
        if(n->visited == mask || !(A[node-nodes][n-nodes] & mask))
            continue;
        else if(dfs(n, n->visited = mask))
            return true;
    return false;
}

int bitcount(unsigned int i)
{
    int count = (i >> 1) & 033333333333; 
    count = i - count - ((count >> 1) & 033333333333);
    return (count + (count >> 3) & 030707070707) % 077;
} 

int main()
{
    int K, N, M, x, y, c;
    scanf("%d %d %d", &K, &N, &M);
    while(M--)
    {
        scanf("%d %d %d", &x, &y, &c);
        if(!A[x][y])
        {
            nodes[x].v.push_back(&nodes[y]);
            nodes[y].v.push_back(&nodes[x]);
        }
        A[x][y] = A[y][x] |= (1 << c);
    }

    for(int mask = 0; mask < (1 << K); mask++)
        sc[bitcount(mask)].push_back(mask);

    // Binary search over the number of bits set
    int left = 1, right = K, bestmask;
    while (right >= left) 
    {
        int mid = (left + right)/2;
        bool found = false;
        std::vector<int>& v = sc[mid];

        // Loop over all bitmasks with this number set
        for(int i = 0; i < v.size() && !found; i++)
            if(dfs(&nodes[0], v[i]))
                found = bestmask = v[i];
        if (found)
            right = mid - 1;
        else
            left = mid + 1;
    }
    printf("%d\n", bitcount(bestmask));
    for(int i = 0; i < maxk; i++)
        if(bestmask >> i & 1)
            printf("%d ", i);
}