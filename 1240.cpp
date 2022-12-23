/* 1240. Heroes of Might and Magic - http://acm.timus.ru/problem.aspx?num=1240
 *
 * Strategy:
 * Depth-first search through the game space where we memoize the player's health and mana points
 * and the position and health of the monsters.
 *
 * Performance:
 * Runs the test cases in 0.156s using 3,908KB memory.
 */

#include <stdio.h>
#include <tuple>
#include <unordered_set>
#include <vector>

int N, Hp_Hmax, Mp_H, Hp_M, N_M, V, dH;
int HpM;
int L[11];
bool A[11][101][51][101]; // The memo array

bool solve(int p, int Hp_H, int Mp_H, int Hp_M, std::vector<std::pair<char, int>>& s) {
    if(A[N][Hp_H][Mp_H][Hp_M])
        return false;

    if(Hp_M <= 0)
        return true;

    int Hp2 = Hp_H;
    if(!s.empty() && p == 1) // We get hit
        Hp2 -= 1+((Hp_M-1)/HpM);

    if(Hp2 <= 0 || Mp_H <= 0) // If we have no health or mana, we're cooked
        return false;

    // Cast lightning
    s.push_back({ 'L', 0 });
    if(solve(std::max(1, p-V), Hp2, Mp_H-1, Hp_M-L[p], s))
        return true;
    s.pop_back();

    // Teleport
    for(int i = 1; i <= N; i++) {
        s.push_back({ 'T', i });
        if(solve(std::max(1, i-V), Hp2, Mp_H-1, Hp_M, s))
            return true;
        s.pop_back();
    }

    // Heal
    s.push_back( { 'H', 0 } );
    if(solve(std::max(1, p-V), std::min(Hp_Hmax, Hp2+dH), Mp_H-1, Hp_M, s))
        return true;
    s.pop_back();

    A[N][Hp_H][Mp_H][Hp_M] = true;
    return false;
}


int main() {
    scanf("%d %d %d %d %d %d %d", &N, &Hp_Hmax, &Mp_H, &HpM, &N_M, &V, &dH);
    for(int i = 1; i <= N; i++)
        scanf("%d", L+i);

    std::vector<std::pair<char, int>> s;
    if(!solve(N, Hp_Hmax, Mp_H, N_M*HpM, s))
        return printf("DEFEATED"), 0;

    printf("VICTORIOUS\n");
    for(auto p : s) {
        if(p.first == 'T')
            printf("%c %d\n", p.first, p.second);
        else
            printf("%c\n", p.first);
    }
}
