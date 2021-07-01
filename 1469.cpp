/* 1469. No Smoking! - http://acm.timus.ru/problem.aspx?num=1469
 *
 * Strategy:
 * Bentley-Ottmann.
 *
 * Performance:
 * O(N log N), runs the tests in 0.109s using 13,356KB memory.
 */

#include <stdio.h>
#include <set>

using ll = long long;

struct point { // More like event
    ll x, y;
    bool start;  // "Left" or "right" endpoint
    int segment; // Index of the segment it belongs to
    point operator- (const point& b) const { return { x - b.x, y - b.y, start }; }
    ll operator% (const point& b) const { return x*b.y - b.x*y; } // Cross product
    bool operator< (const point& b) const { // The ordering of the priority queue
        return std::make_tuple(x,!start,y,segment) < std::make_tuple(b.x,!b.start,b.y,b.segment);
    }
};

struct segment {
    point s, e; // Left and right points (start and end)
    int i; // The index of this segment in the input
    bool operator<(const segment& b) const { // Returns true if this is below segment b
        if(b.s.x == s.x)
            return b.s.y > s.y;
        if(b.s.x < s.x)
            return (b.s-b.e)%(s-b.e) > 0;
        return (s-e)%(b.s-e) < 0;
    }

} segments[50001];

std::set<segment> segset;
std::set<point> events;

bool in(ll a, ll b, ll c) { // Returns true if c is in interval [a,b]
    return std::min(a, b) <= c && c <= std::max(a, b);
}

bool intersects(const segment& a, const segment& b) { // Intersects segment a and b
    auto x = (b.s - a.s) % (a.e - a.s);
    auto y = (b.e - a.s) % (a.e - a.s);
    auto z = (a.s - b.s) % (b.e - b.s);
    auto w = (a.e - b.s) % (b.e - b.s);
    if(x*y == 0 && z*w == 0)
        // One segment has at least one point on the others' extended line so we check if any point
        // is actually within the proper interval of the segment
        return in(a.s.x, a.e.x, b.s.x) && in(a.s.y, a.e.y, b.s.y) ||
               in(a.s.x, a.e.x, b.e.x) && in(a.s.y, a.e.y, b.e.y);
    return x*y <= 0 && z*w <= 0; // The segments straddle each other
}

std::set<segment>::iterator prev(const std::set<segment>::iterator& it) {
    return it == segset.begin() ? segset.end() : std::prev(it);
}

std::set<segment>::iterator next(const std::set<segment>::iterator& it) {
    return it == segset.end() ? segset.end() : std::next(it);
}

std::pair<int, int> solve() {
    for(auto& e : events) {
        if(e.start) {
            // Event is a left endpoint, insert into segment set and test neighbors for intersection
            auto s = e.segment;
            auto it = segset.lower_bound(segments[s]);
            auto it2 = prev(it);
            for(auto i : { it, it2 })
                if(i != segset.end())
                    if(intersects(*i, segments[s]))
                        return { i->i, s };
            segset.insert(segments[s]);
        } else {
            // The event is a right endpoint, delete the segment and check neighbors for intrsection
            auto s = e.segment;
            auto it = segset.find(segments[s]);
            auto it2 = prev(it), it3 = next(it);
            if(it2 != segset.end() && it3 != segset.end())
                if(intersects(*it2, *it3))
                    return { it2->i, it3->i };
            segset.erase(segments[s]);
        }
    }
    return { -1, -1 };
}

segment make_segment(ll x1, ll y1, ll x2, ll y2, int i) {
    // Creates a segment and makes sure its left and right endpoints are ordered appropriately
    point s = { x1, y1, false, i }, e = { x2, y2, false, i };
    if(e < s)
        std::swap(s, e);
    s.start = true, e.start = false;
    return segment { s, e, i };
}

int main() {
    int N;
    scanf("%d", &N);
    for(int i = 1; i <= N; i++) {
        ll x1, y1, x2, y2;
        scanf("%lld %lld %lld %lld", &x1, &y1, &x2, &y2);

        segments[i] = make_segment(x1, y1, x2, y2, i);
        events.insert(segments[i].s);
        events.insert(segments[i].e);
    }

    auto p = solve();
    if(p.first >= 0)
        printf("YES\n%d %d", p.first, p.second);
    else
        printf("NO");
}
