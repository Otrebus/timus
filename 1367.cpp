/* 1367. Top Secret - http://acm.timus.ru/problem.aspx?num=1367
 *
 * Strategy:
 * BFS from each secret, but never revisit old cells.
 *
 * Performance:
 * O(HW + S^2), runs the tests in 0.296 using 31,684KB memory.
 */

#include <stdio.h>
#include <queue>
#include <vector>

const int maxn = 1000, maxs = 100;
const char E = 1, N = 2, W = 4, S = 8;

struct cell {
    bool v; // Visited
    char d; // Reachability bitmap
    int s; // Secret no. (4 for each '#', we'll combine the information later)
} A[maxn*2][maxn*2];
bool C[maxs*4+1][maxs*4+1]; // Reachability matrix

int p = 0; // Number of secrets*4
std::pair<int, int> P[maxs*4+1]; // Maps secrets to coordinates

int main() {
    char s[1001];
    int X = 0, Y = 0;
    for(int y = 0; scanf("%s", s) != EOF; y++) {
        for(int x = 0; s[x]; x++) {
            // Assign the reachabilities
            cell& ul = A[x*2][y*2], &ur = A[x*2+1][y*2], &bl = A[x*2][y*2+1], &br = A[x*2+1][y*2+1];
            ul.d = ur.d = bl.d = br.d = ~0;
            switch(s[x]) {
            case '-':
                ul.d = ~S, ur.d = ~S, bl.d = ~N, br.d = ~N;
                break;
            case '|':
                ul.d = ~E, ur.d = ~W, bl.d = ~E, br.d = ~W;
                break;
            case '#':
                ul.s = 1;
            case '+':
                ul.d = ~(E|S), ur.d = ~(W|S), bl.d = ~(E|N), br.d = ~(W|N);
                break;
            }
            X = (x+1)*2;
        }
        Y = (y+1)*2;
    }

    // Count the secrets, map them to cells
    for(int y = 0; y < Y; y += 2)
        for(int x = X-2; x >= 0; x -= 2)
            if(A[x][y].s)
                for(int dx = 0; dx < 2; dx++)
                    for(int dy = 0; dy < 2; dy++)
                        P[A[x+dx][y+dy].s = ++p] = {x+dx, y+dy};

    for(int i = 1; i <= p; i++) {
        auto [x, y] = P[i];

        // BFS from each secret
        std::queue<std::pair<int, int>> Q;
        Q.push( { x, y } );
        std::vector<int> v { A[x][y].s }; // The set of secrets found 

        while(!Q.empty()) {
            auto [x, y] = Q.front();
            Q.pop();

            auto& cell = A[x][y];
            if(cell.v)
                continue;
            cell.v = true;

            if(cell.s) // Cell is a secret
                v.push_back(cell.s);

            // Visit neighboring cells based on visibility
            if((cell.d & N) && y)
                Q.push( { x, y-1 } );
            if((cell.d & E) && x < X-1)
                Q.push( { x+1, y } );
            if((cell.d & S) && y < Y-1)
                Q.push( { x, y+1 } );
            if((cell.d & W) && x)
                Q.push( { x-1, y } );
        }
        for(int i = 0; i < v.size(); i++) // Make sure all found secrets can see each other
            for(int j = 0; j < v.size(); j++)
                C[v[i]][v[j]] = true;
    }

    // Print out the final reachability, consider the 4 id's of each secret
    for(int i = 1; i <= p; i += 4) {
        for(int j = 1; j <= p; j += 4) {
            int found = 0;
            for(int k = 0; !found && k < 4; k++)
                for(int l = 0; !found && l < 4; l++)
                    if(C[i+k][j+l])
                        found = 1;
            printf("%d", found);
        }
        printf("\n");
    }
}
