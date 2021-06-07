/* 2013. Neither shaken nor stirred - http://acm.timus.ru/problem.aspx?num=2013
 *
 * Strategy:
 * Simple DFS, but one that terminates when we reach a node that we will give the same state 
 * that it already had rather than when we visit a node previously visited. For example, if we visit
 * a node which previously had an invalue (previously drunk B-52's) of 4 and we bring it a value of
 * 5, the invalue will change to become unknown. This might then spread to other nodes if no B-52s
 * are drunk at this node. But, if a node was already given an invalue of unknown, there is no use
 * to continue as nothing from this point would change.
 *
 * Performance:
 * Linear in the size of the input, runs the test set in 0.202s using 11972KB memory.
 */

#include <stdio.h>
#include <vector>
#include <cctype>

const int unvisited = -1;
const int unknown = -2;

inline int getint()
{
    int ret = 0, ch;
    while(!isdigit(ch = getc(stdin)));
    ret = ch - '0';
    while(isdigit(ch = getc(stdin)))
        ret = ret*10 + ch - '0';
    return ret;
}

struct node
{
    int in; // Drinks drunk at last place we drank a drink
    int n; // Drinks you can drink this place
    int out; // Last number of drinks drunk when we could drink a drink
    std::vector<node*> v;
};

void dfs(node* node, int pre)
{
    if(node->in != unvisited)
    {
        if(pre == node->in || node->in == unknown)
            return; // Nothing will change as a result of this
        else if(pre != node->in) // A conflict, set as unknown and recurse
            node->in = unknown;
    }
    else 
        node->in = pre; // Previously unvisited, set this invalue to what we got
    node->out = node->n == 0 ? node->in : node->n; // If we can't drink here, propagate the invalue,
    for(auto v : node->v)                          // otherwise the drinks we can drink here
        dfs(v, node->out);
}

void print(int n)
{
    if(n == 0)
        printf("sober");
    else if(n == unknown)
        printf("unknown");
    else
        printf("%d", n);
}

int main()
{
    int n = getint();
    std::vector<node> nodes(n+1, { unvisited, 0, 0, {} } );
    for(int i = 1; i <= n; i++)
    {
        nodes[i].n = getint();
        int m = getint();
        while(m--)
            nodes[i].v.push_back(&(nodes[getint()]));
        nodes[i].in = unvisited;
    }
    dfs(&nodes[1], 0);
    for(int i = 1; i <= n; i++)
    {
        print(nodes[i].in);
        putc(' ', stdout);
        print(nodes[i].out);
        putc('\n', stdout);
    }
    return 0;
}