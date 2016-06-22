/* 1452. Pascal vs. C++ - http://acm.timus.ru/problem.aspx?num=1452
 *
 * Strategy:
 * Dynamic programming. Let v represent the input array that we have sorted and removed all
 * duplicates from, and let A[i][n] denote the length of the longest subsequence with the required
 * equidistance property among the elements with indices 1 through n that end with the two elements
 * with indices i and n. Then we can calculate each new entry A[k][n+1] for 1 <= k <= n+1, by doing
 * the following: for every A[k][n+1], denote d = v[n+1]-v[k]. Then, if there is an element A[i][k]
 * where v[k]-v[i] = d, we will have A[k][n+1] = 1 + A[i][k]. To find that element A[i][k] we can
 * either use binary search, or, as in this program, postpone that update iteration until a later
 * iteration down the k:s by using a queue. After these calculations, we find the maximum A[i][j]
 * for i,j <= N, and use the information together with the input to construct an answer sequence.
 * 
 * Performance:
 * O(N^2), runs in 0.062s using 8,084KB memory.
 */

#include <vector>
#include <stdio.h>
#include <algorithm>

const int maxn = 2000;

short A[maxn][maxn];

int main()
{
    int N, x;
    scanf("%d", &N);
    std::vector<std::pair<int, int>> v;
    for(int i = 0; i < N; i++)
    {
        scanf("%d", &x);
        v.push_back( { x, i } );
    }
    std::sort(v.begin(), v.end());
    auto fsteq = [] (std::pair<int, int> a, std::pair<int, int> b) { return a.first == b.first; };
    v.erase(std::unique(v.begin(), v.end(), fsteq), v.end()); 

    std::pair<int, short> q[maxn]; // The queue, values are { requested value, index of requester }
    int front = 0, back = 0; // Front and back of the above
    for(int n = 0; n < v.size(); n++)
    {
        front = back = 0;
        for(int i = n-1; i >= 0; i--)
        {
            A[i][n] = 1;
            // The front of the queue always holds the greatest requested values, and any requests
            // larger than v[i] will not be able to be met at this point since v is sorted
            while(front != back && v[i].first < q[front].first)
                front++;
            // If we find a value requested earlier at i = q[front].second ..
            if(front != back && q[front].first == v[i].first)
                A[q[front].second][n] = A[i][q[front++].second] + 1; // .. do the DP step
            // We push the request located "on the other side of v[i], looking from v[n]"
            q[back++] = { v[n].first -2*(v[n].first-v[i].first), i };
        }
    }

    // Find the maximum value of all the A[i][j]'s
    int maxi, maxj, max = -1;
    for(int i = 0; i < v.size(); i++)
        for(int j = 0; j < v.size(); j++)
            if(A[i][j] > max)
                max = A[maxi=i][maxj=j];

    // Reconstruct the answer sequence and print it
    int d = std::max(1, v[maxj].first - v[maxi].first);
    std::vector<int> ans;
    for(int i = maxj; ans.size() <= max; i--)
        if((v[i].first - v[maxj].first)%d == 0)
            ans.push_back(v[i].second);
    printf("%d\n", ans.size());
    for(auto it = ans.rbegin(); it < ans.rend(); it++)
        printf("%d ", *it+1);
}