/* 1440. Training Schedule - http://acm.timus.ru/problem.aspx?num=1440
 *
 * Strategy:
 * For each of the 2^7 combinations of fixed training days, calculate the total number of training
 * days over the given period in constant time and see if it satisfies the requirement.
 *
 * Performance:
 * O(2^d) where d is the number of days of the week, runs in 0.015s using 260KB memory.
 */

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

std::string days[7] = { "Monday", "Tuesday", "Wednesday", "Thursday",
                        "Friday", "Saturday", "Sunday" };
std::string months[4] = { "September", "October", "November", "December" };

// Calculates the number of days between the two given dates inclusive
int dist(int d1, int m1, int d2, int m2)
{
    int d = 0;
    for(int m = m1+1; m < m2; m++)
        d += 30 + m%2;
    return m1 == m2 ? 1 + d2-d1 : d + 1 + (30+m1%2)-d1 + d2;
}

// Calculates the number of times the days given by the mask falls within the period of days that is
// 'dist' long and starts with the day 'start'
int calc(int start, int dist, int mask)
{
    int d = 0;
    for(int k = 0; k < 7; k++)
        if(mask & (1 << k))
            // Basically, this formula cuts away the part of the period until the first k-day
            // (expressed by (7+k-start)%7), but adds a first "week" before the first day (+6), and
            // then counts the number of such "weeks" that end with k-day (/7)
            d += (6+dist-(7+k-start)%7)/7;
    return d;
}

int main()
{
    std::string firstday, matchmonth;
    int nmatchdate, A, B;

    std::cin >> firstday >> matchmonth >> nmatchdate >> A >> B;

    int nfirstday = std::find(days, days+7, firstday) - days;
    int nmatchmonth = std::find(months, months+4, matchmonth) - months;
    int d = dist(2, 0, nmatchdate, nmatchmonth) - 1;

    for(int m = 1; m < (1 << 7); m++)
    {
        int c = calc((1 + nfirstday)%7, d, m);
        if(c >= A && c <= B)
        {
            std::vector<std::string> output;
            for(int k = 0; k < 7; k++)
                if(m & (1 << k))
                    output.push_back(days[k]);
            std::cout << output.size() << "\n";
            for(auto d : output)
                std::cout << d << "\n";
            return 0;
        }
    }
    std::cout << "Impossible";
}