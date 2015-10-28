/* 1291. Gear-wheels - http://acm.timus.ru/problem.aspx?num=1291
 *
 * Strategy:
 * From the information given, we already know the number of cogs/second that all gears will be
 * spinning at, so all we need to do is run through the graph and figure out the spin directions
 * before printing out the final results.
 *
 * Performance:
 * O(N); runs the tests in 0.001s using 320KB memory.
 */

#include <stdio.h>
#include <vector>

class Gear
{
public:
    std::vector<Gear*> adj;
    int cogs;
    bool visited;
    bool parity; // Or spin direction
};

void dfs(Gear* gear)
{
    gear->visited = true;
    for(auto v : gear->adj)
        if(!v->visited)
            v->parity = !gear->parity, dfs(v);
}

int gcd(int a, int b)
{
    if(b == 0)
        return a;
    return gcd(b, a % b);
}

Gear gears[1001];

int main()
{
    int N;
    scanf("%d", &N);
    for(int i = 1; i <= N; i++)
    {
        scanf("%d", &gears[i].cogs);

        while(true)
        {
            int x;
            scanf("%d", &x);
            if(!x) 
                break;
            gears[i].adj.push_back(gears+x);
        }
    }
    int start, V;

    scanf("%d %d", &start, &V);
    V *= gears[start].cogs;
    if(V < 0)
        V = -V, gears[start].parity = true;
    dfs(gears+start);
    for(int i = 1; i <= N; i++)
    {
        if(!gears[i].visited)
            printf("0/1\n");
        else
        {
            int g = gcd(gears[i].cogs, V);
            printf("%s%d/%d\n", gears[i].parity ? "-" : "", V/g, gears[i].cogs/g);
        }
    }
    return 0;
}