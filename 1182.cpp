/* 1182. Team Them Up! - http://acm.timus.ru/problem.aspx?num=1182
 *
 * Strategy:
 * See the header comment for 1156.cpp. This problem is very similar except we connect those who do 
 * not have mutual knowledge as edges.
 *
 * Performance:
 * O(n^2) (from the subset-sum-similar pseudopolynomial solution), runs in 0.015s using 408KB
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
    std::vector<Node*> v;
    bool color, visited;
};

std::pair<bool, std::vector<bool>> A[201]; // For the color difference equation solver
std::pair<bool, std::vector<bool>>* a = A + 100;

Node nodes[101];
std::vector<Node*> sum2[201]; // 
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
        for(auto it = node->v.begin(); it < node->v.end(); it++)
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
        ans[node->color != s].push_back(node-nodes);
        for(auto it = node->v.begin(); it < node->v.end(); it++)
        {
            if(!(*it)->visited)
            {
                (*it)->visited = true;
                q.push(*it);
            }
        }
    }
}

std::vector<bool> solve(std::vector<int>& digits) // Assigns pluses or minuses in front of each
{                                                 // digit to solve the equation sum(digits) = 0
    int max = std::accumulate(digits.begin(), digits.end(), 0, 
                              [] (int a, int b) { return a + std::abs(b); });
    int sum = std::accumulate(digits.begin(), digits.end(), 0, [] (int a, int b) { return a + b; });
    for(int i = 0; i < 201; i++)
        A[i] = { false, std::vector<bool>(100, 0) };
    a[sum].first = true;
    for(int i = 0; i < digits.size(); i++)
    {
        for(int j = -max; j <= max; j++)
        {
            if(a[j].first && !a[j].second[i] && !a[j-2*digits[i]].first)
            {
                a[j-2*digits[i]].second = a[j].second;
                a[j-2*digits[i]].second[i] = a[j-2*digits[i]].first = true;
            }
        }
        if(a[0].first)
            return a[0].second;
    }
    for(int i = 1; i <= max; i++) // Return the solution closest to balance (0 was handled above)
        if(a[i].first)
            return a[i].second;
        else if(a[-i].first)
            return a[-i].second;
}

bool C[101][101];

int main()
{
    int N;
    scanf("%d", &N);
    for(int i = 1; i <= N; i++)
    {
        int j = 1;
        while(j)
        {
            scanf("%d", &j);
            C[i][j] = true;
        }
    }
    for(int i = 1; i <= N; i++)
        for(int j = 1; j <= N; j++)
            if(i > j && (!C[i][j] || !C[j][i]))
                nodes[i].v.push_back(nodes+j), nodes[j].v.push_back(nodes+i);
    for(int i = 1; i <= N; i++)
    {
        if(nodes[i].visited)
            continue;
        auto p = color(nodes + i); // Check colorability and calculate size of each component
        if(p.first == 0)
        {
            printf("No solution");
            return 0;
        }
        sum[p.second].push_back(nodes+i);
    }
    std::vector<int> ans[2];
    std::vector<int> digits;
    for(int i = -100; i <= 100; i++)
        for(auto it = sum[i].begin(); it < sum[i].end(); it++)
            digits.push_back(i);
    auto v = solve(digits);
    for(int i = 0; i < digits.size(); i++)
    {   // Distribute the colors of the components to the right solution sets
        assign(v[i], ans, sum[digits[i]].back());
            sum[digits[i]].pop_back();
    }
    printf("%d ", ans[0].size());
    for(auto it = ans[0].begin(); it < ans[0].end(); it++)
        printf("%d ", *it);
    printf("\n");
    printf("%d ", ans[1].size());
    for(auto it = ans[1].begin(); it < ans[1].end(); it++)
        printf("%d ", *it);
    return 0;
}