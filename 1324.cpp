/* 1324. Extra Spaces - http://acm.timus.ru/problem.aspx?num=1324
 *
 * Strategy:
 * Find a pattern by brute-force (if we iterate from the bottom up, it turns out we get the most
 * additional spaces from L >= i by choosing k = i/2 + 1 for some reason), execute pattern.
 *
 * Performance:
 * O(L), runs the tests in 0.015s using 408KB memory.
 */

#include <iostream>
#include <vector>

int main() {
    int L;
    std::cin >> L;
    std::vector<int> ans;
    if(L > 1)
        ans.push_back(2);

    for(int i = 2; i < L;) {
        int k = i/2 + 1;
        ans.push_back(k);
        for(int s = i, j = s + 1; j/k + j%k <= s && j <= L; i = j++);
    }

    std::cout << ans.size() << std::endl;
    for(auto a = ans.rbegin(); a != ans.rend(); a++)
        std::cout << *a << std::endl;
}

/*
// Program to find k = i/2 + 1
int main() {
    int i = 2;
    while(true) {
        int maxj = 0;
        int maxk = 0;
        for(int k = 2; k <= i; k++) {
            for(int j = i+1; ; j++) {
                if(j/k + j%k <= i) {
                    if(j > maxj) {
                        maxj = j;
                        maxk = k;
                    }
                }
                else
                    break;
            }
        }
        i = maxj;
        std::cout << "new k: " << maxk << std::endl;
        std::cout << "new i: " << i << std::endl;
    }
}
*/
