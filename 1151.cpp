/* 1151. Radiobeacons - http://acm.timus.ru/problem.aspx?num=1151
 *
 * Strategy:
 * For each beacon, maintain a set of its possible locations, and for each checkpoint which detects
 * that beacon, intersect said set with the points at the given range from the checkpoint.
 *
 * Performance:
 * O(NMR), runs the tests in 0.031s using 444KB memory.
 */

#include <iostream>
#include <map>
#include <unordered_set>
#include <string>
#include <sstream>

struct node {
    int x, y;
    bool operator()(const node& a) const { return (x << 16) | y; }
    bool operator==(const node& a) const { return a.x == x && a.y == y; }
};

std::map<int, std::unordered_set<node, node>> B;

int main() {
    int M;
    std::cin >> M;
    std::cin.ignore();
    for(int i = 0; i < M; i++) {
        int X, Y;
        std::string str;
        char s;

        std::getline(std::cin, str);
        std::stringstream ss(str);

        ss >> X >> s >> Y;
        while(ss >> s) {
            int r, d;
            ss >> r >> s >> d;

            std::unordered_set<node,node> S;

            // Add the positions at the top and bottom of the given range into S
            for(int x = X - d; x <= X + d; x++)
                for(int y : { Y - d, Y + d })
                    if(x >= 1 && y >= 1 && y <= 200 && x <= 200)
                        S.insert({x, y});

            // Add the positions at the left and right
            for(int y = Y - d + 1; y <= Y + d - 1 ; y++)
                for(int x : { X - d, X + d })
                    if(x >= 1 && y >= 1 && y <= 200 && x <= 200)
                        S.insert({x, y});

            // Intersect S with the existing set of coordinates for the beacon
            std::unordered_set<node, node> C;
            if(B[r].empty()) // If there is no set, make it equal to S
                B[r] = S;
            else { // Otherwise intersect
                for(auto s : S)
                    if(B[r].find(s) != B[r].end())
                        C.insert(s);
                B[r] = C;
            }
        }
    }
    for(auto p : B) {
        auto m = p.second;
        std::cout << p.first << ":";
        // For each beacon, we only know its position if there is one possible location for it
        if(m.size() != 1) {
            std::cout << "UNKNOWN" << std::endl;
        } else {
            auto c = m.begin();
            std::cout << c->x << "," << c->y << std::endl;
        }
    }
}
