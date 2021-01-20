/* 1465. Pawn Game - https://acm.timus.ru/problem.aspx?num=1465
 *
 * Strategy:
 * Whenever we move a pawn, we split the game into two new games (or one game if we make the move
 * on the edge) by the three closest columns of the moved pawn. Since the games are combinatorial
 * impartial games, we can solve them using the Sprague-Grundy theorem. In this case, the Grundy
 * values become cyclical at some point for increasing game sizes, which allows us to calculate
 * them quickly for any large value of N.
 *
 * Performance:
 * Constant, runs the tests in 0.015s using 252KB memory.
 */

#include <iostream>
#include <cstring>

int A[102];

int solve(int n) {
    if (A[n] != -1)
        return A[n];

    bool B[128];
    std::memset(B, false, sizeof(B));

    int m = solve(n - 2);
    B[solve(n-2)] = true;
    for (int i = 1; i < n-1; i++) {
        B[solve(i - 1) ^ solve(n - i - 2)] = true;
    }
    for (int i = 0; i < 128; i++) {
        if (!B[i]) {
            A[n] = i;
            break;
        }
    }
    return A[n];
}

int main() {
    std::memset(A, -1, sizeof(A));
    A[0] = 0; A[1] = 1; A[2] = 1;

    int n;
    std::cin >> n;

    int a = (n < 68) ? solve(n) : solve(68 + n % 34);
    std:: cout << (a ? "White" : "Black");
    return 0;
}
