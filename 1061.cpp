/* 1061. Buffer Manager - http://acm.timus.ru/problem.aspx?num=1061
 *
 * Strategy:
 * Naive (see comments).
 *
 * Performance:
 * O(n), runs in 0.015s using 480KB memory.
 */

#include <iostream>
#include <string>
#include <algorithm>

const int inf = std::numeric_limits<int>::max()/2;

int main()
{
    int N, K;
    std::cin >> N >> K;
    std::string A, input;
    while(std::cin >> input)
        A.append(input);

    unsigned long long sum = 0, minsum = inf;
    int minpos = -1, n = 0;
    // Any asterisk will make the sliding window sum greater than our "inf" value (which minsum is
    // initiated to), which will remain true until the K-sized window has moved past all asterisks
    auto val = [] (char a) -> unsigned long long { return a == '*' ? inf : a-'0'; };

    for(int i = 0; i < N; i++)
    {
        n = std::min(n+1, K);
        sum += val(A[i]); // We update the sum as we move forward by adding the new number ..
        if(n == K && sum < minsum)
            minsum = sum, minpos = 2+i-K;
        sum -= i+1 >= K ? val(A[1+i-K]) : 0; // .. and subtracting the one no longer in the window
    }
    std::cout << (minpos == -1 ? 0 : minpos);
}