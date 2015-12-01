/* 1535. The Hobbit or There and Back Again - http://acm.timus.ru/problem.aspx?num=1535
 *
 * Strategy:
 * Brute-forced a number of answers to find a pattern. Below program generates that pattern.
 *
 * Performance:
 * O(N), runs the test suite in 0.015s using 1184KB memory.
 */

#include <iostream>
#include <vector>
#include <algorithm>

std::vector<int> getmin(int n)
{
    std::vector<int> v;
    if(n % 2)
    {
        int up = 2;
        int down = n;
        for(int i = 0; up < n; i++)
        {
            if(i % 2)
            {
                v.push_back(up);
                up += 2;
            }
            else
            {
                v.push_back(down);
                down -= 2;
            }
        }
        v.push_back(1);
    }
    else
    {
        int down = n;
        int up = 2;
        int i = 0;
        while(up <= down)
        {
            if(i++ % 2)
            {
                v.push_back(up);
                up += 2;
            }
            else
            {
                v.push_back(down);
                down -= 2;
            }
        }
        if(n % 4 == 0)
        {
            up = n/2+1;
            down = n/2-1;
            while(v.size() < n)
            {
                if(up <= n)
                {
                    v.push_back(up);
                    up += 2;
                }
                if(down > 0)
                {
                    v.push_back(down);
                    down -= 2;
                }
            }
        }
        else
        {
            v.push_back(n/2);
            up = n/2+2;
            down = n/2-2;
            while(v.size() < n)
            {
                if(up <= n)
                {
                    v.push_back(up);
                    up += 2;
                }
                if(down > 0)
                {
                    v.push_back(down);
                    down -= 2;
                }
            }
        }
    }
    return v;
}

std::vector<int> getmax(int n)
{
    std::vector<int> v;
    for(int i = 1; i <= n; i += 2)
        v.push_back(i);
    for(int i = n % 2 ? n-1 : n; i > 0; i -= 2)
        v.push_back(i);
    return v;
}

void print(std::vector<int>& ans)
{
    for(int i = 0; i < ans.size(); i++)
    {
        if(ans[i] == 1)
        {
            for(int j = 0; j < ans.size(); j++)
                std::cout << ans[(j+i)%ans.size()] << " ";
            std::cout << "\n";
            return;
        }
    }
}

int main()
{
    int n;
    std::cin >> n;
    print(getmin(n));
    print(getmax(n));
}