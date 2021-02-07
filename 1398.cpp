/* 1398. Bishop and Pawn - http://acm.timus.ru/problem.aspx?num=1398
 *
 * Strategy:
 * Minimax + memoization.
 *
 * Performance:
 * Constant, runs the tests in 0.015s using 424KB memory.
 */

#include <iostream>

int W[4][64][64];
bool visiting[4][64][64];

const int WHITE = 1, DRAW = 2, BLACK = 3;

int move(int turn, int wp, int bp) {

    if(W[turn][wp][bp])
        return W[turn][wp][bp];
    if(visiting[turn][wp][bp])
        return 0;
    visiting[turn][wp][bp] = true;

    int xw = wp/8, yw = wp%8;
    int xb = bp/8, yb = bp%8;
    int result;
    if (turn == BLACK) {
        if(std::abs(xw - xb) == 1 && yw == yb - 1 || yb == 0)
            // Pawn captures
            result = BLACK;
        else if(xw == xb && yw == yb - 1)
            // Pawn cannot move
            result = DRAW;
        else
            // Move north
            result = move(WHITE, wp, bp - 1);
    }
    else if(turn == WHITE) {
        if(std::abs(xw - xb) == std::abs(yw - yb))
            // Bishop captures
            result = WHITE;
        else {
            result = 100;

            // Move to every possible square
            for(int i = -1; i <= 1; i += 2) {
                for(int j = -1; j <= 1; j += 2) {
                    for(int d = 1; d < 8; d++) {
                        int x = xw + i*d;
                        int y = yw + j*d;
                        if(0 <= x && x < 8 && 0 <= y && y < 8) {
                            int r = move(BLACK, x*8 + y, bp);
                            if(r)
                                result = std::min(result, r);
                        }
                    }
                }
            }
        }
    }
    visiting[turn][wp][bp] = false;
    if(result >= WHITE && result <= BLACK) {
        W[turn][wp][bp] = result;
        return result;
    }
    return 0;
}

int main() {
    char a[3], b[3];
    std::cin >> a >> b;
    int result = move(WHITE, (a[0]-'a')*8 + a[1]-'1', (b[0]-'a')*8 + b[1]-'1');
    std::cout << (result == 1 ? "WHITE" : result == 2 ? "DRAW" : "BLACK");
}
