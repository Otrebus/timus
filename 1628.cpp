/* 1628. White Streaks - http://acm.timus.ru/problem.aspx?num=1628
 *
 * Strategy:
 * Sort the input points first by row major order, and look at how much white space is spanned
 * between each point. If there are at least two spaces in between, this constitutes one streak.
 * Entire lines between two points counts as one streak each. If there is only a single white space
 * this only counts as a streak if it is an isolated point and not part of a streak in the other
 * direction, so only take note of those points. 
 *   Do the same as above again, but for the points sorted by column major order, and count how
 * many singular points we've counted twice, add this to the answer, and we have the final answer.
 *
 * Performance:
 * O(k log k), runs the tests in 0.062s using 1728KB memory (ranked 13/562).
 */

#include <stdio.h>
#include <vector>
#include <cctype>
#include <algorithm>

inline int getint()
{
    int ret = 0, ch;
    while(!isdigit(ch = getc(stdin)));
    ret = ch - '0';
    while(isdigit(ch = getc(stdin)))
        ret = ret*10 + ch - '0';
    return ret;
}

int main()
{
    std::vector<std::pair<int, int>> p; // Input points
    std::vector<std::pair<int, int>> v; // Possible singular points that we encounter
    int m = getint(), n = getint(), k = getint();
    for(int i = 0; i < k; i++)
    {
        int x = getint(), y = getint();
        p.push_back({x, y});
    }
    p.push_back({1, 0}); // Sentinel point, top left
    p.push_back({m, n+1}); // Sentinel point, bottom right
    std::sort(p.begin(), p.end());
    int ans = 0;
    for(int i = 1; i < p.size(); i++)
    {
        int dy = p[i].first - p[i-1].first; // Number of rows skipped
        int dx = p[i].second - p[i-1].second - 1; // Horizontal streak (only applicable for dy = 0)
        if(dy == 0) // Points on same row, straightforward
        {
            if(dx == 1)
                v.push_back( { p[i].first, p[i].second - 1 } );
            else if(dx > 0)
                ans++;
        }
        else // Points on different rows, a little bit trickier
        {
            if(p[i].second == 2) // Second point has a singular point before the left wall
                v.push_back( { p[i].first, 1 } );
            else if(p[i].second > 2) // Second point has a bigger space before the left wall
                ans++;
            if(p[i-1].second == n-1) // First point has a singular point before the right wall
                v.push_back( { p[i-1].first, n } );
            else if(p[i-1].second < n-1) // First point has a bigger space before the right wall
                ans++;
            if(n > 1) // If the row width is greater than one, each row is definitely a streak
                ans += dy - 1;
            // Otherwise, we are dealing with a singular point only if we have exactly one 
            // line between (the next pass will catch the streak since it's a single column)
            else if(dy == 2) 
                v.push_back( { p[i].first-1, 1 } );
        }
    }

    // The next number of lines is the same as above, just ordered in column order
    p.push_back({0, 1});
    p.push_back({m+1, n});
    std::sort(p.begin(), p.end(), [] (std::pair<int, int> a, std::pair<int, int> b) 
             { return a.second == b.second ? a.first < b.first : a.second < b.second; } );
    for(int i = 2; i < p.size()-1; i++)
    {
        int dy = p[i].second - p[i-1].second;
        int dx = p[i].first - p[i-1].first - 1;
        if(dy == 0)
        {
            if(dx == 1)
                v.push_back( { p[i].first-1, p[i].second} );
            else if(dx > 0)
                ans++;
        }
        else
        {
            if(p[i].first == 2)
                v.push_back( { 1, p[i].second } );
            else if(p[i].first > 2)
                ans++;
            if(p[i-1].first == m-1)
                v.push_back( { m, p[i-1].second } );
            else if(p[i-1].first < m-1)
                ans++;
            if(m > 1) 
                ans += dy - 1;
            else if(dy == 2)
                v.push_back( { 1, p[i].second-1 } );
        }
    }
    std::sort(v.begin(), v.end());
    // Singular points counted twice are streaks
    int dif = v.end() - std::unique(v.begin(), v.end());
    printf("%d", ans + dif);
    return 0;
}