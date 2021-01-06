/* 1245. Pictures - http://acm.timus.ru/problem.aspx?num=1245
 *
 * Strategy:
 * Treat each blood stain as the bounding box of its circle. Order all their vertical segments
 * from the left to the right and scan those segments in turn, treating each segment and its
 * successor as defining the rightmost and leftmost edges of the two paintings. Also keep two sets
 * of coordinates representing all the horizontal segments contained within each painting, which we
 * update as we progressively grow the left painting and shrink the right painting and keep track
 * of the minimum combined area found so far. Do the same in the vertical direction by simply
 * swapping all x and y coordinates. As far as the minimum size limit goes, it is always possible
 * to expand both dimensions of each painting outwards away from the center.
 * 
 * Performance:
 * O(N log N), runs the tests in 0.015s using 488KB.
 */

#include <iostream>
#include <set>
#include <vector>
#include <algorithm>
#include <set>
#include <limits>

struct rect { int x1, y1, x2, y2; }; // Each blood spot treated as a square
// The vertical segments, "start" indicating if it's the leftmost or rightmost of a square
struct event { int x, y1, y2; bool start; };

int solve(std::vector<rect>& v) {
    std::vector<event> e;   // All the vertical segments
    std::multiset<int> ys1; // Y coords of all the horizontal segments inside the leftmost painting
    std::multiset<int> ys2; // Same for rightmost

    int min = std::numeric_limits<int>::max();

    // Add and sort all left and right vertical segments of the squares (blood spots)
    for(auto r : v) {
        e.push_back( { r.x1, r.y1, r.y2, true } );
        e.push_back( { r.x2, r.y1, r.y2, false } );
    }
    std::sort(e.begin(), e.end(), [] (event& a, event& b) {
        return a.x == b.x && a.start < b.start || a.x < b.x; }
    );

    // Pre-add all horizontal segments coordinates to the rightmost painting
    for(auto& event : e) {
        if(event.start) {
            ys2.insert(event.y1);
            ys2.insert(event.y2);
        }
    }

    for(auto it = e.begin(); it != e.end(); it++) { // Go through the vertical segments one by one
        auto it2 = it + 1;

        if(it->start) // A leftmost side of a painting adds its horizontal segments
            ys1.insert( { it->y1, it->y2 } );
        
        // If there is nothing in between the paintings (so we cover all spots)
        if(ys1.size() + ys2.size() == e.size()) {
            int A = 0, B = 0;

            if(!ys1.empty()) { // Form the left painting
                // The leftmost edge is the first vertical segment, the rightmost edge is
                // the edge we're currently at, and the top and bottom edges are the
                // maximum and minimum of the y coordinates that we are keeping track of
                int xa = (it->x - e.front().x), ya = (*ys1.rbegin() - *ys1.begin());
                xa = std::max(100, xa); // Expand the painting (imagine outwards from the middle)
                ya = std::max(100, ya); // if necessary
                A = xa*ya;
            }
            if(!ys1.empty() && !ys2.empty()) { // Form the right painting
                int xb = (e.back().x - it2->x), yb = (*ys2.rbegin() - *ys2.begin());
                yb = std::max(100, yb);
                xb = std::max(100, xb);
                B = xb*yb;
            }

            if(A > 0 || B > 0) // If we could form either
                min = std::min(min, A+B);
        }

        // We're shrinking the right painting, so remove the appropriate horizontal segments
        if(it2 != e.end() && !it2->start) {
            ys2.erase(ys2.find(it2->y1));
            ys2.erase(ys2.find(it2->y2));
        }
    }
    return min;
}

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int N;
    std::cin >> N;
    std::vector<rect> v;
    while(N--) {
        int r, x, y;
        std::cin >> r >> x >> y;
        v.push_back( { x-r, y-r, x+r, y+r } );
    }

    int a = solve(v); // Horizontal direction first
    for(auto& r : v) {
        std::swap(r.x1, r.y1);
        std::swap(r.x2, r.y2);
    }
    int b = solve(v); // Now vertical
    std::cout << std::min(a, b);
}
