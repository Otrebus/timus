/* 1970. 皇后像廣場 - http://acm.timus.ru/problem.aspx?num=1970
 *
 * Strategy:
 * Brute-force - represent the sides of the squares by 32-bit integers and try to match them
 * in every combination and rotation.
 *
 * Performance:
 * Runs the tests in 0.015 using 412KB memory.
 */

#include <iostream>

int Q[9][4][4]; // The input squares
int hash[9][5]; // The representation of the edges
int hashr[9][5]; // Reverse representation for comparisons after rotations
int ans[9]; // The positions of the squares in the answer
int rot[9]; // The rotations of the squares in the answer
bool picked[9]; // The positions we've tried so far in the search

int xrot(int x, int y, int r) {
    // Returns the new x coordinate of a position in a 4x4 picture that was rotated r times
    if(!r)
        return x;
    return xrot(y, 3-x, r-1);
}

int yrot(int x, int y, int r) {
    // See above
    if(!r)
        return y;
    return yrot(y, 3-x, r-1);
}

bool solve(int p) {
    // Tries to find some combination of squares starting at position i in the image, (0 <= i < 9)
    int x = p%3, y = p/3;
    if(p == 9)
        return true;

    for(int i = 0; i < 9; i++) {
        // For each square
        if(picked[i])
            continue;

        for(int r = 0; r < 4; r++) {
            // For each rotation

            int pl = x-1 + y*3, pu = x + (y-1)*3;
            if(x > 0 && hash[ans[pl]][rot[pl]] != hashr[i][(2 + r) % 4])
                continue; // Doesn't match the square to the left
            if(y > 0 && hash[ans[pu]][(3 + rot[pu])%4] != hashr[i][(1 + r) % 4])
                continue; // Doesn't match the square above

            ans[p] = i;
            rot[p] = r;
            picked[i] = true;
            bool b = solve(p + 1); // Try adding another square to the right (wrapped)
            if(b)
                return b;
            ans[p] = -1;
            rot[p] = -1;
            picked[i] = false;
        }
    }
    return false;
}

int main() {

    for(int i = 0; i < 9; i++) {    
        for(int y = 0; y < 4; y++) {
            for(int x = 0; x < 4; x++) {
                std::cin >> Q[i][x][y];
            }
        }
        for(int j = 0; j < 4; j++) {
            // Put together the numbers on the edges into single numbers
            hash[i][0] += Q[i][3][3-j] << (j*8);
            hash[i][1] += Q[i][3-j][0] << (j*8);
            hash[i][2] += Q[i][0][j] << (j*8);
            hash[i][3] += Q[i][j][3] << (j*8);

            // Also in the reverse bit direction so we can compare edges
            hashr[i][0] += Q[i][3][j] << (j*8);
            hashr[i][1] += Q[i][j][0] << (j*8);
            hashr[i][2] += Q[i][0][3-j] << (j*8);
            hashr[i][3] += Q[i][3-j][3] << (j*8);
        }
    }

    solve(0);
    int out[10][10];

    // Put together the final image
    for(int i = 0; i < 9; i++) {
        int x = (i%3)*4, y = (i/3)*4;
        x -= x/3; y -= y/3;
        for(int y2 = 0; y2 < 4; y2++)
            for(int x2 = 0; x2 < 4; x2++)
                out[x+x2][y+y2] = Q[ans[i]][xrot(x2, y2, rot[i])][yrot(x2, y2, rot[i])];
    }

    // Print it
    for(int y = 0; y < 10; y++) {
        for(int x = 0; x < 10; x++) {
            std::cout << out[x][y] << " ";
        }
        std::cout << std::endl;
    }
}
