/* 1177. Like Comparisons - http://acm.timus.ru/problem.aspx?num=1177
 *
 * Strategy:
 * Dynamic programming over the position in the string and in the template.
 *
 * Performance:
 * O(Nmn) where m and n are the string and template lengths, runs the tests in 0.015s using 612KB.
 */

#include <stdio.h>
#include <cstring>

unsigned char s[101], t[101], B[1000]; // String, template, input
bool A[101][101]; // DP array

bool match() {
    int p = 1, n = 0;

    // Read the string
    for(; ;s[n++] = B[p++])
        if(B[p] == '\'')
            if(B[++p] != '\'') // Skip double apostrophes
                break;

    // Skip the middle 'like' part
    while(B[p++] != '\'');
    int m = 0;

    // Read the template
    for(; ;t[m++] = B[p++]) {
        if(B[p] == '\'')
            if(B[++p] != '\'')
                break;
        while(B[p] == '%' && B[p+1] == '%') // Remove double percentages
            p++;
    }

    std::memset(A, 0, sizeof(A));
    A[0][0] = true;

    for(int i = 0; i < n; i++) {
        for(int j = 0; j < m; j++) {
            if(!A[i][j]) // This state is not reachable
                continue;
            if(t[j] == '_') { // Any char matches this, next string/template pos is reachable
                A[i+1][j+1] = true;
            } else if(t[j] == '%') { // We can reach any remaining point in the string from this
                for(int x = i; x <= n; x++)
                    A[x][j+1] = true;
            } else if(t[j] == '[') {
                bool str8 = true, match = false; // Straight match or inverse, match?
                if(t[j+1] == '^') {
                    str8 = false;
                    j++;
                }
                int k = j+1;
                for(; k < m; k++) {
                    if(t[k] == ']')
                        break;
                    if(t[k+1] == '-' && t[k+2] != ']') { // "a-z"-like range
                        if(t[k] <= s[i] && s[i] <= t[k+2])
                            match = true;
                        k += 2;
                    } else if(t[k] == s[i]) // Verbatim char
                        match = true;
                }
                j = k;
                if(match == str8)
                    A[i+1][j+1] = true;
            } else if(s[i] == t[j]) { // Verbatim char, next pos in string and template reachable
                A[i+1][j+1] = true;
            }
        }
    }

    // Ignore any last % if we reached the pos before it in the template
    if(t[m-1] == '%' && A[n][m-1])
        A[n][m] = true;

    return A[n][m]; // This was a match if the end of the string and template is reachable
}

int main() {
    int N;
    scanf("%d", &N);
    gets_s((char*) B, 10000);
    while(N--) {
        gets_s((char*) B, 10000);
        printf(match() ? "YES\n" : "NO\n");
    }
}
