/* 1871. Seismic Waves - http://acm.timus.ru/problem.aspx?num=1871
 *
 * Strategy:
 * Bellman-Ford.
 *
 * Performance:
 * O(n^3), runs the test set in 0.015s using 684KB memory.
 */

#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <limits>
#include <algorithm>

struct node
{
    int l;    // Length of the name of this user
    int recv; // Shortest length (currently) of the message received
    int send; // Shortest length (currently) of the message we retweet
};

std::vector<std::pair<node*, node*>> edges;
node nodes[100];

int main()
{
    std::map<std::string, node*> m; // Mapping from string to node
    std::string str, str2, s, line;
    int n, k, c = 0;
    std::cin >> n;

    // Read input
    for(int i = 0; i < n; i++)
    {
        std::cin >> str >> k;
        if(m.find(str) == m.end())
            m.insert( { str, nodes + c++ } );
        while(k--)
        {
            std::cin >> str2;
            if(m.find(str2) == m.end())
                m.insert( { str2, nodes + c++ } );
            edges.push_back( { m[str], m[str2] } );
        }
        m[str]->l = str.length();
    }
    std::cin.ignore();
    std::getline(std::cin, line);

    // Initialize costs in the graph
    for(auto x : m)
        x.second->send = x.second->recv = std::numeric_limits<int>::max()/2;
    nodes[0].send = nodes[0].recv = line.length();

    // Relax all edges in regards to the retweet length
    for(int i = 0; i < n-1; i++)
        for(int j = 0; j < edges.size(); j++)
            edges[j].second->send = std::min(edges[j].second->send, 
                                             edges[j].first->send + edges[j].first->l + 6);
    // Calculate the final incoming tweet length for each node
    for(int j = 0; j < edges.size(); j++)
        edges[j].second->recv = std::min(edges[j].second->recv, edges[j].first->send);
    
    // Output the answer
    std::vector<std::string> out;
    for(auto n : m)
        if(n.second->recv <= 140)
            out.push_back(n.first);
    std::cout << out.size() << "\n";
    for(auto s : out)
        std::cout << s << "\n";
    return 0;
}