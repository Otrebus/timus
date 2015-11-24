/* 1971. Graphics Settings - http://acm.timus.ru/problem.aspx?num=1971
 *
 * Strategy:
 * Store all settings and their slowdowns in a set, and whenever we compute the performance
 * we multiply together all stored slowdowns at most until we are sure to have a "slideshow".
 *
 * Performance:
 * O(m*log(p/(h*w)), runs in 0.967s using 9532KB memory.
 */

#include <iostream>
#include <string>
#include <unordered_map>

std::unordered_map<std::string, int> option; // Maps the option string into its slowdown
std::unordered_map<int, int> set; // Maps each possible slowdown (k) to its multiplicity
long long h, w, p;

inline void line(long long p, long long r)
{
    std::cout << ((p < 10*r) ? "Slideshow\n" : (p < 60*r) ? "So-so\n" : "Perfect\n");
}

void output()
{
    long long r = h*w;
    for(auto it = set.begin(); it != set.end(); it++)
    {
        for(int i = 0; i < it->second; i++)
        {
            r *= (long long) it->first;
            if(p < 10*r) // We're sure to have a slideshow
                return line(p, r);
        }
    }
    line(p, r);
    return;
}

int main()
{
    std::ios::sync_with_stdio(false);
    int n;
    std::cin >> n;
    while(n--)
    {
        std::string s;
        int k;
        std::cin >> s >> k;
        option[s] = k;
        set[option[s]]++;
    }
    std::cin >> h >> w >> p;
    int m;
    std::cin >> m;
    output();
    while(m--)
    {
        std::string how, what;
        std::cin >> how;
        if(how[0] == 'O')
        {
            std::cin >> what;
            if(how[1] == 'f')
                set[option[what]]--;
            else
                set[option[what]]++;
        }        
        else
            std::cin >> h >> w;
        output();
    }
}