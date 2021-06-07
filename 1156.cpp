/* 1156. Two Rounds - http://acm.timus.ru/problem.aspx?num=1156
 *
 * Strategy:
 * Treat each constraint as an edge in a graph, and check that the graph is two-colorable. Then,
 * go through each component of the graph and note the difference in the number of both colors
 * (a component of 5 black and 2 white nodes would have a difference of 3). For color differences
 * d1 .. dn, solve the equation +/-d1 +/- ... +/- dn = 0, to figure out which color of nodes of
 * each component to send to which solution set.
 *
 * Performance:
 * O(n^2) (from the subset-sum-similar pseudopolynomial solution), runs in 0.001s using 320KB 
 * memory.
 */

#include <vector>
#include <queue>
#include <algorithm>
#include <functional>
#include <numeric>

class Node
{
public:
    std::vector<Node*> neighbors;
    int number;
    bool color, visited;
};

unsigned long long A[200]; // For the color difference equation solver
unsigned long long* a = A + 100;

Node nodes[101];
std::vector<Node*> sum2[200]; // 
std::vector<Node*>* sum = sum2 + 100;

std::pair<int, int> color(Node* n) // Returns size and color difference of the component connected
{                                  // to this node
    std::queue<Node*> q;
    q.push(n);
    n->visited = true;
    n->color = false;
    int num = 0, sum = 0;
    while(!q.empty())
    {
        auto node = q.front();
        q.pop();
        sum += node->color ? -1 : 1;
        num++;
        for(auto it = node->neighbors.begin(); it < node->neighbors.end(); it++)
        {
            if((*it)->visited && (*it)->color == node->color)
                return std::make_pair(0, 0);
            if(!(*it)->visited)
            {
                (*it)->visited = true;
                (*it)->color = !node->color;
                q.push(*it);
            }
        }
    }
    return std::make_pair(num, sum);
}

void assign(bool s, std::vector<int>* ans, Node* n) // Distributes the edges in a component
{                                                   // into the corresponding solution set
    for(int i = 1; i <= 100; i++)                   // according to the criteria given
        nodes[i].visited = false;
    std::queue<Node*> q;
    n->visited = true;
    q.push(n);
    while(!q.empty())
    {
        auto node = q.front();
        q.pop();
        ans[node->color != s].push_back(node->number);
        for(auto it = node->neighbors.begin(); it < node->neighbors.end(); it++)
        {
            if(!(*it)->visited)
            {
                (*it)->visited = true;
                q.push(*it);
            }
        }
    }
}

unsigned long long solve(std::vector<int>& digits) // Assigns pluses or minuses in front of each
{                                                  // digit to solve the equation sum(digits) = 0
    unsigned long long M = 0; // The "solution", each binary digit represents a plus or minus
    int sum = std::accumulate(digits.begin(), digits.end(), 0);
    int max = std::accumulate(digits.begin(), digits.end(), 0, 
                              [] (int a, int b) { return std::abs(a) + std::abs(b); });
    a[sum] = ~0;
    for(int i = 0; i < digits.size(); i++)
    {
        for(int j = -max; j <= max; j++)
            if(j - 2*digits[i] >= -max && j - 2*digits[i] <= max && !a[j-2*digits[i]])
                a[j-2*digits[i]] = a[j] & ~(1 << i);
        if(a[0])
            return a[0];
    }
    return 0;
}

int main()
{
    int N, M;
    scanf("%d %d", &N, &M);
    for(int i = 0; i < M; i++)
    {
        int x, y;
        scanf("%d %d", &x, &y);
        if(x == y)
            continue;
        nodes[x].neighbors.push_back(nodes + y);
        nodes[y].neighbors.push_back(nodes + x);
    }
    for(int i = 1; i <= N*2; i++)
        nodes[i].number = i;
    for(int i = 1; i <= N*2; i++)
    {
        nodes[i].number = i;
        if(nodes[i].visited)
            continue;
        auto p = color(nodes + i); // Check colorability and calculate size of each component
        if(p.first == 0)
        {
            printf("IMPOSSIBLE\n");
            return 0;
        }
        sum[p.second].push_back(nodes+i);
    }
    bool s = true;
    std::vector<int> ans[2];
    std::vector<int> digits;
    for(int i = -99; i <= 99; i++)
        for(auto it = sum[i].begin(); it < sum[i].end(); it++)
            digits.push_back(i);
    auto v = solve(digits);
    if(v == 0)
    {
        printf("IMPOSSIBLE\n");
        return 0;
    }
    for(int i = 0; i < digits.size(); i++)
    {   // Distribute the colors of the components to the right solution sets
        assign((v >> i) & 1, ans, sum[digits[i]].back());
            sum[digits[i]].pop_back();
    }
    for(auto it = ans[0].begin(); it < ans[0].end(); it++)
        printf("%d ", *it);
    printf("\n");
    for(auto it = ans[1].begin(); it < ans[1].end(); it++)
        printf("%d ", *it);
    return 0;
}