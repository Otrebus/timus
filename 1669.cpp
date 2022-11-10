/* 1669. Universal Word - http://acm.timus.ru/problem.aspx?num=1669
 *
 * Strategy:
 * Dynamic programming where we store the smallest prefix length that we can solve the problem for
 * involving a certain subset (a bitmask) of the input alphabet. For any subset we generate the
 * solution by, for each character, generating the solution (prefix length) for the subset without
 * that character, and noting the leftmost place we can place that character in the remaining
 * suffix (meaning, for each character, we put that character at the end of every permutation of
 * the rest of the subset). The overall solution for the subset mask is the minimum of all those
 * leftmost places over all the characters.
 *
 * Performance:
 * O(N*2^N), runs the tests in 0.109s using 12,288KB memory.
 */

#include <unordered_map>
#include <stdio.h>

std::unordered_map<int32_t, int16_t> L; // Maps a subalphabet to its smallest solution prefix length

short E[302]['z'-'a'+1]; // The index of the closest character to the right at this index
char S[301];
int N, len;
const int inf = 0xfff;

int16_t dfs(int32_t m) {
    if(m == 0)
        return 0;

    if(auto it = L.find(m); it != L.end())
        return it->second;

    int b = 0, B = 0; // "Leftmost place" of a character, and overall "leftmost place"
    for(int j = 0; j < N && b != inf; j++) // For each character to replace
        if(m & (1 << j)) // If it's part of the mask
            if((b = dfs(m ^ (1 << j))) != inf) // Generate subsolution without it
                B = std::max(B, b = E[b][j]); // Note the first position of the char in the suffix
    return L[m] = std::max(B, b);
}

int main() {
    scanf("%d %s", &N, S), len = strlen(S);

    // Precalculate so we can quickly find the first position of a char in a suffix of a string
    std::fill((short*) E, (short*) E+sizeof(E)/sizeof(short), 0xfff);
    for(int i = len-1; i >= 0; i--)
        for(int c = 0; c <= 'z'-'a'; c++)
            E[i][c] = (S[i] == c + 'a') ? i : E[i+1][c];

    printf(dfs((1 << N) - 1) < inf ? "YES" : "NO");
}
