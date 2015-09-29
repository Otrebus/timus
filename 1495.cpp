/* 1495. One-two, One-two 2 - http://acm.timus.ru/problem.aspx?num=1495
 *
 * Strategy:
 * Breadth first search over all (1|2)+ numbers. The modulus of each tried number is saved in order
 * to prune the search tree.
 *
 * Performance:
 * O(N) since we insert at most N-1 numbers of different moduli into the queue.
 */

#include <string>
#include <iostream>

struct node
{
    int mod, num; // The number is represented in binary, so 10010 represents 21121
    char level; // Length of the number, depth of the bfs tree
};

bool used[1000000];
node Q[1000001];

inline std::string solve(int N)
{
    int front = 0, back = 0;
    if(N < 3)
        return std::to_string(N);
    Q[back++] = node { 1, 0, 1 } ;
    Q[back++] = node { 2, 1, 1 } ;
    while(front != back)
    {
        const node& node = Q[front++];

        for(int i = 0; i <= 1; i++) 
        {
            int newmod = (node.mod*10 + i+1) % N;
            int newnum = (node.num << 1) | i;
            if(newmod == 0)
            {
                std::string ans;
                for(int i = node.level; i >= 0; i--)
                    ans += '1' + ((newnum >> i) & 1);
                return ans;
            }
            if(!used[newmod]) 
            {
                used[newmod] = true;
                if(node.level < 29)
                    Q[back++] = ::node { newmod, newnum, node.level + 1 };
            }
        }
    }
    return "Impossible";
}

int main()
{
    std::ios::sync_with_stdio(false);
    int N;
    std::cin >> N;
    std::cout << solve(N) << std::endl;
}