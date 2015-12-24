/* 2071. Juice Cocktails - http://acm.timus.ru/problem.aspx?num=2071
 *
 * Strategy:
 * Use brute-force to find the optimal grouping of the seven different drink types.
 *
 * Performance:
 * O(n + 2^m!) where n is the number of customers and m is the number of ingredients (in this case 
 * 3). Runs the test cases in 0.046s using 1068KB memory.
 */

#include <stdio.h>
#include <vector>
#include <string>
#include <algorithm>

std::vector<int> v[8]; // The customers of each type of drink, v[110b] contains the
                       // indices of the people who ordered BP drinks, etc


int eval(std::vector<int>& order) // Evaluates the number of pourings of some drink pouring 
{                                 // order - input { 001b, 110b } means first A then BP, etc
    bool m[3] = { false, false, false }; // Are we currently pouring this ingredient?
    int ans = 0;
    for(auto i : order)
        for(int j = 0; j < 3; j++)
            if((i & (1 << j)) && !m[j]) // If we aren't pouring this ingredient, 
                m[j] = true, ans++;     // start pouring and add to the answer
            else if(!(i & (1 << j)))
                m[j] = false;           // We stopped pouring this ingredient
    return ans;
}

std::pair<int, std::vector<int>> getmin() // Returns the minimal number of pourings for
{                                         // the given combination of ingredients
    std::vector<int> w, maxw;
    for(int i = 0; i < 8; i++)
        if(!v[i].empty())
            w.push_back(i);
    int min = std::numeric_limits<int>::max();
    do
    {
        int e = eval(w);
        if(e < min)
            min = e, maxw = w;
    } while(std::next_permutation(w.begin(), w.end()));
    return { min, maxw };
}

int main()
{
    int n;
    scanf("%d", &n);
    for(int i = 1; i <= n; i++)
    {
        char s[4], type = 0;
        scanf("%s", s);
        for(int j = 0; s[j]; j++)
            type |= (s[j] == 'A' ? 1 : s[j] == 'B' ? 2 : 4);
        v[type].push_back(i);
    }

    auto p = getmin();
    printf("%d\n", p.first);
    std::vector<int> ord;
    for(int i = 0; i < p.second.size(); i++)
        for(int j = 0; j < v[p.second[i]].size(); j++)
            printf("%d ", v[p.second[i]][j]);
    return 0;
}