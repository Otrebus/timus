/* 1188. Library - http://acm.timus.ru/problem.aspx?num=1188
 *
 * Strategy:
 * Exhaustive search - for every position on every shelf, cut away the other shelves it intersects,
 * each in the cheapest way.
 *
 * Performance:
 * O(XN^2), runs the test cases in 0.078s using an unreported amount of memory.
 */

#include <stdio.h>
#include <tuple>

int m, n, w, h, N;

struct shelf {
    int l, r, a, b, y, w;
} A[100];

struct cost { // A tuple that you can add and order
    int a, b;
    cost operator+ (cost& c) { return { c.a+a, c.b+b }; }
    bool operator< (const cost &c) const { return (a < c.a) ? true : a == c.a && b < c.b; }
};

// Calculates the cost of putting the tome on position x on shelf i
std::pair<bool, cost> add(int i, int x) {
    if(A[i].y + h > n) // Tome extends above the niche
        return { false, {} };

    if(A[i].w < w) // Tome is wider than the shelf
        return { false, {} };

    if(x < A[i].l) { // Tome is left of the shelf, need to move the shelf
        int e = A[i].l - x; // The amount we need to slide the board to the left

        if(A[i].r - e < A[i].a) // Shelf would end up to the left of the leftmost peg
            return { false, {} };

        if(e > A[i].r - A[i].b) // Can move the shelf without removing a peg
            return { true, { 1, 0 } };

        // We need to move a peg, or if the board rotates left
        if(A[i].a - x > A[i].r - e - A[i].a)
            return { true, { 1, 0 } };
    }
    if(x + w > A[i].r) {
        int e = (x + w) - A[i].r;

        if(e > A[i].b - A[i].l)
            return { false, {} };

        if(e > A[i].a - A[i].l)
            return { true, { 1, 0 } };

        if(A[i].r + e - A[i].b > A[i].b - (A[i].l + e))
            return { true, { 1, 0 } };
    }
    return { true, { 0, 0 } };
}

cost remove(int j, int x, int y) {

    // Tome isn't touching any shelves
    if(y+h <= A[j].y || y >= A[j].y || x+w <= A[j].l || x >= A[j].r)
        return { 0, 0 };

    if(x+w <= A[j].a) { // Completely to the left of the left peg
        int e = x+w - A[j].l; // The part we slide over

        // The length beyond the right peg that needs to be cut or it would rotate
        auto r1 = std::max(0, (A[j].r + e - A[j].b) - (A[j].b - (x+w)));
        auto r2 = A[j].r + e - m; // The length beyond the frame
        return { 0, std::max(r1, r2) };
    }

    if(x >= A[j].b) { // Completely to the right of the right peg
        int e = A[j].r - x; // The part we slide over
        auto r1 = std::max(0, A[j].a - (A[j].l - e) - ((A[j].r - e) - A[j].a));
        auto r2 = 0 - (A[j].l - e);
        return { 0, std::max(r1, r2) };
    }

    if(x < A[j].a && x+w > A[j].b) { // Covers both pegs
        return { 2, A[j].w };
    }

    // Calculate the cost of sliding the shelf to the right and maybe cutting it
    int e = x + w - A[j].l;
    cost cl = { 1, 0 };
    if(e + A[j].r > m) { // If the extension is beyond the niche
        cl = { 1, e + A[j].r - m }; // Cut it
    }
    if(x + w == m) // If the extension touches the side of the niche we can't add a peg
        cl = { 2, A[j].w };

    e = A[j].r - x; // Analogous
    cost cr = { 1, 0 };
    if(A[j].l - e < 0) {
        cr = { 1, e - A[j].l };
    }
    if(x == 0)
        cr = { 2, A[j].w };

    // Covers no pegs, in the middle
    if(x >= A[j].a && x+w <= A[j].b) {
        return std::min(cl, cr); // We can pick a side and pretend we cover the other peg
    }

    // Covers left peg only
    if(x + w > A[j].a && x < A[j].a)
        return cl;

    // Covers right peg only
    if(x < A[j].b)
        return cr;
}

int main() {
    scanf("%d %d %d %d %d", &m, &n, &w, &h, &N);

    for(int i = 0; i < N; i++) {
        int yi, xi, li, x1i, x2i;
        scanf("%d %d %d %d %d", &yi, &xi, &li, &x1i, &x2i);
        A[i].y = yi;
        A[i].l = xi;
        A[i].r = xi + li;
        A[i].a = A[i].l + x1i;
        A[i].b = A[i].l + x2i;
        A[i].w = li;
    }

    cost minc = { 1e9, 1e9 };
    for(int i = 0; i < N; i++) { // For each shelf
        for(int x = 0; x + w <= m; x++) { // For each position
            auto [b, c] = add(i, x); // If we can put the tome on the shelf
            if(b) {
                for(int j = 0; j < N; j++) { // For each other shelf
                    if(j != i) {
                        // Calculate the cost of removing that shelf
                        auto c2 = remove(j, x, A[i].y);
                        c = c + c2;
                    }
                }
                minc = std::min(minc, c);
            }
        }
    }
    printf("%d %d", minc.a, minc.b);
}