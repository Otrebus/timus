/* 2143. Victoria! - http://acm.timus.ru/problem.aspx?num=2143
 *
 * Strategy:
 * Dynamic programming over B[n][m] which represents the greatest number of passengers we can seat
 * in the plane from row n and forward with row n configured according to bitmask m. We update
 * B[n][m] for each m by iterating over B[n-1][m2] for all m2 that fit according to the given rules.
 *
 * Performance:
 * O(n), runs the tests in 0.031s using 228KB memory.
 */

#include <stdio.h>

int A[102]; // Existing passengers
int B[102][1<<6]; // DP array
int P[102][1<<6]; // Previous value on best path

int n, k;

bool set(int i, int m) { // Checks if bit i of mask m is set
    return i >= 0 && i <= 5 && (m & (1<<i));
}

bool test(int i, int m) { // Returns true if seat i can't be filled if the front row is bitmask m
    return set(i-1, m) && i != 3 || set(i, m) || set(i+1, m) && i != 2;
}

int count(int m) { // Counts the number of bits in m
    int ret = 0;
    for(int i = 0; i < 6; i++)
        ret += !!(m & (1 << i));
    return ret;
}

bool check(int m, int m2, int i) { // Checks that configuration m is valid with m2 above it
    if(m & A[i]) // Can't sit on occupied seats
        return false;
    for(int s = 0; s < 5; s++) // Can't have two seats next to each other except for the aisle
        if(s != 2 && (m & (3<<s)) == (3<<s))
            return false;
    for(int j = 0; j < 6; j++) // Can't sit close to someone in the front
        if((m & (1 << j)) && test(j, m2))
            return false;
    return true;
}

int main() {
    scanf("%d %d", &n, &k);
    for(int i = 1; i <= n; i++) { // Read each row of the input
        for(int j = 0; j < 6; j++) {
            char c = 0;
            while(c != '.' && c != '*')
                scanf("%c", &c);
            A[i] |= ((c == '*') << j);
        }
    }
    for(int i = 1; i <= n+1; i++) // Do the dp
        for(int m2 = 0; m2 < (1<<6); m2++) // For every config in the front
            for(int m = 0; m < (1<<6); m++) // For every config we try
                if(check(m, m2, i) && B[i][m] < count(m) + B[i-1][m2]) // If valid and better
                    P[i][m] = m2, B[i][m] = B[i-1][m2] + count(m);

    if(B[n+1][0] < k) // Check if the last virtual n+1 row with 0 passengers can have k in front
        return printf("PORAZHENIE"), 0;

    printf("POBEDA\n"); // Otherwise, use the P array to rewind the actual configuration
    int m = 0, r = 0;
    for(int i = n+1; i >= 1; m = P[i--][m])
        for(int j = 0; j < 6; j++)
            if((m & (1 << j)) && r < k)
                printf("%d%c\n", i, 'A'+j), r++;
}
