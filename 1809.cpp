/* 1809. Chapaev and Potatoes - http://acm.timus.ru/problem.aspx?num=1809
 *
 * Strategy:
 * Brute-force: try every combination of moving 0, 1 or 2 potatoes into all possible random
 * locations until a valid location is found.
 *
 * Performance:
 * Constant in the size of the input, but with a variable sized board of width N this would be
 * O(N^4). Runs in 0.015s using 384KB memory.
 */

#include <iostream>
#include <vector>
#include <algorithm>

// Checks whether a board configuration is valid according to the rules given
bool isValid(const std::vector<std::pair<int, int>>& v)
{
    for(int i = 0; i < 4; i++)
    {
        int hits = 0;
        for(int j = 0; j < 4; j++)
            if(i != j && (v[j].first == v[i].first || v[i].second == v[j].second))
                hits++;
        if(hits != 1)
            return false;
    }
    return true;
}

// Starting at index i and moving k potatoes, tries to mutate v into a valid configuration and
// returns true if it succeeded
bool search(std::vector<std::pair<int, int>>& v, int k, int i)
{
    if(!k)
        return isValid(v);
    for(int j = 0; j < 1+v.size()-k; j++)
    {
        for(int x = 1; x <= 20; x++)
        {
            for(int y = 1; y <= 20; y++)
            {
                if(std::find(v.begin(), v.end(), std::make_pair(x, y)) != v.end())
                    continue;
                auto tmp = v[j];
                v[j] = { x, y };
                if(search(v, k-1, j+1))
                    return true;
                v[j] = tmp;
            }
        }
    }
    return false;
}

int main()
{
    std::vector<std::pair<int, int>> v(4);
    for(int i = 0; i < 4; i++)
        std::cin >> v[i].first >> v[i].second;
    for(int i = 0; i < 3; i++)
        if(search(v, i, 0))
            break;
    for(int i = 0; i < 4; i++)
        std::cout << v[i].first << " " << v[i].second << "\n";
}