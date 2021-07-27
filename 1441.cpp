/* 1441. From the History of Gringotts Bank - http://acm.timus.ru/problem.aspx?num=1441
 *
 * Strategy:
 * As long as there are any odd-degree vertices, trace a path starting at such a vertex, removing
 * each visited edge from the graph until we reach a dead end, adding the path to the answer set.
 * When there are no longer any odd vertices, find some vertex v whose edges we've partially removed
 * already, extract a cycle (which it is guaranteed to form since it and all the remaining vertices
 * have an even degree) and splice it into some extracted path that ran through v. If there are no
 * odd-degree vertices or any even-degree vertices we have visited, pretend some unvisited vertex is
 * odd and recurse.
 *
 * Performance:
 * O(n+m), runs the tests in 0.062s using 8,856KB memory.
 */

#include <stdio.h>
#include <unordered_set>

std::unordered_set<int> odd, semi, un; // Odd-degree, partially visited and unvisited vertices

const int maxn = 20001;
int a, h = 1;
int ans[maxn], next[maxn*2], val[maxn*2]; // We store the answers in a linked list

struct vertex {
    std::unordered_set<int> edges;
    int s; // The linked list node of an answer path that represents this vertex
} verts[maxn];

int add(int i, int x) { // Adds a linked-list node with value (vertex) x after list node i
    val[h] = x;
    return (next[i] = h++);
}

void reclassify(int v) { // Adds/removes vertex n from the sets that classify its degree
    semi.erase(v);
    un.erase(v);
    if(verts[v].edges.size())
        semi.insert(v);
    odd.erase(v);
    if(verts[v].edges.size() % 2)
        odd.insert(v);
}

void dfs(int v, int l) { 
    // Traces a path starting at vertex v, adding its vertices to linked list l
    while(!verts[v].edges.empty()) {

        auto& vert = verts[v];
        if(!vert.s) // Save the list node so we can splice a cycle to it later
            vert.s = l;

        auto v2 = *(vert.edges.begin());
        l = add(l, v2); // Add a neighboring vertex to the path

        vert.edges.erase(v2); // Remove the edge and its twin representation from the graph
        verts[v2].edges.erase(v);

        reclassify(v); // Reclassify this vertex and its neighbor as even/odd/unvisited
        reclassify(v2);
        v = v2;
    }
}

int main() {
    int n, m, x, y;
    scanf("%d %d", &n, &m);
    for(int i = 1; i <= m; i++) {
        scanf("%d %d", &x, &y);
        verts[x].edges.insert(y);
        verts[y].edges.insert(x);
    }
    for(int i = 1; i <= n; i++) {
        if(verts[i].edges.size() % 2)
            odd.insert(i);
        if(verts[i].edges.size())
            un.insert(i);
    }

    while(odd.size() || semi.size() || un.size()) {
        if(odd.size()) { // We have odd-degree vertices, trace a path starting at one
            int v = *odd.begin();
            dfs(v, ans[a++] = add(h, v));
        } else if(semi.size()) { // Otherwise, trace a cycle starting at some partially visited vert
            int n = *semi.begin(), l = h++;
            auto s = verts[n].s;
            dfs(n, l);
            next[h-1] = next[s], next[s] = next[l]; // Splice the cycle into an existing path
        } else if(un.size()) { // Otherwise pretend some unvisited vertex is odd
            odd.insert(*un.begin());
            un.erase(*un.begin());
        }
    }

    for(printf("%d\n", a); a; a--, printf("\n")) // Print out the answer
        for(int x = ans[a-1]; x; x = next[x])
            printf("%d ", val[x]);
}
