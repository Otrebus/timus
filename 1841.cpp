/* 1841. Cableways - http://acm.timus.ru/problem.aspx?num=1841
 *
 * Strategy:
 * Keep the cableways in a map indexed by their destination index, and update the optimal cost of
 * any such point by simple dynamic programming while iterating over the incoming and outgoing
 * cableways while climbing the path from vertex to vertex.
 *
 * Performance:
 * O(nlog n), runs the tests in 0.156s using 8,844KB memory.
 */

#include <map>

using ll = long long;

struct line {
    ll t; // The optimal time to reach the destination if we use this cableway
    int i; // The path this cableway is part of
};

std::map<int, line> M;
ll A[100001], v[100001], C[100001]; // Vertices, speeds, optimal outgoing cableway costs
ll a, xp; // Optimal cost (answer), previous x position
int L[100001], N, m, i; // Outgoing cableway positions

auto first_leq = [] (int n) {
    // Returns a reverse iterator to the first smaller key of n
    auto ub = M.upper_bound(n);
    if(ub != M.end())
        return std::make_reverse_iterator(ub);
    return M.empty() ? M.rend() : M.rbegin();
};

// Different ways of erasing or checking an iterator depending on if it's a reverse or
// forward iterator
auto is_last(std::map<int, line>::iterator& it) { return it == M.end(); }
auto is_last(std::map<int, line>::reverse_iterator& it) { return it == M.rend(); }
auto erase (std::map<int, line>::iterator& it) { return M.erase(it); };
auto erase (std::map<int, line>::reverse_iterator& it) {
    return std::make_reverse_iterator(M.erase(std::prev(it.base())));
};

template<typename iterator> void update(iterator it) {
    // Updates the least costs while moving along path i and using incoming cableways in the map
    // starting at iterator it
    int j = 0, s = (A[i+1] > A[i]) ? 1 : -1;
    while(true) {
        // If the first incoming cableway comes before the first outgoing
        if(!is_last(it) && (j >= m || L[j]*s >= it->first*s) && it->first*s <= A[i+1]*s) {
            // Make sure we don't consider an outgoing cableway an incoming
            if(it->first != A[i] || (it->second.i != i && it->second.i != i-1)) {
                ll dx = std::abs(xp - it->first);
                xp = it->first;
                a = std::min(a + dx*v[i], it->second.t); // DP, standard relaxation
                it = erase(it);
            } else
                it++;
        } else if(j < m) {
            // Otherwise, update the cost of the outgoing
            ll dx = std::abs(xp - L[j]);
            C[j] += (a += dx*v[i]);
            xp = L[j++];
        } else
            break;
    }
    a += v[i]*std::abs(xp-A[i+1]);
}

int main() {
    scanf("%d", &N);
    for(i = 0; i < N+1; i++)
        scanf("%lld", &A[i]);

    for(i = 0; i < N; i++) {
        scanf("%lld %d", &v[i], &m);
        for(int j = 0; j < m; j++)
            scanf("%d %lld", &L[j], &C[j]);

        xp = A[i];
        if(A[i+1] > A[i]) // Heading right
            update(M.lower_bound(A[i])); // Find the first cableway to iterate from and do DP
        else
            update(first_leq(A[i]));
        for(int j = 0; j < m; j++)
            M.insert({ L[j], { C[j], i } });
    }
    printf("%lld", a);
}
