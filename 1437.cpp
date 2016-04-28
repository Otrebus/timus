/* 1437. Gasoline Station - http://acm.timus.ru/problem.aspx?num=1437
 *
 * Strategy:
 * For each possible configuration of the cans (starting with all of them empty), generate all
 * possible new configurations (by filling any of them to capacity, or pouring from any of them to
 * any other) while keeping track of which configurations have already been tried with a hashset.
 * During this, add together the amounts in the three cans of every configuration in the 8
 * different ways and record each new such amount.
 *
 * Performance:
 * Runs the test cases in 0.031s using 1,848KB memory.
 */

#include <stdio.h>
#include <vector>
#include <algorithm>
#include <unordered_set>
#include <numeric>

std::vector<int> liters(255*3+1); // Each possible amount
int c[3]; // Capacities

class Cans // Represents the three cans
{
public:
    int f[3]; // Amount filled
    Cans()
    {
        f[0] = f[1] = f[2] = 0;
    }
    Cans(int packed)
    {
        for(int i = 0; i < 3; i++)
            f[i] = (packed >> 8*i) & 0xFF;
    }
    int pack() const // Packs the amounts into a single int
    {
        return f[0] | f[1] << 8 | f[2] << 16;
    }

    Cans fill(int from, int to) const // Fills as much as possible from one can to another
    {
        Cans cans = *this;
        int d = std::min(f[from], c[to] - f[to]);
        cans.f[from] -= d;
        cans.f[to] += d;
        return cans;
    }

    Cans fill(int to) const // Fills one can to capacity
    {
        Cans cans = *this;
        cans.f[to] = c[to];
        return cans;
    }
};

std::vector<int> s; // The different can configurations yet to go through
std::unordered_set<int> m; // The ones we already tried

void add(Cans c) // Adds this configuration and the measurements we can derive from it to our
{                // hash structures
    for(int mask = 0; mask < 8; mask++)
    {   // Go through the 2^3 different ways we can combine the contents of these cans
        int sum = 0;
        for(int i = 0; i < 3; i++)
            if(mask & (1 << i))
                sum += c.f[i];
        liters[sum] = 1;
    }
    int h = c.pack();
    if(m.find(h) == m.end()) // Add this to the set of disposed configurations so we don't do the
        m.insert(h), s.push_back(h); // same work twice
}

int main()
{
    scanf("%d %d %d", c, c+1, c+2);
    s.push_back(Cans().pack());
    while(!s.empty())
    {
        Cans cans(s.back());
        s.pop_back();
        for(int i = 0; i < 3; i++)
        {
            add(cans.fill(i));
            for(int j = i+1; j < 3; j++)
                add(cans.fill(i, j)), add(cans.fill(j, i));
        }
    }
    int ret = std::accumulate(liters.begin()+1, liters.end(), 0);
    printf("%d\n", ret);
}