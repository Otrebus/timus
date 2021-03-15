/* 2110. Remove or Maximize - http://acm.timus.ru/problem.aspx?num=2110
 *
 * Strategy:
 * If the difference between n and k (the number of numbers we can pick) is larger than the number
 * of bits, we can, for each bit, just pick a number that will set that bit to one in the result,
 * so the result will simply be the OR of all integers. If k is less than 7 we then know than n must
 * be less than or equal to 25 (or it would have fallen into the above case), so we can solve that
 * with dfs since the number of combinations is relatively small. For all other cases, the number
 * of bits in the result is relatively small. In this case we can associate each bit index with the
 * set of numbers that set that bit to 1, and we random-brute-force the solution by picking a random
 * number for each still empty bit (in the result) from its bucket and OR it to the result. Repeat
 * some large number of times and we are practically certain to get the maximum value.
 *
 * Performance:
 * Hard to derive the complexity, runs the tests in 0.859s using 1,304KB memory.
 */

#include <iostream>
#include <vector>
#include <unordered_set>
#include <algorithm>
#include <random>

std::vector<int> v;
std::vector<int> b[17];
int n, k;

int dfs(int i, int j, int s) {
    if(i == n)
        return j == 0 ? s : 0;
    int a = j ? dfs(i + 1, j - 1, s) : 0;
    int b = dfs(i + 1, j, s | v[i]);
    return std::max(a, b);
}

// Used to randomize what bucket we pick numbers from
int w[17] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16 };

int rnd(std::default_random_engine& gen) {
    // For each still zero bit in the result, randomly pick some number that
    // sets that bit (among others) to one
    int r = n - k, s = 0;
    std::random_shuffle(w, w+17);
    for(int a = 16; a >= 0; a--) {
        int i = w[a];
        if((s & (1 << i)) == 0 && r && b[i].size()) {
            std::uniform_int_distribution<int> jgen(0, b[i].size()-1);
            int j = jgen(gen);
            s |= b[i][j];
            r--;
        }
    }
    return s;
}

int solve() {

    std::default_random_engine generator(10);

    int max = 0;
    if(n - k > 31) {
        int sum = 0;
        for(auto x : v)
            sum |= x;
        return sum;
    }
    if(k <= 7)
        return dfs(0, k, 0);

    std::unordered_set<int> m(v.begin(), v.end()); // Remove duplicates
    for(auto x : m) { // Associate each bit index with all the numbers that has it set
        for(int d = 0; d < 17; d++)
            if(x & (1 << d))
                b[d].push_back(x);
    }

    for(int i = 0; i < 1000000; i++) {
        // Randomize-brute-force the answer
        int res = rnd(generator);
        if(res > max) {
            max = res;
            i = 0; 
        }
    }
    return max;
}

int main() {
    std::cin >> n >> k;

    for(int i = 0; i < n; i++) {
        int x;
        std::cin >> x;
        v.push_back(x);
    }
    std::cout << solve();
}
