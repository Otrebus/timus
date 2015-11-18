/* 1051. Simple Game on a Grid - http://acm.timus.ru/problem.aspx?num=1051
 *
 * Strategy:
 * Brute-force a number of cases and extract the given solution.
 *
 * Performance:
 * Constant (0.001s, 380KB).
 */

#include <iostream>

int algo(int M, int N)
{
    if(M == 1 || N == 1)
        return (M*N+1)/2;
    else
        if(M % 3 == 0 || N % 3 == 0)
            return 2;
    return 1;
}

int main()
{
    int M, N;
    std::cin >> M >> N;
    std::cout << algo(M, N) << std::endl;
}

// Brute-force test program below

/*#include <iostream>
#include <algorithm>
#include <unordered_map>

std::unordered_map<unsigned long long, int> m;

inline unsigned long long set(unsigned long long b, int i, int j)
{
    return b | (1ull << (i*8 + j));
}

inline unsigned long long clear(unsigned long long b, int i, int j)
{
    return b & ~(1ull << (i*8 + j));
}

bool get(unsigned long long b, int i, int j)
{
    return (b >> (i*8 + j)) & 1;
}

int dfs(unsigned long long a)
{
    int x = m[a];
    if(x)
        return x;
    bool found = false;
    int count = 0;
    int min = 10000;

    for(int i = 0; i < 8; i++)
    {
        for(int j = 0; j < 8; j++)
        {
            if(get(a, i, j))
            {
                get(a, i, j);
                count++;
            }
            else
                continue;
            if(i + 2 < 8 && get(a, i+1, j) && !get(a, i+2, j))
            {
                found = true;
                auto b = set(a, i+2, j);
                b = clear(b, i+1, j);
                b = clear(b, i, j);
                min = std::min(min, dfs(b));
            }
            if(j + 2 < 8 && get(a, i, j+1) && !get(a, i, j+2))
            {
                found = true;
                auto b = set(a, i, j+2);
                b = clear(b, i, j+1);
                b = clear(b, i, j);
                min = std::min(min, dfs(b));
            }
            if(i > 1 && get(a, i-1, j) && !get(a, i-2, j))
            {
                found = true;
                auto b = set(a, i-2, j);
                b = clear(b, i-1, j);
                b = clear(b, i, j);
                min = std::min(min, dfs(b));
            }
            if(j > 1 && get(a, i, j-1) && !get(a, i, j-2))
            {
                found = true;
                auto b = set(a, i, j-2);
                b = clear(b, i, j-1);
                b = clear(b, i, j);
                min = std::min(min, dfs(b));
            }
        }
    }
    if(!found)
    {
        m[a] = count;
        return count;
    }
    m[a] = min;
    return min;
}

int algo(int M, int N)
{
    if(M == 1 || N == 1)
        return (M*N+1)/2;
    else
        if(M % 3 == 0 || N % 3 == 0)
            return 2;
    return 1;
}

int brute(int M, int N)
{
    unsigned long long a = 0;
    for(int i = 8/2-M/2; i - (8/2-M/2) + 1 <= M; i++)
        for(int j = 8/2-N/2; j - (8/2-N/2) + 1 <= N; j++)
            a = set(a, i, j);
    return dfs(a);
}

int main()
{
    for(int i = 1; i <= 6; i++)
        for(int j = 1; j <= 6; j++)
        {
            m.clear();
            printf("%d %d: ", i, j);
            if(brute(i, j) != algo(i, j))
            {
                printf("FAILED\n");
                std::system("pause");
            }
            else
                printf("passed\n", i, j);
        }
    printf("pass!");
    std::system("pause");
}*/