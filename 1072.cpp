/* 1072. Routing - http://acm.timus.ru/problem.aspx?num=1072
 *
 * Strategy:
 * BFS, but we add all the computers of all the subnets that a computer belongs to when we add
 * the computer to the BFS queue.
 *
 * Performance:
 * O(NK), runs the test cases in 0.015s using 420KB memory.
 */

#include <iostream>
#include <unordered_map>
#include <vector>
#include <string>
#include <queue>
#include <stack>

unsigned int toNum(std::string str) // Converts an ip string to its 32 bit representation
{
    int p = 0;
    unsigned int num = 0;
    for(int i = 0; i < 4; i++, p++)
    {
        std::string s;
        while(str[p] != '.' && p < str.length())
            s += str[p++];
        num |= ((unsigned int) std::stoi(s)) << ((3 - i)*8);
    }
    return num;
}

struct node // Graph node representing a computer
{
    int id;
    node* p; // Parent node in the BFS search
    std::vector<unsigned int> e; // Numbers of the subnets it belongs to
    bool visited;
};

std::unordered_map<unsigned int, std::vector<node*>> m; // Maps a subnet to the nodes it contains
node nodes[91];

// Adds all the nodes of all subnets connected to the given node to the queue
inline void addNode(node* n, std::queue<node*>& q)
{
    for(auto i : n->e)
        for(auto v : m[i])
            if(!v->visited)
                q.push(v), v->p = n, v->visited = true;
}

int main()
{
    int N;
    std::cin >> N;
    for(int i = 1; i <= N; i++)
    {
        int K;
        std::cin >> K;
        while(K--)
        {
            std::string ip, mask;
            std::cin >> ip >> mask;
            auto subnet = toNum(ip) & toNum(mask);
            nodes[i].id = i;
            nodes[i].e.push_back(subnet);
            m[subnet].push_back(nodes+i);
        }
    }
    int s, f;
    std::cin >> s >> f;
    std::queue<node*> Q;
    nodes[s].visited = true;
    addNode(nodes+s, Q);
    while(!Q.empty()) // BFS
    {
        std::vector<node*> v;
        auto n = Q.front();
        addNode(n, Q);
        Q.pop();
    }
    std::stack<node*> st;
    if(!nodes[f].visited)
        std::cout << "No" << std::endl;
    else
    {
        std::cout << "Yes" << std::endl;
        for(auto v = nodes+f; v; v = v->p)
            st.push(v);
        while(!st.empty())
            std::cout << st.top()->id << " ", st.pop();
    }
}