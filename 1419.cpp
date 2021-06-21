/* 1419. Maps of the Island Worlds - http://acm.timus.ru/problem.aspx?num=1419
 *
 * Strategy:
 * Use a version of Tarjan's strongly connected components algorithm to determine whether the given
 * graph is connected appropriately.
 *
 * Performance:
 * O(mn), runs the tests in 0.015s using 1,664KB memory.
 */

#include <vector>
#include <iostream>

const int maxw = 99, maxh = 99;
int m, n, d, weak;

char input[maxh*2][maxw*2+10];

struct node {
    bool visited, exists; // If we visited; whether this corresponds to an actual node
    int m; // Visit order
    std::vector<node*> v;
} nodes[maxw*2][maxh*2];

int dfs(node* n, node* parent) {
    // The algorithm mentioned in the header comment, it checks that each node in the dfs tree
    // forms a cycle via a path to a node which is closer to the root than itself
    if(n->visited)
        return n->m;
    int m = d++, mp = m;
    n->m = m;
    n->visited = true;
    for(auto v : n->v)
        if(v != parent)
            m = std::min(m, dfs(v, n));
    weak |= m && m == mp;
    return m;
}

bool run() {

    // Read the input and connect the nodes accordingly, check that the map makes sense
    for(int y = 0; y < n; y++) {
        for(int x = 0; x < m; x++) {
            int x1 = x, y1 = y, x2 = x, y2 = y;
            switch(input[y][x]) {
                case 'O':
                case ' ':
                    continue;
                case '-':
                    x1 = x-1, x2 = x+1;
                    break;
                case '|':
                    y2 = y+1, y1 = y-1;
                    break;
                case '\\':
                    y1 = y-1, x1 = x-1, y2 = y+1, x2 = x+1;
                    break;
                case '/':
                    y1 = y+1, x1 = x-1, y2 = y-1, x2 = x+1;
                    break;
                default:
                    return false;
            }

            // Is the path leading to something silly?
            if(x1 < 0 || y1 < 0 || x1 >= m || y1 >= n || x2 < 0 || y2 < 0 || x2 >= m || y2 >= n
                      || !nodes[x1][y1].exists || !nodes[x2][y2].exists )
                return false;

            // Connect the two nodes
            nodes[x1][y1].v.push_back(&nodes[x2][y2]);
            nodes[x2][y2].v.push_back(&nodes[x1][y1]);
        }
    }

    node* s = 0; // Start node for dfs
    // Pick a start node, check that each node has three edges
    for(int y = 0; y < n; y++) {
        for(int x = 0; x < m; x++) {
            if(nodes[x][y].exists) {
                 if(nodes[x][y].v.size() != 3)
                    return false;
                s = &nodes[x][y];
            }
        }
    }
    if(!s) // If there are no nodes, abort
        return false;
    dfs(s, 0); // Run Tarjan

    // Check that we visited all nodes
    for(int y = 0; y < n; y++)
        for(int x = 0; x < m; x++)
            if(nodes[x][y].exists && !nodes[x][y].visited)
                return false;
    return !weak;
}

int main() {
    scanf("%d %d", &m, &n);
    fgets(input[0], 10, stdin);
    for(int y = 0; y < n; y++) {
        fgets(input[y], m+12, stdin);
        for(int x = 0; x < m; x++)
            nodes[x][y].exists = input[y][x] == 'O';
    }

    printf(run() ? "Island world" : "Just a picture");
}
