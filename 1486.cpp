/* 1486. Equal Squares - http://acm.timus.ru/problem.aspx?num=1486
 *
 * Strategy:
 * Use a Rabin-Karp style rolling hash to search for squares of size K, and then do binary search
 * over K. The rolling hash is computed in two steps. First we hash each vertical string of size K,
 * and then we use each such K-sized vertical string as a "character" with its hash value as the
 * character value to compute the rolling hash for the squares as we march horizontally.
 *
 * Performance:
 * O(W^2 log W) where W = max(N, M) assuming not too many hash collisions, runs in 0.468s using
 * 10,508KB memory.
 */

#include <algorithm>
#include <stdio.h>
#include <unordered_map>
#include <unordered_set>

const unsigned int basechar = 587; // The base for the rolling hash for individual chars
const unsigned int basecol = 239; // The base for the rolling hash for the vertical strings
const int maxn = 500;

unsigned char A[maxn][maxn]; // The input
unsigned int C[maxn][maxn]; // C[i][j] is the hash for the vertical string starting at (i, j)
unsigned int S[maxn][maxn]; // Same as above, but for the square at (i, j)

// Searches the array size NxM for matching squares of size K, returning true if one was found and
// the locations in the additional parameter slots
bool solve(int N, int M, int K, int& x1, int& y1, int& x2, int& y2)
{
    // To save space, we hash squares to a coordinate first, and only a vector of coordinates
    // once we have a hash collision in the first set
    std::unordered_map<unsigned int, std::pair<short, short>> set;
    std::unordered_map<unsigned int, std::vector<std::pair<short, short>>> map;

    int charN = 1, colN = 1; // base^N for characters and columns
    for(int i = 0; i < K; i++)
        charN *= basechar, colN *= basecol;

    // First, compute the rolling hashes for the strings
    for(int x = 0; x < M; x++)
    {
        C[x][0] = 0;
        for(int y = 0; y < K; y++)
            C[x][0] = (C[x][0] + A[x][y])*basechar;
        for(int y = 1; y + K <= N; y++)
            C[x][y] = ((C[x][y-1]-charN*A[x][y-1]) + A[x][y+K-1])*basechar;
    }

    // Next, use the above hashes to compute hashes for the squares
    for(int y = 0; y + K <= N; y++)
    {
        S[0][y] = 0;
        for(int x = 0; x + K <= M; x++)
        {
            if(x == 0)
                for(int x = 0; x < K; x++)
                    S[0][y] = (S[0][y] + C[x][y])*basecol;
            else
                S[x][y] = ((S[x-1][y]-colN*C[x-1][y]) + C[x+K-1][y])*basecol;

            // As we insert this hash into the hash set we check if it was already found
            // and if so we do a manual comparison
            auto it = set.find(S[x][y]);
            if(it != set.end())
            {
                auto& v = map[S[x][y]]; 
                if(v.empty())
                    v.push_back(it->second);
                for(auto p : v)
                {
                    bool match = true;
                    for(int i = 0; match && i < K; i++)
                        for(int j = 0; match && j < K; j++)
                            if(A[p.first+i][p.second+j] != A[x+i][y+j])
                                match = false;
                    if(match)
                    {
                        x1 = p.first; y1 = p.second; x2 = x; y2 = y;
                        return true;
                    }
                }
                v.push_back( { x, y } );
            }
            else
                set[S[x][y]] = { x, y };
        }
    }
    return false;
}

int main()
{
    int N, M, x1, x2, y1, y2;
    scanf("%d %d", &N, &M);

    for(int j = 0; j < N; j++)
    {
        char input[501];
        scanf("%s", input);
        for(int i = 0; i < M; i++)
            A[i][j] = input[i];
    }
    bool found = false;
    int left = 1, right = std::min(N, M);
    while (right >= left) // Binary search
    {
        int mid = (left + right)/2;
        if(solve(N, M, mid, x1, y1, x2, y2))
            found = true, left = mid + 1;
        else
            right = mid - 1;
    }
    if(found)
        printf("%d\n%d %d\n%d %d\n", right, y1+1, x1+1, y2+1, x2+1);
    else
        printf("0\n");
}