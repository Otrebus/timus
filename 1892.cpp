/* 1892. Morning in Koltsovo - http://acm.timus.ru/problem.aspx?num=1892
 *
 * Strategy:
 * Straightforward simulation according to the rules given.
 *
 * Performance:
 * O(n+m), runs the tests in 0.015s using 200KB memory.
 */

#include <stdio.h>
#include <algorithm>
#include <queue>
#include <map>

int main()
{
    int t1, t2, t3, t4, N, M, h, m, s; // Inputs
    std::queue<int> lq, tq; // Queue for getting into the landing and takeoff queues
    std::map<std::queue<int>*, std::vector<int>> out;

    scanf("%d %d %d %d %d", &t1, &t2, &t3, &t4, &N);
    while(N--)
        scanf("%d:%d:%d", &h, &m, &s), tq.push(s + 60*(m + 60*h));
    scanf("%d", &M);
    while(M--)
        scanf("%d:%d:%d", &h, &m, &s), lq.push(s + 60*(m + 60*h));

    std::map<std::queue<int>*, std::map<std::queue<int>*, int>> T = // Same as T[&tq][&tq] = t1, etc
                  { { &tq, { { &tq, t1 }, { &lq, t2 } } }, { &lq, { { &tq, t3 }, { &lq, t4 } } } };
    std::queue<int>* pq = tq.front() < lq.front() ? &tq : &lq; // pq is the closest relevant queue
    int t = std::min(tq.front(), lq.front()); // Current time

    while(true)
    {
        out[pq].push_back(t);
        pq->pop();
        if(lq.empty() && tq.empty())
            break;
        else if(!tq.empty() && (lq.empty() || t+T[pq][&tq] < lq.front() && tq.front() < lq.front()))
            t = std::max(t + T[pq][&tq], tq.front()), pq = &tq; // One plane may take off
        else
            t = std::max(t + T[pq][&lq], lq.front()), pq = &lq; // In all other cases, land a plane
    }

    for(auto t : out[&tq])
        printf("%02d:%02d:%02d\n", t%(24*60*60)/3600, t%(24*60*60)%3600/60, t%(24*60*60)%60);
    for(auto t : out[&lq])
        printf("%02d:%02d:%02d\n", t%(24*60*60)/3600, t%(24*60*60)%3600/60, t%(24*60*60)%60);
    std::system("pause");
}