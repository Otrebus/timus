/* 1176. Hyperchannels - http://acm.timus.ru/problem.aspx?num=1176
 *
 * Strategy:
 * This is essentially the same problem as 1136; we are to find the Euler path of the inverse of
 * the graph this time.
 *
 * Performance:
 * O(N^2), which is optimal since the input is of this size. The solution runs in 0.187s and uses
 * 896KB memory.
 */

#include <stdio.h>
#include <vector>
#include <queue>
#include <cctype>

struct vertex
{
    std::vector<vertex*> out;
    int n;
};

void explore(vertex* u, std::vector<vertex*>& ans)
{
    ans.push_back(u);
    std::queue<vertex*> q;
    vertex* v = u;
    while(!v->out.empty())
    {
        vertex* w = v->out.back();
        v->out.pop_back();
        v = w;
        q.push(v);
    }
    while(!q.empty())
    {
        explore(q.front(), ans);
        q.pop();
    }
}

int main()
{
    int N, A;
    std::vector<vertex> v(1001);

    scanf("%d %d", &N, &A);
    for(int i = 1; i <= N; i++)
    {
        v[i].n = i;
        for(int j = 1; j <= N; j++)
        {
            int n;
            do
            {
                n = getc(stdin);
            } while(isspace(n));

            if(n == '0' && i != j)
                v[i].out.push_back(&v[j]);
        }
    }
    int m, last, tot = 0;

    std::vector<vertex*> ans;
    explore(&v[A], ans);
    for(int i = 0; i < ans.size()-1; i++)
        printf("%d %d\n", ans[i]->n, ans[i+1]->n);
    return 0;
}