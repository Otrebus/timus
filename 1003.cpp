/* 1003. Parity - http://acm.timus.ru/problem.aspx?num=1003
 *
 * Strategy:
 * For each test, maintain a mapping between the digits of the sequence and the shortest question
 * interval that has its rightmost endpoint at the digit. Whenever an interval is added to the
 * mapping whose endpoint conflicts with an earlier added interval, split the interval into two in
 * the mapping and maintain the above invariant by recursing. Whenever there is a parity conflict
 * between two same-sized and positioned intervals, stop.
 *
 * Performance:
 * Linear in the amount of intervals (questions) given since the left endpoint and right endpoints
 * each cause at most one split each. The solution runs the test cases in 0.031s using 588KB memory.
 */

#include <stdio.h>
#include <vector>
#include <unordered_map>
#include <cctype>

struct entry
{
    int left, right;
    bool parity;
};

int solve(int length, const std::vector<entry>& in)
{
    std::unordered_map<int, entry> m;
    m.reserve(in.size());
    int ans = 0;
    for(auto it = in.cbegin(); it < in.cend(); it++)
    {
        auto ins = *it; // The interval to insert
        if(ins.left < 1 || ins.right > length)
            return ans;
        while(true)
        {
            auto ite = m.find(ins.right);
            if(ite == m.end())
            {
                m[ins.right] = ins;
                break;
            }
            auto e = ite->second;
            if(e.left < ins.left) // Split and recurse
            {
                m[ins.right].left = ins.left;
                m[ins.right].parity = ins.parity;
                ins = entry { e.left, ins.left-1, e.parity ^ ins.parity };
            }
            else if(e.left > ins.left)
                ins = entry { ins.left, e.left-1, e.parity ^ ins.parity };
            else if(e.parity != ins.parity)
                return ans;
            else break;
        }
        ans++;
    }
    return ans;
}

inline int getint()
{
    int ret = 0, ch;
    while(!isdigit(ch = getc(stdin)));
    ret = ch - '0';
    while(isdigit(ch = getc(stdin)))
        ret = ret*10 + ch - '0';
    return ret;
}

int main()
{
    while(true)
    {
        int N, m;
        scanf("%d", &N);
        if(N == -1)
            return 0;
        m = getint();
        std::vector<entry> input;
        input.reserve(m);
        for(int i = 0; i < m; i++)
        {
            char parity[10];
            int l = getint(), r = getint();
            scanf("%s", parity);
            input.push_back(entry { l, r, parity[0] == 'o' });
        }
        printf("%d\n", solve(N, input));
    }
    return 0;
}