/* 1682. Crazy Professor - http://acm.timus.ru/problem.aspx?num=1682
 *
 * Strategy:
 * Precompute the solutions to b+x*x=i (mod k) for each i, and then simulate and use disjoint
 * sets to find cycles.
 * 
 * Performance:
 * O(k), runs the tests in 0.046s using 6,520KB memory.
 */

#include <iostream>
#include <vector>

const int maxn = 100000;
const int maxi = 2*maxn+10;

std::vector<int> rem[maxi]; // rem[i] is the solutions set to x*x % k == i

// All disjoint set stuff until calc()
struct node
{
    node* parent;
    int rank;
} nodes[maxi];

void link(node* a, node* b)
{
    if(a->rank > b->rank)
        b->parent = a;
    else
    {
        a->parent = b;
        if(a->rank == b->rank)
            b->rank++;
    }
}
 
node* find(node* s)
{
    if(s != s->parent)
        s->parent = find(s->parent);
    return s->parent;
}
 
void join(node* a, node* b) 
{
    link(find(a), find(b));
}
 
int calc(long long k)
{
    for(long long i = 1; i <= k*2+5; i++)
        rem[(i*i)%k].push_back(i), nodes[i].parent = nodes+i;
    for(long long a = 1; ; a++)
    {
        // Find the b's as described in the problem description
        int b1 = (k*maxi - a*a)%k;
        if(b1 && b1 < a)
            if(find(nodes+b1) == find(nodes+a)) // Linked together earlier? This is now a cycle
                return a;
            else
                join(nodes+b1, nodes+a); // Not linked together yet, link now
 
        auto x = (long long) (k*maxi - a)%k;
        for(auto b2 : rem[x])
            if(b2 >= a)
                break;
            else if(b1 != b2)
                if(find(nodes+b2) == find(nodes+a))
                    return a;
                else
                    join(nodes+b2, nodes+a);
    }
}
 
int main()
{
    int x;
    std::cin >> x;
    std::cout << calc(x);
}