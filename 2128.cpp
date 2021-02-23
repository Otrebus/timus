/* 2128. Rubina's Solitaire - http://acm.timus.ru/problem.aspx?num=2128
 *
 * Strategy:
 * Maintain a container of the next needed cards to put onto any stack. Loop the following:
 *   Fetch a card from that container. If the card is the front of any row, add it to the output
 * and pop that row, otherwise add it to a pool to add to the container next time we see it
 * in front of any row.
 * Loop until the container is empty, check that we're outputting n*m cards.
 *
 * Performance:
 * O(n*m), runs the tests in 0.14s using 11,412KB memory.
 */

#include <stdio.h>
#include <unordered_set>
#include <unordered_map>
#include <vector>

struct card {
    int s, r;
    std::size_t operator()(const card& k) const
    {
      return std::hash<long long>()((long long)k.s << 32) ^ std::hash<long long>()((long long)k.r);
    }
    bool operator==(const card& k) const { return s == k.s && r == k.r; };
};
int m, n, k;

int main() {
    scanf("%d%d%d", &m, &n, &k);

    std::vector<std::vector<card>> row(k); // The rows
    std::vector<card> s, out; // The container of needed cards and the output
    std::unordered_set<card, card> pool; // The set of cards buried in the rows we're waiting for
    std::unordered_map<card, int, card> front; // The front of the rows and their cards

    int a, b, c;
    for(int i = 0; i < k; i++) {
        scanf("%d", &a);
        for(int j = 0; j < a; j++) {
            scanf("%d %d", &b, &c);
            row[i].push_back(card{b, c});
        }
        front[row[i].back()] = i;
    }

    for(int i = 1; i <= m; i++)
        s.push_back({i, 1});
   
    while(!s.empty()) {
        // Get some card from the container we want to put on a stack
        card c = s.back();
        s.pop_back();

        if(front.find(c) == front.end()) {
            // If we don't have that stack in the front of any row, wait for it
            pool.insert(c);
            continue;
        }
        out.push_back(c);

        // Pop the row, add the new front card to the front set
        int ro = front[c];
        row[ro].pop_back();
        if(!row[ro].empty()) {
            auto bc = row[ro].back();
            front[bc] = ro;
            if(pool.find(bc) != pool.end()) {
                // If we uncover a card we were waiting for, put it back in the container
                pool.erase(bc);
                s.push_back(bc);
            }
        }

        front.erase(c);

        if(c.r < n) {
            // Add the next needed cards of the suite to the container
            card nc {c.s, c.r+1};
            s.push_back(nc);
        }
    }

    if(out.size() != n*m) {
        printf("NO\n");
        return 0;
    }

    printf("YES\n");
    for(auto o : out)
        printf("%d %d\n", o.s, o.r);
}
