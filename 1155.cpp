/* 1155. Troubleduons - http://acm.timus.ru/problem.aspx?num=1155
 *
 * Strategy:
 * On two opposite sides of the cube: twice, eliminate as much as possible from the largest
 * vertex from the biggest adjacent vertex. Then merge the remaining two vertices on the side.
 * For the two vertices on opposite sides of the cube, they are deletable if they are completely
 * diagonal, otherwise there is no solution.
 *
 * Performance:
 * Runs in 0.015s using 404KB.
 */

#include <cmath>
#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

int cube[8];

std::vector<std::string> output;

bool adjacent(int a, int b)
{
    return a != b && std::abs(a-b) != 2;
}

int opposite(int a)
{
    return (a > 3 ? 4 : 0) + (a + 2) % 4;
}

void inc(char a, char b)
{
    char str[] = { 'A'+a, 'A'+b, '+', 0 };
    cube[a]++;
    cube[b]++;
    output.push_back(std::string(str));
}

void dec(char a, char b)
{
    char str[] = { 'A'+a, 'A'+b, '-', 0 };
    cube[a]--;
    cube[b]--;
    output.push_back(std::string(str));
}

void inc(char a, char b, int n)
{
    while(n--)
        inc(a, b);
}

void dec(char a, char b, int n)
{
    while(n--)
        dec(a, b);
}

void cut(int offset)
{
    int m = std::max_element(cube+offset, cube+offset+4) - cube;
    int n = offset + (m+3)%4;
    if(cube[n] < cube[offset + (m+1)%4])
        n = offset + (m+1)%4;
    dec(m, n, cube[n]);
}

void merge(int offset)
{
    int m = std::max_element(cube+offset, cube+offset+4) - cube;
    int tmp = cube[m];
    cube[m] = -1;
    int n = std::max_element(cube+offset, cube+offset+4) - cube;
    cube[m] = tmp;
    if(m == n)
        return;
    if(adjacent(m, n))
        return dec(m, n, cube[n]);
    inc(offset+(n+1)%4, m, cube[n]);
    dec(offset+(n+1)%4, n, cube[n]);
}

bool finish()
{
    int m1 = std::max_element(cube, cube+4) - cube;
    int m2 = std::max_element(cube+4, cube+8) - cube;

    if(cube[m1] == 0 && cube[m2] == 0)
        return true;
    if(cube[m1] != cube[m2] || adjacent(m1+4, m2))
        return false;
    if(m1+4 == m2)
    {
        dec(m1, m2, cube[m1]);
        return true;
    }

    inc((m1+3)%4, opposite(m1), cube[m1]);
    dec(m2, opposite(m1), cube[m2]);
    dec(m1, (m1+3)%4, cube[m1]);
    return true;
}

int main()
{
    for(int i = 0; i < 8; i++)
        std::cin >> cube[i];
    for(int i : { 0, 4 })
        cut(i), cut(i), merge(i);
    if(finish())
        for(auto s : output)
            std::cout << s << std::endl;
    else
        std::cout << "IMPOSSIBLE" << std::endl;
    return 0;
}