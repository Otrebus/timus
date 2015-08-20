/* 1282. Game Tree - http://acm.timus.ru/problem.aspx?num=1282
 *
 * Strategy:
 * Minimax.
 *
 * Performance:
 * O(N), runs in 0.001s and uses 272KB of memory.
 */

#include <stdio.h>
#include <vector>
#include <algorithm>

struct Node
{
    std::vector<Node*> children;
    char result;
};

Node nodes[1001];

int minmax(int player, Node* node)
{
    if(node->children.empty())
        return node->result;
    int const & (*f) (int const &, int const &) = std::max<int>;
    if(player < 0)
        f = std::min<int>;
    int min = -1*player;
    for(auto it = node->children.begin(); it < node->children.end(); it++)
        min = f(min, minmax(-1*player, *it));
    return min;
}

int main()
{
    int N;
    scanf("%d", &N);
    for(int i = 2; i <= N; i++)
    {
        char type[2];
        int parent, result;
        scanf("%s %d", type, &parent);
        nodes[parent].children.push_back(nodes + i);
        if(type[0] == 'L')
        {
            scanf("%d", &result);
            nodes[i].result = result;
        }
    }
    int result = minmax(1, nodes+1);
    printf("%s%d", result > 0 ? "+" : "", result);
}