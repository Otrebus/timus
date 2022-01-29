/* 1739. Faryuks - http://acm.timus.ru/problem.aspx?num=1739
 *
 * Strategy:
 * Dynamic programming over A[m][s] where m is the bitmask of Faryuks left, and s is the bitmask
 * of the two actions used so far. We also precalculate all the sets of Faryuks - with zero, one
 * or two of the anti- or pro-scent actions performed on them - whose scent sum is divisible by
 * 256, to speed up the search.
 *
 * Performance:
 * O(2^(2n)), runs the tests in 0.015s using 812KB memory.
 */

#include <stdio.h>
#include <vector>
#include <string>
#include <iostream>
#include <cstring>

struct action {     // The action of carrying the faryuks in bitmask m after
    int t, i, j, m; // possibly having altered the scent of up to two of those
};

int A[1<<12][4];    // The DP array
action N[1<<12][4]; // Next action in the optimal series
int w[12];          // The aromas
int n;
std::vector<action> V[4]; // Contains all the valid sets of faryuks that can be carried
                          // after having applied the different actions to up to two of them

int sum(int m) { // Sums the aroma of faryuks in bitmask
    int r = 0;
    for(int i = 0; i < n; i++)
        r += !!(m & (1 << i))*w[i];
    return r;
}

int count(int m) { // Counts the number of bits in bitmask
    int r = 0;
    for(int i = 0; i < n; i++)
        r += !!(m & (1 << i));
    return r;
}

int dfs(int m, int s) {
    // Performs all possible actions with faryuks inside the fence represented by bitmask m
    // and aroma-strength-affecting actions performed so far represented in bitmask s and returns
    // the least number of checkpoint-passings to reach 0 faryuks inside the fence
    if(m == 0)
        return 0;

    if(A[m][s] < 1e9)
        return A[m][s];

    int ret = 1e8;
    for(int i = 0; i < 4; i++) { // For each type of action
        if(!(s & i)) {
            // If the action hasn't been performed yet
            for(int j = 0; j < V[i].size(); j++) {
                // Remove the faryuks of the action from the mask and recurse; here we
                // also use the fact that we can carry faryuks back (we don't need to
                // match the entire mask)
                int m2 = m & ~(V[i][j].m), s2 = s | i;
                if(m2 != m) { // .. but we need to match at least something, otherwise no use
                    int x = dfs(m2, s2);
                    if(ret > x + 1) { // Best solution so far
                        ret = x + 1;
                        N[m][s] = V[i][j];
                    }
                }
            }
        }
    }
    return A[m][s] = ret;
}

std::vector<action> minimize(std::vector<action>& u) {
    // Removes any element of u that has a superset in u
    std::vector<action> v;
    for(int i = 0; i < u.size(); i++) {
        bool s = false;
        for(int j = 0; j < u.size() && !s; j++)
            s = (i != j && ((u[i].m & u[j].m) == u[i].m));
        if(!s)
            v.push_back(u[i]);
    }
    return v;
}


int main() {
    std::memset(A, 0x3f, sizeof(A));

    std::cin >> n;
    for(int i = 0; i < n; i++) {
        std::cin >> w[i];
        w[i]*=1000; // Prevents fractional values
    }

    // Calculate all the sets of faryuks that can be carried without altering their scent
    for(int m = 1; m < (1 << n); m++) {
        int S = sum(m);

        if(!(S % 256000))
            V[0].push_back({ 0, 0, 0, m });
    }
    // Don't bother with any 256-divisible sets of faryuks that are subsets of a bigger sets
    // of such faryuks, because the bigger set can do everything the smaller set can
    V[0] = minimize(V[0]);

    // Calculate the sets of faryuks, with an aroma-changing action performed on one of them
    // that can be carried through the checkpoint
    for(int i = 0; i < n; i++) { // For each faryuk to alter
        std::vector<action> W1, W2;
        for(int m = 1; m < (1 << n); m++) { // For each set of faryuks
            if(!((1 << i) & m)) // If the faryuk to alter isn't in the set, skip
                continue;
            int S = sum(m);

            // Apply deodorant
            int s = S - w[i]/4;
            if(!(s % 256000))
                W1.push_back({ 1, i, -1, m });

            // Apply scent
            s = S + w[i]/10;
            if(!(s % 256000))
                W2.push_back({ 2, i, -1, m });
        }

        // Minimize the number of subsets like we did for the non-scented faryuks
        W1 = minimize(W1);
        W2 = minimize(W2);
        V[1].insert(V[1].end(), W1.begin(), W1.end());
        V[2].insert(V[2].end(), W2.begin(), W2.end());
    }
    
    // Similar to the previous for loops, but with the action performed to two faryuks
    for(int i = 0; i < n; i++) {
        for(int j = 0; j < n; j++) {
            std::vector<action> W;
            for(int m = 1; m < (1 << n); m++) {
                if(!((1 << j) & m) || !((1 << i) & m))
                    continue;
                int S = sum(m);
                int s = (i != j) ? S - w[i]/4 + w[j]/10 : S - w[i]*7/40;
                if(!(s % 256000))
                    W.push_back({ 3, i, j, m });
            }
            W = minimize(W);
            V[3].insert(V[3].end(), W.begin(), W.end());
        }
    }

    int x = dfs((1<<n)-1, 0);
    if(x == 1e8)
        return printf("IMPOSSIBLE"), 0;

    int m = (1<<n)-1, s = 0;
    std::vector<std::string> v;

    // Reconstruct the dfs path with the help of the Next array
    while(m > 0) {
        // Unpack the 
        int d = ~m & N[m][s].m, i = N[m][s].i, j = N[m][s].j, t = N[m][s].t;

        if(d) {
            std::string a = "return " + std::to_string(count(d));
            for(int i = 0; i < n; i++)
                if(d & (1<<i))
                    a += " " + std::to_string(i+1);
            v.push_back(a);
        }

        if(t & 1)
            v.push_back("dearomatize " + std::to_string(i+1));
        if(t & 2)
            v.push_back("aromatize " + std::to_string(t == 3 ? (j+1) : (i+1)));

        std::string a = "take " + std::to_string(count(N[m][s].m));
        for(int i = 0; i < n; i++)
            if(N[m][s].m & (1<<i))
                a += " " + std::to_string(i+1);
        v.push_back(a);

        int s2 = s | N[m][s].t, m2 = m & ~N[m][s].m;
        s = s2, m = m2;
    }
    printf("%d\n", v.size());
    for(auto& s : v)
        std::cout << s << std::endl;
}
