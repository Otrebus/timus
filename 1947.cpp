/* 1947. Meander - http://acm.timus.ru/problem.aspx?num=1947
 *
 * Strategy:
 * Straightforward evaluation/dfs according to the rules given.
 *
 * Performance:
 * Runs the tests in 0.015s using 132KB memory.
 */

#include <stdio.h>
#include <algorithm>

int dir[4][2] = { { 1, 0 }, { 0, -1 }, { -1, 0 }, { 0, 1 } }; // Maps dir value to dx/dy

int board[5][5]; // The board
int board2[5][5]; // Temporary storage to revert
int t[2][4] = { { 1, 0, 3, 2 }, { 3, 2, 1, 0 } }; // Maps tile type and direction to new direction

bool dfs(int x, int y, int d, int l, int s) {
    // Is there a path entering the square (x,y) in direction d (original dir s) with depth l?
    if((x < 0 || x > 4 || y < 0 || y > 4))
        return (d+2)%4 != s && l > 1; // Winning condition
    if(board[x][y] == 2) // Empty square
        return false;
    int d2 = t[board[x][y]][d]; // Find out the new direction and recurse
    int dx = dir[d2][0], dy = dir[d2][1];
    return dfs(x+dx, y+dy, d2, l+1, s);
}

bool win() {
    // Checks if the current board wins by trying entry from every side
    for(int x = 0; x < 5; x++)
        if(dfs(x, 0, 3, 0, 3) || dfs(x, 4, 1, 0, 1))
            return true;
    for(int y = 0; y < 5; y++)
        if(dfs(0, y, 0, 0, 0) || dfs(4, y, 2, 0, 2))
            return true;
    return false;
}

int main() {
    char s[6];
    int ex, ey;
    for(int y = 0; y < 5; y++) { // Read the input
        scanf("%s", s);
        for(int x = 0; x < 5; x++) {
            board[x][y] = s[x] == '/' ? 0 : s[x] == '\\' ? 1 : 2;
            if(board[x][y] == 2)
                ex = x, ey = y; // Empty square location
        }
    }
    std::memcpy(board2, board, sizeof(board));
    if(win())
        return printf("WIN"), 0;

    for(int dd : { -1, 1 }) { // Move the empty square down/up
        std::memcpy(board, board2, sizeof(board2));
        for(int dy = 0; ey+dy+dd < 5 && ey+dy+dd >= 0; dy = dy + dd) {
            std::swap(board[ex][ey+dy], board[ex][ey+dy+dd]);
            if(win())
                return printf("%c%d", dd > 0 ? 'U' : 'D', std::abs(dy+dd)), 0;
        }
    }
    for(int dd : { -1, 1 }) { // Move the empty square left/right
        std::memcpy(board, board2, sizeof(board2));
        for(int dx = 0; ex+dx+dd < 5 && ex+dx+dd >= 0; dx = dx + dd) {
            std::swap(board[ex+dx][ey], board[ex+dx+dd][ey]);
            if(win())
                return printf("%c%d", dd > 0 ? 'L' : 'R', std::abs(dx+dd)), 0;
        }
    }
    printf("FAIL");
}
