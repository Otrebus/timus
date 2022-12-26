/* 1549. Another Japanese Puzzle - http://acm.timus.ru/problem.aspx?num=1549
 *
 * Strategy:
 * Start with a path of just LLLL, and expand. It's helpful to think of the path of having four
 * sides, two vertical (west, east) and two horizontal (north, south). Since we need to preserve
 * the final direction the path is heading along each side (between the L's), there are only a
 * few different combinations of pieces that we can choose to expand with. The basic unit is the
 * RL combination which maintains the direction but displaces the endpoints one step horizontally
 * and vertically, which we can then compensate for in a few different ways:
 * 
 *   1) Adding an LR block to the same side, with an RLLR combination on the other side
 *   2) Adding an LR block on the same side, with a vertical block, a horizontal block and an
 *      RL combination on the other side.
 *   3) Adding an LR block on the same side, with a couple of vertical blocks on the the other side
 *   4) Adding an LR block on the other side (only possible without overlap if there are already
 *      any straight segments on the path)
 *   5) Adding a vertical and horizontal straight block.
 * 
 * After using all the T blocks like this, we can add all remaining S blocks pairwise as either
 * vertical or horizontal blocks on opposing sides.
 *
 * Performance:
 * O(T+S), runs in 0.031s using 48KB memory.
 */

#include <iostream>
#include <string>

int main() {
    // Number of RLLR (on the East or West sides) combos, F blocks (E, W, N, S) or LR (E, W) combos
    int e4 = 0, w4 = 0, eV = 0, wV = 0, nH = 0, sH = 0, e2 = 0, w2 = 0;
    int S, T;
    std::cin >> S >> T;

    if(T < 4) // Need at least four
        return printf("Atawazu"), 0;

    T -= 4; // The initial four

    // 1) As many RLLR combos on the west and east sides as we can
    if(T/8) {
        e4 += T/8, w4 += T/8;
        T = T%8;
    } 

    // 2) RLLR on the west side together with an LR on the east side with a horizontal
    // and vertical straight segment
    if(T >= 6 && S >= 2) {
        w4++, e2++, eV += 1, sH += 1;
        S -= 2, T -= 6;
    }

    // 3) RLLR on the west side and a couple straight segments on the east side
    if(T >= 4 && S >= 2) {
        w4++, eV += 2;
        S -= 2, T -= 4;
    }

    // 4) RL on the west and east sides
    if(T >= 4 && (e4 || nH)) {
        e2++, w2++;
        T -= 4;
    }

    // 5) RL on the west side together with a vertical and horizontal line segment
    if(T >= 2 && S >= 2) {
        w2++;
        nH++;
        eV++;
        T -= 2, S -= 2;
    }
    
    eV += S/2, wV += S/2; // The remaining straight ones

    std::string ans;

    // Turn left along the east side
    ans += "L";
    while(e4--)
        ans += "RLLR";
    while(e2--)
        ans += "LR";
    while(eV--)
        ans += "F";

    // Turn left along the north side
    ans += "L";
    while(nH--)
        ans += "F";

    // Turn left along the west side
    ans += "L";
    while(w4--)
        ans += "RLLR";
    while(wV--)
        ans += "F";
    while(w2--)
        ans += "LR";

    // Turn left along the south side
    ans += "L";
    while(sH--)
        ans += "F";

    std::cout << ans.length() << std::endl << ans;
}
