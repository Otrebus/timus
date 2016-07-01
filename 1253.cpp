/* 1253. Necrologues - http://acm.timus.ru/problem.aspx?num=1253
 *
 * Strategy:
 * Depth-first search with caching and cycle detection.
 *
 * Performance:
 * Linear in the size of the limit and the input and quadratic in N, runs the tests in 0.001s
 * using 7,376KB memory (rated 6/601).
 */

#include <string>
#include <iostream>

const int maxn = 9;
const int limit = 1000000;

bool A[maxn+1][maxn+1]; // Adjacency matrix
bool visited[maxn+1], onstack[maxn+1]; // For caching and cycle detection
int size[maxn+1]; // Size of the output of this node
std::string str[maxn+1]; // The output of this node

bool print(int n, std::string& output)
{
    visited[n] = onstack[n] = true;

    for(int i = 0; i < str[n].size(); i++)
    {
        if(str[n][i] == '*')
        {
            int m = str[n][++i]-'0';
            if(onstack[m]) // Cycle
                return false;
            else if(!visited[m]) // Haven't visited, so recurse and cache
            {
                std::string tmp;
                if(!print(m, tmp))
                    return false;
                str[m] = std::move(tmp);
            }
            size[n] += size[m];
            output.append(str[m].begin(), str[m].end());
        }
        else
            output.push_back(str[n][i]);
    }
    onstack[n] = false;
    return true;
}

int main()
{
    std::ios::sync_with_stdio(false);
    std::string line;
    std::getline(std::cin, line);
    int n = 1, N = std::stoi(line);
    while(std::getline(std::cin, line))
    {
        int pre = n;
        for(int i = 0; i < line.size(); i++)
        {
            if(line[i] == '#')
                n++;
            else
            {
                str[n].push_back(line[i]);
                if(line[i] == '*')
                {
                    A[n][line[++i]-'0'] = true;
                    str[n].push_back(line[i]);
                }
                else
                    size[n]++;
            }
        }
        if(line.back() != '#')
            size[n]++;
        if(pre == n)
            str[n].push_back('\n');
    }
    std::string output;
    std::fill(visited, visited+maxn+1, false);
    std::cout << ((print(1, output) && size[1] <= limit) ? output : "#");
}