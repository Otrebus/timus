/* 1552. Brainf#$k - http://acm.timus.ru/problem.aspx?num=1552
 *
 * Strategy:
 * Dynamic programming - store the minimum cost in an array A[n][v0][v1][v2][p] where n denotes the
 * number of characters left to output, p denotes the current position of the pointer, which we also
 * assume contains the previous character that we output, and v0-v2 denotes the characters in the
 * remaining memory positions. We fill in the values naturally, moving from one character to the
 * next (over n), determining the optimal cost for the case of n characters remaining for some
 * memory content, from the optimal cost of n-1 characters remaining after having moved to a new
 * position and filled in some memory location.
 *
 * Performance:
 * O(nw(c+1)^(w-1)) where w is the number of memory positions used, c is the number of characters
 * and n is the length of the string, runs the tests in 0.156s using 11,892KB memory.
 */

#include <iostream>
#include <algorithm>
#include <string>

short A[51][27][27][27][4]; // DP array
char P[51][27][27][27][4]; // Previous memory position

int dif(int a, int b) {
    // The difference of the contents of memory locations as given in DP values a and b (since 0
    // corresponds to 0 and 1 corresponds to 'a' (and so on) the calculation is slightly more
    // involved than a straight difference)
    int x = std::min(a, b), y = std::max(a, b);
    return (y-x + (y && !x)*('a'-1))*(b > a ? 1 : -1);
}

int main() {
    std::string s;
    std::cin >> s;

    int N = s.length();
    std::reverse(s.begin(), s.end()); // We're iterating over characters remaining, so reverse
    std::memset(A, 0x3f, sizeof(A)); // DP values are infinite until calculated ...
    std::memset(A[0], 0, sizeof(A[0])); // ... except the end state (any state w/ 0 chars remaining)

    int v[3];
    for(int n = 1; n <= N; n++) { // From a single remaining character to the entire string
        for(v[0] = 0; v[0] < 27; v[0]++) { // For each possible memory content
            for(v[1] = 0; v[1] < 27; v[1]++) {
                for(v[2] = 0; v[2] < 27; v[2]++) {
                    for(int p = 0; p < 4; p++) { // For each position of the pointer
                        int w[4]; // The actual memory contents
                        w[p] = n < N ? 1+s[n]-'a' : 0; // Position p contains the last output char
                        for(int i = 0; i < 3; i++)
                            w[i+(i>=p)] = v[i]; // Fill in the actual memory (skip previous char)

                        // For the number of characters n remaining and the given memory content we
                        // move to each position and output the new character at that position,
                        // poll the previously calculated optimal value for that state and use that
                        // to update our state.
                        for(int q = 0; q < 4; q++) {
                            int z[3]; // The DP notation to poll
                            for(int i = 0; i < 3; i++)
                                z[i] = w[i+(i>=q)];
                            short& a = A[n][v[0]][v[1]][v[2]][p]; // Current optimal state
                            // Cost of moving q-p positions, incrementing the cell content to the
                            // new char and outputting the character
                            int b = std::abs(q-p) + std::abs(dif(w[q], 1 + s[n-1]-'a')) + 1;
                            if(b + A[n-1][z[0]][z[1]][z[2]][q] < a) {
                                // If the calculated value is better than the current optimum,
                                // update the optimum and record the movement that got us here
                                a = b + A[n-1][z[0]][z[1]][z[2]][q];
                                P[n][v[0]][v[1]][v[2]][p] = q;
                            }
                        }
                    }
                }
            }
        }
    }

    // Find the optimum position of the pointer to start from (or rather the optimum way
    // to number the memory locations)
    int m = 1e9, p = 0, c[3] = { 0, 0, 0 };
    for(int i = 0; i < 4; i++)
        if(A[N][0][0][0][i] < m)
            m = A[N][0][0][0][i], p = i;

    // Run through the dp state from character to character and output
    for(int n = N; n > 0; n--) {
        int q = P[n][c[0]][c[1]][c[2]][p];

        int w[4]; /// The actual memory content
        w[p] = n == N ? 0 : 1+s[n]-'a';
        for(int i = 0; i < 3; i++)
            w[i+(i>=p)] = c[i];

        // If we moved the pointer, output that
        for(;p < q; p++)
            std::cout << ">";
        for(;p > q; p--)
            std::cout << "<";

        int x = dif(w[p], s[n-1]-'a'+1); // Number of increments/decrements

        // The DP notation of the memory location
        w[p] = s[n-1]-'a'+1;
        for(int i = 0; i < 3; i++)
            c[i] = w[i+(i>=p)];

        // Output the increment and output
        std::cout << std::string(std::abs(x), x > 0 ? '+' : '-');
        std::cout << ".";
    }
}
