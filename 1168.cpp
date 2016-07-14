/* 1168. Radio Stations - http://acm.timus.ru/problem.aspx?num=1168
 *
 * Strategy:
 * Straightforward; for each radio station, update the minimum and maximum possible height for a
 * radio receiver for every location on the map.
 * 
 * Performance:
 * O(NMK), runs the test cases in 0.031s using 224KB memory.
 */

#include <cmath>
#include <algorithm>
#include <stdio.h>

const int maxn = 50, maxk = 1000;

struct pos
{
    int h, max, min;
    bool station;
} A[maxn][maxn];

struct station
{
    int x, y;
    double power;
} stations[maxk];

int main()
{
    int M, N, K;
    scanf("%d %d %d", &M, &N, &K);

    for(int y = 0; y < M; y++)
    {
        for(int x = 0; x < N; x++)
        {
            scanf("%d", &A[x][y].h);
            A[x][y].max = std::numeric_limits<int>::max()/2;
            A[x][y].min = A[x][y].h;
        }
    }
    
    for(int k = 0; k < K; k++)
    {
        int x, y;
        scanf("%d %d %lf", &y, &x, &stations[k].power);
        stations[k].x = x-1;
        stations[k].y = y-1;
        A[x-1][y-1].station = true;
    }
    
    for(int k = 0; k < K; k++)
    {
        for(int x = 0; x < N; x++)
        {
            for(int y = 0; y < M; y++)
            {
                if(A[x][y].station)
                    continue;
                int sx = stations[k].x, sy = stations[k].y;
                int dx = x-sx, dy = y-sy;
                int C = dx*dx + dy*dy;
                double p = stations[k].power;
                double dz = std::sqrt(p*p-C);
                if(dz != dz) // dz is NaN, so this location is out of range of this receiver
                    A[x][y].max = -1;
                if(A[sx][sy].h-dz > A[x][y].min)
                    A[x][y].min = std::ceil(A[sx][sy].h-dz) + 0.001;
                if(A[sx][sy].h+dz < A[x][y].max)
                    A[x][y].max = A[sx][sy].h+dz;
            }
        }
    }
    int res = 0;
    for(int y = 0; y < M; y++)
        for(int x = 0; x < N; x++)
            if(!A[x][y].station)
                res += std::max(0, A[x][y].max - A[x][y].min + 1);
    printf("%d\n", res);
}