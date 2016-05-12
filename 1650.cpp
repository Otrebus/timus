/* 1650. Billionaires - http://acm.timus.ru/problem.aspx?num=1650
 *
 * Strategy:
 * Maintain a set of the most money-dense cities. Every time a new day is given in the input, update
 * the score using the top values in the set as data.
 .
 * Performance:
 * O(k log k), runs the test ensemble in 0.39s using 10,480KB memory.
 */

#include <string>
#include <set>
#include <map>
#include <unordered_map>
#include <iostream>
#include <functional>
#include <algorithm>

const int maxn = 10000, maxm = 50000, maxk = 50000;

struct City
{
    long long money;
    int days; // Number of days on the top
    std::string name;
} cities[maxn+maxk];

struct Person
{
    std::string name;
    long long money;
    City* location;
} persons[maxn];

std::unordered_map<std::string, Person*> personMap; // Maps names into people
std::map<std::string, City*> cityMap; // Maps names into cities
std::set<std::pair<long long, City*>,std::greater<>> scoreBoard; // The cities ordered by money

 int main()
{
    std::ios::sync_with_stdio(false);
    int n, m, k, c = 0;
    std::cin >> n;
    for(int i = 0; i < n; i++)
    {
        std::string name, location; long long money;
        std::cin >> name >> location >> money;
        personMap[name] = &persons[i];
        if(!cityMap[location])
            cityMap[location] = &cities[c++];
        persons[i].location = cityMap[location];
        persons[i].money = money;
        cityMap[location]->name = location;
        cityMap[location]->money += money;
    }
    for(auto c : cityMap)
        scoreBoard.insert( { c.second->money, c.second } );
    std::cin >> m >> k;
    int prevday = 0, day;
    std::string name, place;
    while(true)
    {
        bool finished = (!(std::cin >> day >> name >> place));
        if(finished) // Zoom to the last day m (the last entry could be many days prior)
            day = m;
        if(day != prevday)
        {
            auto it2 = scoreBoard.begin();
            auto it = it2++;
            if(it2 == scoreBoard.end() || it->first > it2->first)
                it->second->days += day-prevday;
        }
        if(finished)
            break;
        if(!cityMap[place]) // New city mentioned
        {
            cityMap[place] = &cities[c++];
            cityMap[place]->name = place;
        }

        // Update the set and the city data structure itself
        auto nextplace = cityMap[place];
        auto person = personMap[name];
        auto prevplace = person->location;
        auto prevmoney = prevplace->money;
        scoreBoard.erase( { prevmoney, prevplace } );
        prevplace->money -= person->money;
        scoreBoard.insert( { prevplace->money, prevplace } );
        scoreBoard.erase( { nextplace->money, nextplace } );
        nextplace->money += person->money;
        scoreBoard.insert( { nextplace->money, nextplace } );
        person->location = nextplace;
        prevday = day;
    }
    std::vector<std::pair<std::string, int>> output;
    for(auto& r : scoreBoard)
        if(r.second->days)
            output.push_back( std::make_pair(r.second->name, r.second->days));
    std::sort(output.begin(), output.end());
    for(auto& o : output)
        std::cout << o.first << " " << o.second << "\n";
}