/* 1715. Another Ball Killer - http://acm.timus.ru/problem.aspx?num=1715
 *
 * Strategy:
 * Straightforward simulation according to the rules given.
 *
 * Performance:
 * O((MN)^2), runs the tests in 0.203s using 744KB memory.
 */

#include <stdio.h>
#include <vector>
#include <algorithm>
#include <memory>

struct cell {
    char c; // The color of the cell
    bool v; // Visited?
    int n;  // Number of cells of this figure
} cells[50][50]; // Input
cell cs[50][50]; // The balls we simulate

bool b[256]; // If color exists at all
int n, m;

std::vector<std::pair<int, int>> dirs = { { -1, 0 }, { 1, 0 }, { 0, -1 }, { 0, 1 } };

bool valid(int i, int j, int c) {
    // Checks if we can move into this coordinate when we flood-fill
    return i < m && i >= 0 && j < n && j >= 0 && cs[i][j].c == c && !cs[i][j].v;
}

void count(int i, int j, char c, int& N) {
    // Counts the number of balls in the group at this position
    if(!valid(i, j, c))
        return;
    cs[i][j].v = true;
    N += 1;

    for(auto& d : dirs)
        count(i + d.first, j + d.second, c, N);
}

void remove(int i, int j, char c) {
    // Removes all balls of the group at this position
    if(!valid(i, j, c))
        return;
    cs[i][j].c = 0;
    cs[i][j].v = true;

    for(auto& d : dirs)
        remove(i + d.first, j + d.second, c);
}

bool isEmpty(int i) {
    // Checks if this column is empty of balls
    for(int j = 0; j < n; j++)
        if(cs[i][j].c)
            return false;
    return true;
}

int run(char c) {

    std::memcpy(cs, cells, sizeof(cells));

    int res = 0;
    while(true) {

        std::vector<cell*> v;
        // Go from the bottom left corner up to the top right and add a representative ball per
        // group to v
        for(int j = n-1; j >= 0; j--) {
            for(int i = 0; i < m; i++) {
                if(!cs[i][j].v && cs[i][j].c) {
                    int N = 0;
                    count(i, j, cs[i][j].c, N);
                    cs[i][j].n = N;
                    if(N > 1)
                        v.push_back(&cs[i][j]);
                }
            }
        }

        // Sort v, stable for tie-breaking
        std::stable_sort(v.begin(), v.end(), [] (auto c1, auto c2) { return c1->n > c2->n; });

        // Pick the highest ranked ball of another color, or our own if we have to
        cell *r = 0, *u = 0;
        for(int i = 0; i < v.size() && !r; i++)
            if(v[i]->c != c)
                r = v[i];
        if(!r && v.size())
            r = v[0];
        if(!r)
            return res;

        res += r->n*(r->n-1); // Add the score

        for(int i = 0; i < m; i++)
            for(int j = 0; j < n; j++)
                cs[i][j].v = false;

        remove((r-&cs[0][0])/50, (r-&cs[0][0])%50, r->c); // Remove the group we chose

        // Make it rain, column by column
        for(int i = 0; i < m; i++) {
            int J = n-1;
            for(int j = n-1; j >= 0; j--)
                if(cs[i][j].c)
                    cs[i][J--].c = cs[i][j].c;
                while(J >= 0)
                    cs[i][J--].c = 0;
        }

        // Compress sideways
        std::vector<int> c; // Empty column indices
        for(int i = 0; i < m; i++)
            if(isEmpty(i))
                c.push_back(i);

        if(!c.empty() && c[0] != m-c.size()) {
            // For each row, only retain stuff not mentioned in c
            for(int j = 0; j < n; j++) {
                int k = 0, I = 0;
                for(int i = 0; i < m; i++) {
                    if(k >= c.size() || i != c[k])
                        cs[I++][j].c = cs[i][j].c;
                    else
                        k++;
                }
                while(I < m)
                    cs[I++][j].c = 0;
            }
        }

        for(int i = 0; i < m; i++)
            for(int j = 0; j < n; j++)
                cs[i][j].v = false;
    }
    return res;
}

int main() {
    scanf("%d %d", &n, &m);
    for(int j = 0; j < n; j++) {
        char c[51];
        scanf("%s", c);
        for(int i = 0; i < m; i++) {
            cells[i][j].c = c[i];
            b[c[i]] = true;
        }
    }
    for(char c : { 'B', 'G', 'R', 'W', 'Y' })
        if(b[c])
            printf("%c: %d\n", c, run(c));
}
