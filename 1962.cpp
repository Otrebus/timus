/* 1962. In Chinese Restaurant - http://acm.timus.ru/problem.aspx?num=1962
 *
 * Strategy:
 * We calculate the number of circular arrangements, but where we treat each group connected by
 * constraints as a single element. Such a group must have the topology of a path, that is, no
 * branches or loops (unless the loop consists of the entire input). Each such group with more than
 * two members can be oriented two ways. The final formula is then (q-1)!2^p where q is the total
 * number of groups and p is the number of groups with more than one member.
 *
 * Performance:
 * O(n), runs in 0.015s using 276KB memory.
 */

#include <vector>
#include <iostream>
#include <map>
#include <algorithm>

const int maxn = 100;
const int mod = 1000000007;

struct Node
{
    Node* parent;
    int rank, degree;
} nodes[maxn+1];

bool used[maxn+1][maxn+1];

// Disjoint-set forests are used to form the groups
Node* find(Node* s)
{
    return (s != s->parent) ? (s->parent = find(s->parent)) : s->parent;
}

void link(Node* a, Node* b)
{
    if(a->rank > b->rank)
        std::swap(a, b);
    a->parent = b;
    if(a->rank == b->rank)
        b->rank++;
}
 
void join(Node* a, Node* b)
{
    link(find(a), find(b));
}

long long fac(long long n)
{
    long long result = 1;
    for(int i = 2; i <= n; i++)
        result = (result * i) % mod;
    return result;
}

int main()
{
    std::map<Node*, std::vector<Node*>> s;
    for(int i = 0; i <= maxn; i++)
        nodes[i].parent = nodes+i;
    int n, m, x, y;
    std::cin >> n >> m;
    for(int i = 1; i <= m; i++)
    {
        std::cin >> x;
        if(!used[std::min(x,i)][std::max(x,i)])
        {
            nodes[x].degree++;
            nodes[i].degree++;
            used[std::min(x,i)][std::max(x,i)] = true;
        }
        join(nodes+x, nodes+i);
    }

    for(int i = 1; i <= n; i++)
    {
        s[find(nodes+i)].push_back(nodes+i);
        if(nodes[i].degree > 2) // This is a branch: a person wants to sit next to >2 others
        {
            std::cout << 0;
            return 0;
        }
    }
    int p = 0;
    for(auto group : s)
    {
        if(group.second.size() > 1)
        {
            p++;
            // Calculate the total degree of the group to make sure it's not a loop
            int totdeg = 0;
            for(auto node : group.second)
                totdeg += node->degree;
            if(totdeg >= group.second.size()*2 && group.second.size() != n)
            {
                std::cout << 0;
                return 0;
            }
        }
    }
    long long ans = fac(s.size()-1);
    if(n > 2) // Groups of size 2 can't be rearranged
        for(int i = 0; i < p; i++)
            ans = (ans*2) % mod; // 2^p, adds the factor for reorienting all the nonsingleton groups
    std::cout << ans << "\n";
}