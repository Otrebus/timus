/* 1824. Ifrit Bomber - http://acm.timus.ru/problem.aspx?num=1824
 *
 * Strategy:
 * Brute-force dfs - associate each city with a bitmask of the list of cities within the explosion
 * radius from this city (referred to as 'bombable' from the city), along with the bitmask of cities
 * not bombable from cities with indices higher than this city, to help prune the search.
 *
 * Performance:
 * O(2^n), runs the tests in 0.031s using 264KB memory.
 */

#include <iostream>

struct city {
    int m, M; // Cities bombable from here, max city index bombable from here
    int x, y, i;
} cities[31];
int c[30]; // For each index i, the cities not bombable from cities > i

int dist2(city& a, city& b) {
    int dx = a.x-b.x, dy = a.y-b.y;
    return dx*dx + dy*dy;
}

int n, r;
int mint = 31, minM = 0;
void dfs(int k, int t, int m, int M) {
    // City, number of bombs, cities bombed, cities used
    if(m == (1 << n) - 1) {
        // If all cities are bombed and we reached it with the fewest bombs
        if(t < mint)
            mint = t, minM = M;
        return;
    }
    if(k >= n)
        return;

    // Only skip this city if this isn't the last city we can bomb some hitherto unbombed city from
    if((c[k] & m) == c[k])
        dfs(k+1, t, m, M);
    int m1 = m | cities[k].m;
    if(m1 != m) // Drop a bomb on this city
        dfs(k+1, t+1, m1, M | (1 << k));
}

int main() {
  
    std::cin >> n >> r;
    for(int i = 0; i < n; i++) {
        std::cin >> cities[i].x >> cities[i].y;
        cities[i].i = i + 1;
    }

    for(int i = n-1; i >= 0; i--) // Shuffle cities to reduce chances of worst-case input
        std::swap(cities[i], cities[rand()%(i+1)]);

    for(int i = 0; i < n; i++) { // Set the cities-bombable-from-this-city bitmask
        auto& c = cities[i];
        for(int j = 0; j < n; j++)
            if(dist2(c, cities[j]) <= r*r)
                c.m |= (1 << j), c.M = std::max(j, c.M);
    }

    for(int i = 0; i < n; i++) // Set the cities-not-bombable-after-this bitmask
        c[cities[i].M] |= (1 << i);

    dfs(0, 0, 0, 0);

    std::cout << mint << std::endl;
    for(int i = 0; i < n; i++)
        if((1 << i) & minM)
            std::cout << cities[i].i << " ";
}
