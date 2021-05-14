/* 1730. ManBearPig - http://acm.timus.ru/problem.aspx?num=1730
 *
 * Strategy:
 * Make sure all fractions are reducible to power of 2's in the denominator, recast them all in
 * 1024ths, and percolate the creatures through a genealogy tree where for each creature we shave
 * off the contribution of the current node from the numerator of the current creature, iterating
 * through the items and the tree in the wild flurry of passion and chance that brought us all onto
 * this planet. (As usual, see comments below if the above was too nebulous)
 *
 * Performance:
 * O(max b), runs the tests in 0.015s using 160KB memory.
 */

#include <string>
#include <vector>
#include <numeric>

struct {
    std::string name;
    int w; // Number of 1024ths that the final abomination consists of this particular creature
} C[200];
char s[1000], h;

std::vector<std::string> v;

// Current row, number of creatures, fraction, current creature, what's left of it, dummy variable
int i, n, a, b, c, w, x; // I put them all here because I obsessively code golf

int solve(int p) {
    // For the node of this power, see if we can fit the unaccounted parts of the current creature
    // within that power in which case this node is a direct ancestor, otherwise, recurse
    if(C[c].w - w >= p)
        // If the node's contribution is less or equal to what's left of this creature, push the
        // creature as an ancestor. If ==, iterate to the next creature and update the "power used"
        v.push_back(C[c].name), w = w + p >= C[c].w ? p - (C[c++].w - w) : w + p;
    else
        // Otherwise, the remainder of the creature is a product of something further back; go deepr
        v.push_back(std::to_string(solve(p/2)) + " " + std::to_string(solve(p/2)));
    return ++i;
}

int main() {
    scanf("%s", s);
    for(int i = 0; s[i]; i++, n++)
        while(s[i] && s[i] != '-')
            C[n].name.push_back(s[i++]);

    for(int i = 0; i < n; i++) {
        scanf("%d%c%d", &a, &h, &b);

        for(x = b; ~x&1; x >>= 1); // Beat the 2ths out of the denominator
        if(a % x) // If anything remains, it wasn't a power of 2
            return printf("No solution"), 0;
        C[i].w = a*1024/b; // The number of the beast in powers of 1024ths
    }

    // If we don't sum up to unity we're done for
    if(std::accumulate(C, C+n, 0, [] (auto a, auto& b) { return a + b.w; }) != 1024)
        return ("No solution"), 0;

    // Iterate through the tree, building the ancestor array
    solve(1024);
    printf("%d\n", v.size());
    for(auto s : v)
        printf("%s\n", s.c_str());
}
