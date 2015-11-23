/* 1728. Curse on Team.GOV -  http://acm.timus.ru/problem.aspx?num=1728
 *
 * Strategy:
 * Keep track of two sets of teams: teams with three members, represented by the name of the
 * third member, and teams with two members, represented by the name of the second member.
 * For each given name, the conditions given in the problem are then easily checked by going
 * through the trios followed by the duos.
 *
 * Performance:
 * O(mn), runs the test cases in 0.015s using 396KB memory.
 */

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

std::vector<std::string> v[2]; // Team sets as given above

int main()
{
    int n, m, re, rk;
    std::cin >> n;
    while(n--)
    {
        int k;
        std::cin >> k;
        for(int i = 0; i < k; i++)
        {
            std::string str;
            std::cin >> str;
            if(str != "Efremov" && str != "Kantorov")
                v[k-2].push_back(std::move(str));
        }
    }
    int max = 0;
    std::string maxmember;
    std::cin >> re >> rk >> m;
    while(m--)
    {
        std::string member;
        int l;
        std::cin >> member >> l;

        // First check trios
        auto it = std::find(v[1].begin(), v[1].end(), member);
        if(it != v[1].end())
        {
            // If clash, check duos
            it = std::find(v[0].begin(), v[0].end(), member);
            if(it == v[0].end() && rk + l > max)
            {   // Kantorov plus member is the possible team
                max = rk + l;
                maxmember = member;
            }
        }
        else
        {   // Unique trio: Efremov, Kantorov and member is the possible team
            if(re + rk + l > max)
            {
                max = re + rk + l;
                maxmember = member;
            }
        }
    }
    if(max)
        std::cout << "Win\n" << maxmember << "\n";
    else
        std::cout << "Fail\n";
    return 0;
}