/* 1140. Swamp Incident - http://acm.timus.ru/problem.aspx?num=1140
 *
 * Strategy:
 * Keep track and convert the input to the cartesian Y- and Z- coordinates and only deal with
 * involving the X coordinate in the answer when appropriate (when sgn(Y) != sgn(X)).
 *
 * Performance:
 * O(n), in-place memory usage; runs the test cases in 0.015s using 240KB memory.
 */

#include <stdio.h>
#include <numeric>
#include <vector>
#include <algorithm>

int main()
{
    int y = 0, z = 0;
    int m;
    scanf("%d", &m);
    std::vector<std::pair<char, int>> ans;
    while(m--)
    {
        char dir[2];
        int d;
        scanf("%s %d", dir, &d);
        switch(dir[0])
        {
        case 'Z':
            z += d;
            break;
        case 'Y':
            y += d;
            break;
        case 'X':
            z -= d;
            y += d;
            break;
        }
    }
    if(z*y >= 0) // Just undo our cartesian coordinates
    {
        ans.push_back(std::make_pair('Z', -z));
        ans.push_back(std::make_pair('Y', -y));
    }
    else // First descend in X to the closest coordinate axis
    {
        int m = std::min(std::abs(y), std::abs(z));
        if(z < 0)
            m = -m;
        ans.push_back(std::make_pair('X', m));
        ans.push_back(std::make_pair('Z', -(z-m)));
        ans.push_back(std::make_pair('Y', -(y+m)));
    }
    // Count the number of nonzero segments and then print them
    int s = std::accumulate(ans.begin(), ans.end(), 0, 
        [] (int x, std::pair<char, int> p) { return x + int(bool(p.second)); });
    printf("%d\n", s);
    for(auto it = ans.begin(); it < ans.end(); it++)
        if(it->second)
            printf("%c %d\n", it->first, it->second);
    return 0;
}