/* 1063. Domino Puzzle - http://acm.timus.ru/problem.aspx?num=1063
 *
 * Strategy:
 * Represent the problem as a graph with 6 vertices, where each vertex is a valid number of dots on
 * a half of a domino, and the edge between any vertex i and j indicates whether the number of given
 * dominoes of those two halves is odd or even. If there's an even number of dominoes of one type we
 * can always use them to return to the same number like [(1,2), (2,1)] so these are irrelevant and
 * we focus on the "odd" edges. First, we try every way to make the graph connected by brute-force,
 * and for each of these ways, greedily add dominoes to create an Euler path (making the number of
 * odd vertices two or less), and report the minimum cost of all these. Since the number of digits
 * is small we can represent the above graps as 64-bit numbers, and sets of pairs as 64-bit numbers
 * as well.
 *
 * Performance:
 * Something like O(d!*d^3), where d is the number of digits, runs the tests in 0.001s using an
 * unreported amount of memory.
 */

#include <stdio.h>
#include <vector>
#include <numeric>

using ll = long long;

ll ansv, answ; // The set of dominoes in the answer (connecting dominoes, greedy dominoes)
int minans = 100000;
bool E[6]; // Whether any dominoes has this digit

bool set(ll v, int i, int j) { // Whether the pair (i, j) exists in v
    return v & (1ll << (i*6 + j));
}

bool isOdd(ll G, int x) { // Whether vertex x is odd in G
    return G & (1ll << (50+x));
}

bool isConnected(ll G, int a, int b) { // Whether vertices a and b are (oddly) connected
    return (a == b) || set(G, a, b);
}

ll connect(ll G, int a, int b) { // Connects vertices a and b in G and updates their oddness
    return (G | (1ll << (a*6 + b)) | (1ll << (b*6+a))) ^ (1ll << (50+a)) ^ (1ll << (50+b));
};

int sum(ll v) { // Sums the digits of the pairs in v
    int sum = 0;
    for(int i = 1; i <= 6; i++) {
        for(int j = 1; j <= 6; j++)
            sum += set(v, i, j) ? i+j : 0;
    }
    return sum;
}

void addD(ll G, ll v) {
    // Greedily adds vertices to graph G to make an Euler path (pairs in v were used to connect it)
    int a = 0, nOdd = 0, sw = 0;
    ll w = 0;
    for(int i = 1; i <= 6; i++)
        nOdd += isOdd(G, i);

    // Greedily connect pairs of digits
    for(int i = 1; i <= 6 && nOdd > 2; i++) {
        if(isOdd(G, i)) {
            if(a) { // Half of a pair, add i
                w |= 1ll << (a*6 + i); // Add the edge to the answer
                sw += a + i; // Update the sum of digits
                a = 0; // We no longer have the first half of the pair
                nOdd -= 2; // We have two fewer odd vertices
            } else // No pair yet
                a = i;
        }
    }
    int an = sum(v) + sw;
    if(an < minans) { // Update the minimum answer
        minans = an, answ = w, ansv = v;
    }
}

ll updatetc(ll tc, int a, int b) { // Updates the transitive closure of a graph
    tc = connect(tc, a, b);
    for(int i = 1; i <= 6; i++)
        for(int j = 1; j <= 6; j++)
            if(E[i] && E[j] && isConnected(tc, i, a) && isConnected(tc, j, b))
                tc = connect(tc, i, j);
    return tc;
}

void dfsc(ll G, ll tc, int i0, ll v) { // Brute-force connects the graph
    // Check if the graph is connected
    bool connected = true;
    for(int i = 1; i <= 6; i++)
        for(int j = i+1; j <= 6; j++)
            if(E[i] && E[j])
                connected &= isConnected(tc, i, j);

    if(connected) // If it is, greedily make an Euler path with the rest
        return addD(G, v);

    // If not, try every way of adding dominoes to connect it
    for(int i = i0; i <= 6; i++) {
        for(int j = i+1; j <= 6; j++) {
            if(E[i] && E[j] && !isConnected(tc, i, j)) {
                v |= (1ll << (i*6 + j));
                dfsc(connect(G, i, j), updatetc(tc, i, j), i, v);
                v ^= (1ll << (i*6 + j));
            }
        }
    }
}

int main() {
    int N, a, b;
    ll G = 0, tc = 0;
    scanf("%d", &N);
    while(N--) {
        scanf("%d %d", &a, &b);
        E[a] = E[b] = true;
        G = connect(G, a, b);
        tc = updatetc(tc, a, b);
    }

    dfsc(G, tc, 1, 0); // Solve

    // Print the sum of the domino digits
    printf("%d\n", sum(ansv) + sum(answ));

    // Print the number of dominoes in the answer
    int sz = 0;
    for(int i = 1; i <= 6; i++)
        for(int j = 1; j <= 6; j++)
            sz += set(ansv, i, j) + set(answ, i, j);
    printf("%d\n", sz);

    // Print the dominoes we picked
    for(int i = 1; i <= 6; i++) {
        for(int j = 1; j <= 6; j++) {
            if(set(ansv, i, j))
                printf("%d %d\n", i, j);
            if(set(answ, i, j))
                printf("%d %d\n", i, j);
        }
    }
}
