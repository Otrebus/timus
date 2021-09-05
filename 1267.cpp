/* 1267. Yekaterinburg Subway - http://acm.timus.ru/problem.aspx?num=1267
 *
 * Strategy:
 * We dfs the stations, memoizing over
 * 
 *   t = A[M][i][d][p]
 * 
 * where
 *   t: The time it takes to visit the remaining stations assuming we can immediately board
 *      a train in direction d at station i
 *   M: The stations visited so far as a bitmask
 *   i: The current station
 *   d: The direction in which we will depart this station
 *   p: Whether t is in the periodic part of the timetable of the trains heading in direction dir
 * 
 * Performance:
 * Something like O(N^2*2^N), runs the tests in 0.109s using 16,668KB memory.
 */

#include <iostream>

int A[1<<16][16][2][2];
int T[17]; // The (cumulative) time it takes to reach some station from station 0

int N, P, x, s, t0, t1;

int fst(int t, int i, bool dir) {
    // Returns the first train leaving in direction dir from station t after time t
    auto solve = [] (int A, int t) { return std::max(A, A + P*(1+(t-A-1)/P)); };
    return dir ? solve(t0 + (T[i] - T[0]), t) : solve(t1 + (T[N-1] - T[i]),  t);
}

int dfs(int M, int t, int i, int dir) {
    // Returns the time we finish the roundtrip having visited stations in bitmask M, being at
    // time t, at station i, intending to go in direction dir and the train leaving right now
    M |= 1 << i;
    int min = 1e9;
    int t2 = dir ? t1 + T[N-1] - T[i] : t0 + T[i] - T[0]; // Is t in the periodic part?
    if(A[M][i][dir][t>=t2] >= 0) // Already memoed
        return A[M][i][dir][t>=t2];
    if((M == ((1 << N)-1)) && i != s-1) // All stations visited, need to return
        return (i < s-1) == dir ? std::abs(T[i]-T[s-1]) : 1e9;

    for(int j = 0; j < N; j++) {
        if(~M & (1 << j) && (j > i) == dir) { // For each station j in the right dir
            int dt = std::abs(T[i]-T[j]); // The time it takes to reach it
            for(int dir = 0; dir < 2; dir++) { // For each direction
                int td = fst(t+dt+1, j, dir); // When does train leave from j in the given dir?
                min = std::min(min, td - t + dfs(M, td, j, dir));
            }
        }
    }
    return A[M][i][dir][t>=t2] = min;
}

int main() { 
    std::cin >> N;
    std::memset(A, 0xff, sizeof(A));
    for(int i = 0; i < N-1; i++) {
        std::cin >> T[i+1];
        T[i+1] += T[i];
    }
    std::cin >> s >> P >> t0 >> t1;
    int tl = fst(0, s-1, false), tr = fst(0, s-1, true);
    // If N = 0, the answer is 0, otherwise explore starting in both directions from s
    std::cout << ((N == 1) ? 0 : std::min(dfs(0, tl, s-1, 0), dfs(0, tr, s-1, 1)));
}
