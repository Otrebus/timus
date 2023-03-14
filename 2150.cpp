/* 2150. 4B and Zoo - http://acm.timus.ru/problem.aspx?num=2150
 *
 * Strategy:
 * Brute-force. Connect each child with some random unconnected friend, and at the end connect
 * friendless children to any two other children who have them as a friend.
 *
 * Performance:
 * Runs the tests in 0.765s using 4,656KB memory.
 */

#include <stdio.h>
#include <vector>
#include <random> 
#include <numeric>

int N;
int A[1000][1000]; // Friendship graph
bool P[1000];

struct ans { // A friend group
    int a, b, c;
};

std::vector<ans> v; // The output
std::vector<int> w, q; // For random picking

bool find3(int i) { // For child i, find some group to join
    for(int j = 0; j < v.size(); j++) {
        auto a = v[j].a, b = v[j].b, c = v[j].c;
        if(c == -1 && a != i && b != i && A[a][i] && A[b][i]) {
            v[j] = { a, b, i };
            return true;
        }
    }
    return false;
}

std::default_random_engine g;

bool solve() {
    v.clear();
    std::memset(P, 0, sizeof(P));
    
    std::shuffle(w.begin(), w.end(), g);
    std::shuffle(q.begin(), q.end(), g);

    // Connect friends in groups of two
    for(int b = 0; b < N; b++) {
        int i = w[b];
        for(int a = 0; a < N; a++) {
            int j = w[a];
            if(A[i][j] && !P[i] && !P[j]) {
                P[i] = P[j] = true;
                v.push_back({ i, j, -1 });
            }
        }
    }

    // Connect the third friend
    for(int i = 0; i < N; i++)
        if(!P[i])
            if(!find3(i))
                return false;
    return true;
}

int main() {
    std::random_device rd;
    g = std::default_random_engine(rd()); 
    
    scanf("%d", &N);
    for(int i = 0; i < N; i++)
        for(int j = 0; j < N; j++)
            scanf("%d", &A[i][j]);

    v.clear();
    w.resize(N);
    q.resize(N);
    std::iota(w.begin(), w.end(), 0);
    std::iota(q.begin(), q.end(), 0);

    while(!solve());

    printf("YES\n");
    printf("%d\n", v.size());
    for(auto a : v)
        printf(a.c == -1 ? "2 %d %d\n" : "3 %d %d %d\n", a.a+1, a.b+1, a.c+1);
}
