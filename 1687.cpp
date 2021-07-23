/* 1687. Numismatics for Fun - http://acm.timus.ru/problem.aspx?num=1687
 *
 * Strategy:
 * First, go through each of the m needed coins and add some box that contains the coin to the set.
 * Then, add any remaining boxes that contain any needed coins until we have at most m boxes.
 *
 * Performance:
 * O(m*k*max(k_i)), runs the tests in 0.015s using 552KB memory.
 */

#include <iostream>
#include <unordered_set>

std::unordered_set<int> A, B, s, v[100]; // Needed coins, coins added so far, output, boxes

int main() {
    int n, k, ki, x, m;

    std::cin >> n >> k;
    for(int i = 0; i < k; i++) {
        std::cin >> ki;
        while(ki--) {
            std::cin >> x;
            v[i].insert(x);
        }
    }
    std::cin >> m;
    for(int i = 0; i < m; i++) {
        std::cin >> x;
        A.insert(x);
    }

    for(auto x : A) { // For each needed coin
        for(int j = 0; j < k; j++) { // Check that some box contains it
            if(s.find(j) == s.end() && v[j].find(x) != v[j].end()) {
                s.insert(j); // If so, add box to the output
                for(auto y : v[j]) // Add all the needed coins in the box to the set of added coins
                    if(A.find(y) != A.end())
                        B.insert(y);
                break;
            }
        }
    }

    // Until the number of boxes in the set is less than the number of added coins, add a box
    for(int i = 0; i < k && s.size() < B.size(); i++)
        if(s.find(i) == s.end())
            for(auto x : v[i])
                if(A.find(x) != A.end())
                    s.insert(i);

    std::cout << s.size() << std::endl;
    for(auto x : s)
        std::cout << (x+1) << " ";
}
