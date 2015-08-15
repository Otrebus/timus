/* 1261. Tips - http://acm.timus.ru/problem.aspx?num=1261
 *
 * Strategy:
 * Brute-force - try all sums of powers of 3 up to N when N < (3^K-1)/2.
 *
 * Performance:
 * O(N log N), runs the test cases in 0.001s using 240KB memory.
 */

#include <stdio.h>
#include <vector>

std::vector<unsigned int> pow3;

int main()
{
    pow3.push_back(1);
    while(pow3.back() <= ((unsigned int) ~0)/3)
        pow3.push_back(pow3.back()*3);
    int N;
    scanf("%d", &N);
    int i = 0;
    while(N >= (pow3[i+1]-1)/2) // Any power of 3 greater than this will just be used as change
        i++;
    for(int k = 0; k < (1 << (i+1)); k++)
    {   // The bits of K represents the powers of 3 used in our brute force search
        int sum = 0;
        for(int j = 0; j < i+1; j++)
            if(k & (1 << j))
                sum += pow3[j];
        if(sum <= N)
            continue;
        int tip = sum - N;
        for(int l = i; l >= 0; l--) // Check that we can actually represent the tip in powers of 3
            if(tip >= pow3[l])
                tip -= pow3[l];
        if(tip == 0)
        {
            printf("%d %d", sum, sum - N);
            return 0;
        }
    }
    printf("0");
    return 0;
}