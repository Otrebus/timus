/* 1921. Chaos and Order - http://acm.timus.ru/problem.aspx?num=1921
 *
 * Strategy:
 * The canonical pattern looks like
 * 
 *   AABBAABB
 *   BBAABBAA
 *   AABBAABB
 *   BBAABBAA
 * 
 * which can provide any number between 2 and max(m, n) successive letters if we fill the top row
 * or column with A's as needed, possibly after shifting the pattern a few steps. Apart from this,
 * there are only a few special cases: For no successions (l=2), we can use the pattern
 * 
 *   ABABABAB
 *   CDCDCDCD
 * 
 * and we can make do with only two letters for single rows/columns. The case k=1 is a trivial
 * check.
 * 
 * Performance:
 * O(nm), runs the tests in 0.015s using 144KB memory.
 */

#include <stdio.h>
#include <algorithm>

int n, m, k, l;
bool swap = false;

int getch(int x, int y) { // Returns the letter of the given coordinate in the canonical case
    return (k == 1 || y == 0 && x < l || (x + y*2)/2%2) ? 'A' : 'B';
}

void preout() { // Helper for the output functions
    if(swap)
        std::swap(n, m);
    printf("YES\n");
}

void output(int r, bool swap) { // Outputs the canonical case (pattern offset r)
    preout();
    for(int i = 0; i < n; i++, printf("\n"))
        for(int j = 0; j < m; j++)
            printf("%c", swap ? getch(i+r, j) : getch(j+r, i));
}

void output2(int swap) { // Outputs the case for l=2
    preout();
    for(int i = 0; i < n; i++, printf("\n"))
        for(int j = 0; j < m; j++)
            printf("%c", 'A' + i%2*2 + j%(std::max(1, std::min(4, k)-2)));
}

void output3(int swap) { // Outputs a single row
    preout();
    for(int i = 0; i < n; i++, printf("\n"))
        for(int j = 0; j < m; j++)
            printf("%c", 'A' + (i + j)%k);
}

int main() {
    scanf("%d %d %d %d", &n, &m, &k, &l);
    l--;
    if(n > m) // To simplify calculations we have the x axis longest
        std::swap(n, m), swap = true;

    if(k > 1 && l > 1) {
        int r = 0, j; // Pattern offset, x index
        for(;r < 4; r++) { // Find the correct way to fill the top row
            for(j = 0; j < m && getch(r + j, 0) == 'A'; j++);
            if(j == l)
                return output(r, swap), 0;
        }
    } else if(k == 1) { // Single input letter - make sure l fits exactly
        if(l == std::max(n, m))
            return output(0, swap), 0;
    } else if(l == 1) { // Only one successive letter and k > 1, we need k >= 4 or a single row
        if(k >= 4)
            return output2(swap), 0;
        else if(std::min(n, m) == 1)
            return output3(swap), 0;
    }
    printf("NO");
}
