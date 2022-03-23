/* 1421. Credit Operations - http://acm.timus.ru/problem.aspx?num=1421
 *
 * Strategy:
 * Treat the problem as a max-flow problem where we use the stated credits as capacities where we
 * need to maximize the amount of credit that flows from the banks to the enterprises, and solve
 * the network using a barebones push-relabel algorithm.
 *
 * Performance:
 * O(N^4), runs the tests in 0.078s using 620KB memory.
 */

#include <iostream>
#include <unordered_set>

int N;
int C[202][202];
int F[202][202];
int e[202];
int h[202];

struct hash {
    template <class T1, class T2>
    std::size_t operator() (const std::pair<T1, T2>& pair) const {
        return std::hash<T1>()(pair.first) ^ std::hash<T2>()(pair.second);
    }
};

std::unordered_set<std::pair<int, int>, hash> p;
std::unordered_set<int> ex;

bool isAug(int i, int j) {
    return F[i][j] < C[i][j] || F[j][i];
}

bool push() {
    if (p.empty())
        return false;
    auto pair = *p.begin();
    auto i = pair.first, j = pair.second;
    if (C[i][j]) {
        auto a = C[i][j] - F[i][j];
        int r = std::min(e[i], a);
        F[i][j] += r;
        e[i] -= r;
        e[j] += r;
    }
    else {
        auto a = F[j][i];
        int r = std::min(e[i], a);
        e[i] -= r;
        e[j] += r;
        F[j][i] -= r;
    }
    if (e[j] && j != N*2 + 1 && j != 0)
        ex.insert(j);
    if (e[i] && i != N*2 + 1 && i != 0)
        ex.insert(i);
    p.erase(pair);
    return true;
}

bool relabel() {
    if (ex.empty())
        return false;

    auto i = *ex.begin();

    int m = 1000000;
    for (int j = 0; j < 2*N + 2; j++)
        if (isAug(i, j))
            m = std::min(h[j], m);
    h[i] = m + 1;
    if (i > N) {
        int e = N*2 + 1;
        if (h[i] == h[e] + 1 && isAug(i, e))
            p.insert({ i, e });
        for (int j = 0; j <= N; j++) {
            if (h[i] == h[j] + 1 && isAug(i, j)) {
                p.insert({ i, j });
                break;
            }
        }
    }
    else {
        if (h[i] == h[0] + 1 && isAug(i, 0))
            p.insert({ i, 0 });
        for (int j = N; j < 2*N + 2; j++) {
            if (h[i] == h[j] + 1 && isAug(i, j)) {
                p.insert({ i, j });
                break;
            }
        }
    }
    ex.erase(i);
    return true;
}

void solve() {
    h[0] = 2*N + 2;
    for (int i = 0; i < 2*N + 2; i++) {
        F[0][i] = C[0][i];
        e[0] -= C[0][i];
        e[i] += C[0][i];
        if (F[0][i])
            ex.insert(i);
    }

    do {
        while (push());
    } while (relabel());
}

int main() {
    std::cin >> N;

    int A[100], B[100];

    for (int i = 0; i < N; i++)
        std::cin >> A[i];

    for (int i = 0; i < N; i++)
        std::cin >> B[i];

    for (int i = 0; i < N; i++)
        // Connect the source with the column of enterprises with their received credits
        C[0][i+1] = A[i];

    for (int i = 0; i < N; i++)
        // Connect the sink to the column of banks with the credits as weights
        C[1+N+i][1+2*N] = B[i];

    for (int i = 1; i <= N; i++)
        // Knit together every bank with every enterprise with the maximum possible credit
        for (int j = 1 + N; j <= 2*N; j++)
            C[i][j] = 100;

    solve();

    for (int i = 0; i < N; i++)
        // Win if each bank and each enterprise were given their input/output in the maximum flow
        if (F[0][i+1] != A[i] || F[1+N+i][1+2*N] != B[i]) {
            std::cout << "NO" << std::endl;
            return 0;
        }

    std::cout << "YES" << std::endl;
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            // Output the flow between each bank and each credit
            std::cout << F[i+1][1+N+j] << " ";
        }
        std::cout << std::endl;
    }
}
