/* 1403. Courier. - http://acm.timus.ru/problem.aspx?num=1403
 *
 * Strategy:
 * Sort the orders by deadline, and, starting from the first deadline, try to meet each order. If
 * the deadline of some order can't be met, replace the least valued earlier met order by the new
 * order.
 *
 * Performance:
 * O(n log n), running the tests in 0.001s (submission rated top 5 out of 1255) using 5612KB 
 * memory.
 */

#include <stdio.h>
#include <queue>
#include <algorithm>

struct order
{
    bool operator() (const order& a, const order& b) { return a.reward > b.reward; }
    int deadline, reward, number;
    order() {}
    order(int deadline, int reward, int number) : deadline(deadline), reward(reward), number(number)
    { }
};

int main()
{
    std::priority_queue<order, std::vector<order>, order> q;
    std::vector<order> v;
    int n;
    scanf("%d", &n);
    for(int i = 1; i <= n; i++)
    {
        int x, y;
        scanf("%d %d", &x, &y);
        v.push_back(order(x, y, i));
    }
    std::sort(v.begin(), v.end(), 
              [] (const order& a, const order& b) { return a.deadline < b.deadline; });
    for(auto it = v.begin(); it < v.end(); it++)
    {
        if(it->deadline == q.size() && q.top().reward < it->reward)
        {    // The size of q is equal to the day number, if it surpasses the deadline of the
             // current order, we can't meet it, so we (possibly) replace some earlier order
             q.pop();
             q.push(*it);
        }
        else if(it->deadline > q.size())
            q.push(*it);
    }
    printf("%d\n", q.size());
    std::vector<order> ans;

    while(!q.empty())
    {
        ans.push_back(q.top());
        q.pop();
    }
    std::sort(ans.begin(), ans.end(), 
              [] (const order& o1, const order& o2) { return o1.deadline < o2.deadline; } );
    for(auto it = ans.begin(); it < ans.end(); it++)
        printf("%d ", it->number);
    return 0;
}