/* 1595. Perfect Sequence - http://acm.timus.ru/problem.aspx?num=1595
 *
 * Strategy:
 * Naive; make each number x_k be the the first number not used so far that makes the subsequence
 * sum divisible by k.
 *
 * Performance:
 * O(nlog n), runs the tests in 0.359s using 23,840KB memory.
 */

#include <stdio.h>
#include <unordered_set>

int main() {
    long long N, s = 0, x;
    scanf("%lld", &N);

    std::unordered_set<int> S = { 0 };
    for(int i = 1; i <= N*2; i++, S.insert(x-s), s += x-s) {
        for(x = s/i*i + (s/i*i < s ? i : 0); S.find(x-s) != S.end(); x += i);
        printf("%llu ", x-s);
    }
}
