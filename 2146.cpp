/* 2146. Quiz Show - http://acm.timus.ru/problem.aspx?num=2146
 *
 * Strategy:
 * For each p, the set will be split into equivalence classes modulo gcd(n, p). We can then go
 * through each class, form a histogram of letters, record its difference from n/g, sum this
 * number up over each class and see if the sum is lower than k, and overall report the lowest
 * p for which this is true, along with the corresponding letter replacements.
 * 
 * In plainer English, let's say that we have n = 9 and p = 6. As we traverse the digits starting
 * from 1, 2 and 3, we reach numbers { 1, 7, 4, (1) }, { 2, 8, 5, (2) } and { 3, 9, 6, (3) }. These
 * are also the sets of numbers we reach when we have p = 3, and for any other number where
 * gcd(9, 6) = 3. We can then record the most common character in each of these sets to figure out
 * which one is the cheapest to replace, and we don't have to repeat the process for any future p
 * that gives the same gcd with n.
 *
 * Performance:
 * O(nd), where d is the number of divisors of n, runs the tests in 0.078s using 4,248KB memory.
 */

#include <stdio.h>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <numeric>

int n, k, i, j;
int minp;
char s[100001], c; // Input
char o[100001];    // Output

struct entry { int n; char c; }; // The least expensive character to replace in an equivalence class
std::unordered_map<int, std::vector<entry>> v; // Maps gcd to the above mentioned

int gcd(int a, int b) { return a ? gcd(b % a, a) : b; }

void calc(int p) {
    int g = gcd(n, p), t = n/g;
    if(v.find(g) != v.end()) // Already calculated for this gcd
        return;
    for(int i = 0; i < g; i++) { // For each equivalence class
        int c[26] = {};
        for(int j = 0; j < t; j++) // Calculate its histogram
            c[s[(i+j*p)%n]-'a']++;
        auto m = std::max_element(c, c+26); // Take the most common character and push
        v[p].push_back({ *m, 'a'+(char)(m-c) });
    }
    // Take the sum of all the costs
    int x = std::accumulate(v[p].begin(), v[p].end(), 0, [&t] (int a, entry b) { return a+t-b.n; });
    if(x <= k) // If less than k, report that a minimum p is found
        minp = p;
}

int main() {
    scanf("%d %d %s", &n, &k, s);

    for(int p = 1; !minp; p++)
        calc(p);

    printf("%d\n", minp);
    int g = gcd(n, minp), t = n/g;
    for(i = 0; i < g; i++) // Deduce the replaced characters from v
        for(j = 0, c = v[minp][i].c; j < t; j++)
            o[(i+j*minp)%n] = c;
    printf("%s", o);
}
