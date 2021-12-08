/* 1865. The Island of Märket - http://acm.timus.ru/problem.aspx?num=1865
 *
 * Strategy:
 * There are two different cases we deal with.
 * 
 *   1) If the part between the east side of the lighthouse and the east coast is small enough
 *      we make the smallest possible strip of land along the east coast until we reach the level
 *      of the lighthouse, and then wrap the border around the lighthouse along its north, west and
 *      and south sides, continue to the east coast, and then make another thin strip south. Think
 *      of this as shrink-wrapping the tower towards the east. Then we can expand the border
 *      arbitrarily westwards to incorporate the amount of land we need.
 * 
 *   2) If the above mentioned part is too large, we wrap the border around the lighthouse partially
 *      along its eastern side as well, and shape it inside the area between the east wall and the
 *      coast appropriately, before returning to the coast and running along it.
 *
 * Performance:
 * O(h), runs the tests in 0.046s using 168KB memory.
 */

#include <stdio.h>
#include <vector>

using ll = long long;

ll w, h, xt, yt, d;

struct pos { 
    ll x, y;
    pos operator-(const pos& p) { return { x-p.x, y-p.y }; }
    ll operator%(const pos& p) { return x*p.y - y*p.x; }
};

void output(std::vector<pos>& v) { // Prints the border points
    std::vector<pos> v2 = { v[0] };

    // Output only points that do not lie in a straight line
    for(int i = 1; i < v.size()-1; i++) {
        pos& p1 = v2.back(), p2 = v[i], p3 = v[i+1];
        if((p1-p2)%(p3-p2) != 0)
            v2.push_back(v[i]);
    }
    v2.push_back(v.back());

    printf("%d\n", v2.size());
    for(auto c : v2)
        printf("%d %d\n", c.x, c.y);
}

void solve1() { // Draws the border around the lighthouse, and expands it
    std::vector<pos> v; // The border

    ll a = (w*h)/2 - (h + d*(w-1-xt)); // The amount of extra land we need

    // This is done O(h). This can be done in constant time, of course
    for(int i = h-1; i >= 0; i--) {
        ll ai = (i >= yt && i < yt+d) ? w - xt : 1; // The land we already have on this row
        ll r = std::min((w-1)-ai, a); // If we need more area, add as much as we can with this row
        ai += r;
        a -= r;
        if(v.size() && v.back().x == w-ai) // Only add a point to the output if different x-wise
            v.pop_back();
        else
            v.push_back( { w-ai, i+1 } );
        v.push_back( { w-ai, i } );
    }
    output(v);
}

void solve2() { // Draws the border around the lighthouse and inside the area to the right
    std::vector<pos> v; // The border
    int x, y;

    // Draws the border along the coast to the south side of the lighthouse
    v.push_back({ w-1, h });
    v.push_back({ w-1, yt+d });
    v.push_back({ xt, yt+d });
    v.push_back({ xt, yt });
    v.push_back({ xt+d, yt });

    // Determine how many rows of the part between the east side of the lighthouse
    // and the coast we need to fill up (the last row possibly partially)
    ll r = (w*h)/2 - (d*d + h + (w-1-(xt+d)));
    int dy = r/(w-1-(xt+d));
    int dx = r%(w-1-(xt+d));

    // Wraps around the lighthouse and fills in the area described above
    v.push_back({ xt+d, yt+d-1-dy });
    v.push_back({ w-1-dx, yt+d-1-dy });
    v.push_back({ w-1-dx, yt+d-1-dy-1 });
    v.push_back({ w-1, yt+d-1-dy-1 });
    v.push_back({ w-1, 0 });

    output(v);
}

int main() {
    scanf("%lld %lld %lld %lld %lld", &w, &h, &xt, &yt, &d);

    ll a = d*d + h + (w-xt-d-1);
    if(a > (w*h)/2)
        return printf("Impossible"), 0;
    if(h + d*(w-1-xt) <= (w*h)/2) // Area(coast+lighthouse+intermediate area) <= Area(island)
        solve1();
    else
        solve2();
}
