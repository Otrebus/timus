/* 1130. Nikifor's Walk - http://acm.timus.ru/problem.aspx?num=1130
 *
 * Strategy:
 * For any three vectors of length L, we can always pick two of the vectors or their inverse to
 * add together to produce another vector of length L. We can apply this recursively (keeping track
 * of the additions using a tree) until only two vectors remain, and then combine the remaining two
 * to produce a vector of length sqrt(2)L.
 *
 * Performance:
 * O(n), runs the tests in 0.031s using 1,016KB memory.
 */

#include <iostream>
#include <vector>
#include <stdio.h>

struct vec { 
    int x, y;
    vec operator+ (const vec& v) const { return { x+v.x, y+v.y }; }
    vec operator-() const { return { -x, -y }; }
    int l2() { return x*x + y*y; }
} vs[10001];

struct node { // Represents the addition of two vectors
    int p[2]; // The two vector representations we add
    int s[2]; // The sign of the above
    vec v;    // The resulting vector
} nodes[20002];

int s[10001]; // The roots of the tree
int m, p;     // Number of nodes and roots in use
int n, L; // The input

char out[10001]; // The +-+---+- of the output

void merge() { // Merges two of the leftmost three unmerged roots
    int k = std::max(1, p - 3); // The first of the three vectors
    for(int i = 1; i <= 2; i++) { // The second vector we try to combine it with
        for(int a = 0; a < 2; a++) { // Sign of the first vector
            for(int b = 0; b < 2; b++) { // Sign of the second vector
                auto x = a ? -nodes[s[k]].v : nodes[s[k]].v;
                auto y = b ? -nodes[s[k+1]].v : nodes[s[k+1]].v;
                auto w = x + y; // Combine the vectors
                if(w.l2() <= (p == 3 ? 2*L*L : L*L)) { // Check that they satisfy the length req
                    // If so, merge them, put the new root in the tree and remove the merged ones
                    auto nd = node { { s[k], s[k+1] }, { a, b }, w };
                    nodes[m] = nd;
                    s[k] = m++;
                    s[k+1] = s[k+2];
                    p--;
                    return;
                }
            }
        }
        std::swap(s[k], s[p-i]);
    }
}

void output() { // Outputs the way we've combined the vectors
    std::vector<std::pair<int, bool>> v({{s[1], false}});

    while(!v.empty()) { // Dfs
        auto p = v.back();
        v.pop_back();
        auto n = nodes[p.first]; auto b = p.second;
        if(!n.p[0]) // A leaf
            out[p.first] = b ? '-' : '+';
        else {
            v.push_back({n.p[0], b ^ n.s[0]});
            v.push_back({n.p[1], b ^ n.s[1]});
        }
    }
}

int main() {
    std::cin >> n >> L;
    m = p = n+1;
    for(int i = 1; i <= n; i++) { // Initialize every vector into its own tree
        scanf("%d %d", &vs[i].x, &vs[i].y);
        nodes[i].v = vs[i];
        s[i] = i;
    }
    
    while(p > 2) 
        merge();
    output();
    printf("YES\n%s", out+1);
}
