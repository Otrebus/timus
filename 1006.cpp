/* 1006. Square Frames - http://acm.timus.ru/problem.aspx?num=1006
 *
 * Strategy:
 * Brute-force - for each coordinate and width, try to match a frame to that part of the picture.
 * If a frame matches, we add it to the answer, remove it from the picture and allow any part of
 * a future frame to match the empty space, but only add frames that match at least one non-empty
 * space.
 *
 * Performance:
 * O(NH^2W), runs the tests in 0.001s using 448KB memory.
 */

#include <iostream>
#include <string>
#include <vector>

const int UL = 218, UR = 191, LL = 192, LR = 217, H = 196, V = 179, E = 46;

struct ans {
    int x, y, w;
};

std::vector<ans> out;

struct cell {
    int s;
    int type;
    bool added;
} cells [50][20];


bool add(int x, int y, int type, std::vector<int>& added) {
    // Helper to match(), tries to match a part of a frame with a coordinate
    auto& c = cells[x][y];
    if(c.added) // "Empty space" after having matched a previous frame
        return true;
    if(c.type == E) // Actual empty space
        return false;
    if(c.type == type && !c.added) {
        added.push_back(x + y*50); // New matching coordinate
        return true;
    }
    return false;
}

void match(int& remaining, int x, int y, int w) {
    // Attempts to match a frame with the drawing
    int x1 = x, y1 = y;
    int x2 = x + w - 1, y2 = y + w - 1;

    std::vector<int> added; // The positions that match the frame

    if(!add(x, y, UL, added)) // Upper left corner
        return;
    while(++x < x2)
        if(!add(x, y, H, added)) // Upper side
            return;
    if(!add(x, y, UR, added)) // Upper right corner
        return;
    while(++y < y2)
        if(!add(x, y, V, added)) // Right side
            return;
    if(!add(x, y, LR, added)) // Bottom right corner 
        return;
    while(--x > x1)
        if(!add(x, y, H, added)) // Bottom side
            return;
    if(!add(x, y, LL, added)) // Lower left corner
        return;
    while(--y > y1)
        if(!add(x, y, V, added)) // Left side
            return;

    if(!added.empty()) {
        remaining -= added.size();
        out.push_back( { x1, y1, w } );
        for(auto p : added)
            cells[p%50][p/50].added = true;
    }
}

int main() {

    int r = 0; // Cells remaining
    for(int y = 0; y < 20; y++) {
        std::wstring s;
        std::wcin >> s;
        for(int x = 0; x < s.size(); x++) {
            r += (s[x] != 46);
            cells[x][y].type = (int) s[x];
        }
    }

    while(r)
        for(int y = 0; y < 20; y++)
            for(int x = 0; x < 50; x++)
                for(int w = 2; w + x <= 50 && w + y <= 20; w++)
                    match(r, x, y, w);

    std::cout << out.size() << std::endl;
    for(auto o = out.rbegin(); o != out.rend(); o++)
        std::cout << o->x << " " << o->y << " " << o->w << std::endl;
}
