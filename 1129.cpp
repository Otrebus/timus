/* 1129. Door Painting - http://acm.timus.ru/problem.aspx?num=1129
 *
 * Strategy:
 * Create a graph where every room is a node and every door is an edge. If the degree of every node
 * is even, we can take an Euler tour through the house and color every door that we pass green on
 * one side and yellow on the other, and we satisfy the door balance requirement given since we will
 * always color a door yellow when we enter a room and green when we leave it. If the degrees of
 * some nodes are odd, we can join every pair of odd nodes with an additional edge corresponding to
 * a virtual door that we can use to pass between rooms (there is always an even number of odd rooms
 * by the handshaking lemma). These virtual doors will only upset the hitherto perfect yellow/green 
 * balance by at most one door since there is at most one virtual door per room.
 *
 * Performance:
 * O(N^2), which is optimal since the input is of this size. The solution runs in 0.015s using 500KB
 * memory.
 */

#include <iostream>
#include <vector>
#include <queue>

const int maxn = 100;

bool color[maxn+1][maxn+1]; // Color assigned to this side of the door
char mult[maxn+1][maxn+1];  // Multiplicity of this edge (could be a normal edge plus a virtual one)

struct node
{
    bool visited;
    std::vector<node*> out;
    int id;
} v[101];

inline void addmult(int a, int b, int x) { mult[a][b] += x; mult[b][a] += x; }
inline void addedge(node* a, node* b) { a->out.push_back(b); b->out.push_back(a); }

void explore(node* u) // Euler tour + assignment of door color
{
    u->visited = true;
    std::queue<node*> q;
    node* v = u;
    while(!v->out.empty())
    {
        auto w = v->out.back();
        v->out.pop_back();
        if(mult[v->id][w->id])
        {   // If we haven't visited through all multiplicities of this edge, mark it visited
            // by decreasing its multiplicity, and then assign the color
            addmult(v->id, w->id, -1);
            if(!mult[v->id][w->id])
                color[v->id][w->id] = true;
            v = w;
            q.push(v);
        }
    }
    while(!q.empty())
    {
        explore(q.front());
        q.pop();
    }
}

void even(int n) // Adds edges between all nodes of odd degree
{
    std::vector<node*> odd;

    for(int i = 1; i <= n; i++)
        if(v[i].out.size() % 2)
            odd.push_back(&v[i]);
    for(int i = 0; i < odd.size(); i += 2)
    {
        addedge(odd[i], odd[i+1]);
        addmult(odd[i]->id, odd[i+1]->id, 1);
    }
}

int main()
{
    int n, m;
    std::ios::sync_with_stdio(false);
    std::cin >> n;
    std::vector<std::vector<int>> input(n+1);
    for(int i = 1; i <= n; i++)
        v[i].id = i;

    for(int i = 1; i <= n; i++)
    {
        std::cin >> m;
        input[i].reserve(m);
        for(int j = 1; j <= m; j++)
        {
            int u;
            std::cin >> u;
            input[i].push_back(u);
            if(i < u) // Don't add edges twice
            {
                addmult(v[i].id, v[u].id, 1);
                addedge(&v[i], &v[u]);
            }
        }
    }

    even(n);

    for(int i = 1; i <= n; i++)
        if(!v[i].visited)
            explore(&v[i]);

    for(int i = 1; i <= n; i++)
    {
        for(int j = 0; j < input[i].size(); j++)
            std::cout << (color[i][input[i][j]] ? 'Y' : 'G') << " ";
        std::cout << std::endl;
    }
    return 0; 
}