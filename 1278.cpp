/* 1278. "... Connecting People" - http://acm.timus.ru/problem.aspx?num=1278
 *
 * Strategy:
 * We make use of the fact that if we have a calling sequence that looks like
 *
 *  CALL 1
 *  CALL 2
 *    ...
 *  CALL n
 *  BELL&RET
 *
 * the total number of calls to BELL&RET is 2^n, which we will inevitably get even if there are
 * calls before this sequence (remember to transpose the addresses if we add stuff before it).
 * We can then conceptually subtract 2^n from the input number, and add calls - before the
 * sequence - to the additional powers of two that we need from the equivalent subsequence.
 * For example, for the number 5, we can output the sequence
 * 
 *  CALL 3
 *  CALL 2
 *  CALL 3
 *  BELL&RET
 * 
 * since we eventually get the biggest power (2^2) with the end train (CALL 2, CALL 3), and then
 * we are able to supplement the missing power (2^0) with the call to the item at the end of the
 * subsequence.
 * 
 * Performance:
 * O(log K), runs the tests in 0.015s using 228KB memory.
 */

#include <iostream>
#include <vector>
#include <string>

int main() {
    int K;

    std::cin >> K;

    std::vector<int> v;
    std::vector<std::string> out;
    
    for(int b = 0; b < 32; b++) {
        if((1 << b) & K)
            v.push_back(b);
    }

    int n = v.back() + v.size();

    out.push_back("BELL&RET");
    for(int i = 1; i <= v.back(); i++) // The power repository
        out.push_back("CALL " + std::to_string(n-i));

    for(int i = 0; i < v.size()-1; i++) // The supplemental fill-ins
        out.push_back("CALL " + std::to_string(n-v[i]-1));

    for(auto it = out.rbegin(); it < out.rend(); it++) { // Print them out in the right order
        std::cout << *it << std::endl;
    }
}
