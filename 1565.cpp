/* 1565. The Duel for Three - http://acm.timus.ru/problem.aspx?num=1565
 *
 * Strategy:
 * Straightforward probability calculations. The decision to be made for each human duelist
 * (duelists B and C), is whether to shoot in the air or at the T-1000 (duelist A), since if any
 * human shoots the other human, the Terminator would then eliminate him. See comments for details.
 *
 * Performance:
 * O(1), Runs the tests in 0.015s using 248KB memory.
 */

#include <iostream>
#include <algorithm>
#include <iomanip>

using ld = long double;

struct ans { // A vector of three probabilities
    ld v[3];
    ans operator+=(const ans& A) { return *this = { { A.v[0]+v[0], A.v[1]+v[1], A.v[2]+v[2] } }; };
    ans operator/(ld a) const { return { v[0]/a, v[1]/a, v[2]/a }; };
};

ans calc(ld, ld, ld);
ld calc(ld, ld);

ans calc(ld i, ld j, ld k) {
    // Calculates the probabilities of winning for players with skills i, j and k respectively
    // and shooting in that order
    if(i == 1) { // The first player is the Terminator
        // We kill the strongest opponent; next guy has a chance of winning equal to his accuracy
        return j > k ? ans { 1-k, 0, k } : ans { 1-j, j, 0 };
    } else if(j == 1) { // Duelist 2 is the killer robot
        ans a = calc(j, k, i); // If i shoots into the air (or misses)
        ld b = calc(k, i); // If i shoots at duelist #2 and hits, it becomes a duel of two people

        if(a.v[2] > i*(1-b) + (1-i)*a.v[2]) { // If it's better to shoot into the air, do so
            return { a.v[2], a.v[0], a.v[1] };
        } else { // Otherwise, take a shot at the Terminator
            return { i*(1-b) + (1-i)*a.v[2], (1-i)*a.v[0], i*b + (1-i)*a.v[1] };
        }
    } else { // Duelist 3 is the superhuman AI, this case is similar
        ans a = calc(j, k, i);
        ld b = calc(j, i);

        if(a.v[2] > i*(1-b) + (1-i)*a.v[2]) {
            return { a.v[2], a.v[0], a.v[1] };
        } else {
            return { i*(1-b) + (1-i)*a.v[2], i*b + (1-i)*a.v[0], (1-i)*a.v[1] };
        }
    }
}

ld calc(ld i, ld j) {
    // The probability of duelist with skill i winning against duelist with skill j
    // Derivation: Let A_i and A_j be the chance of winning the duel for each duelist, then we have
    //  A_i = i + (1-i)*(1-A_j)
    //  A_j = j + (1-j)*(1-A_i) [or 1 - A_i]
    // and can solve for A_i.
    return i/(1 - (i-1)*(j-1));
}

int main() {
    ld in[3];
    std::cin >> in[0] >> in[1] >> in[2];
    ans an = { 0, 0, 0 }; // Output

    // Try every permutation the duelist order
    int v[3] = { 0, 1, 2 }, w[3]; // The permutation and the inverse 
    do {
        for(int i = 0; i < 3; i++)
            w[v[i]] = i; // Calculate the inverse map (player -> position)
        auto a = calc(in[v[0]], in[v[1]], in[v[2]]);
        an += {{ a.v[w[0]], a.v[w[1]], a.v[w[2]] }};
    } while(std::next_permutation(v, v+3));

    an = an/6;
    std::cout << std::fixed << std::setprecision(10) << an.v[0] << " " << an.v[1] << " " << an.v[2];
}
