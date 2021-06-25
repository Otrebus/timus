/* 1784. Rounders - http://acm.timus.ru/problem.aspx?num=1784
 *
 * Strategy:
 * The most efficient operation we can perform is to move a set of cards of the same suit from its
 * starting pile to one of the four piles that has a matching suite on top, and the only time we
 * benefit from building a pile outside the four main piles is when we are unable to do the above.
 * The canonical set of operations is to start a new deck with a card if its underlying card is of
 * another suit, then fill its old position with a card from another pile that matches suit, and
 * repeat. This takes n+1 operations where n is the number of cards in the chain. We can then
 * repeat this for each chain of the row, and then for each row of the four piles since all the
 * cards above the current row have been processed to consist of the same suit, and can be regarded
 * as a single card by the given rules.
 *
 * Performance:
 * Constant, runs the tests in 0.015s using 156KB memory.
 */

#include <stdio.h>

char D[4][13]; // The cards
bool v[4][13]; // Cards visited as we iterate a chain

int main() { // A bit code-golfed, sorry
    for(int i = 0; i < 4; i++)
        for(int j = 0; j < 13; j++)
            scanf("%*c%c%*c", &D[i][j]);

    int s = 0;
    for(int j = 0; j < 12; j++) // For each row
        for(int i = 0; i < 4; i++) // For each card
            if(D[i][j] != D[i][j+1] && !v[i][j] && ++s) // If suits are different and not visited
                for(int k = i; !v[k][j] && (v[k][j] = true); s++) // Traverse the chain
                    for(int h = 0, c = D[k][j+1]; h < 4; h++) // Find the next card in the chain
                        if(D[h][j] == c)
                            k = h;
    printf("%d", s);
}
