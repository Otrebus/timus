/* 1752. Tree 2 - http://acm.timus.ru/problem.aspx?num=1752
 *
 * Strategy:
 * Let each local longest path in the tree correspond to a sequence in an array, so that searches
 * down the tree can be done with a single lookup. For searches up the tree, an array of the n^2:th
 * ancestors is stored in each array. For searches up the tree and then down some long branch,
 * a "granduncle" pointer is stored in each node which is a pointer to the parent of the longest
 * such branch.
 *
 * Performance:
 * O(n log n) (for the parent array usage, otherwise linear), running the tests in 0.156s using 
 * 4896kb memory.
 */

#include <stdio.h>
#include <vector>
#include <cctype>
#include <utility>
#include <algorithm>

struct node
{
    node *grandUncle, *deepestChild[2];
    std::vector<node*> parent, children;
    int id, depth, maxDepth, depthArrayPos;
};

node* A[20001]; // The array for the locally longest paths
int a = 0;
node nodes[20001];

// Assigns depth information for every node
void dfs(node* const n)
{
    n->deepestChild[0] = n->deepestChild[1] = &nodes[0];

    // Populate depth array
    node* p = n->parent[0];
    for(int i = 0; p && p->parent.size() > i; i++)
    {
        p = p->parent[i];
        n->parent.push_back(p);
    }

    // Assign child depths and remove the parent from every neighbor array
    if(n->parent[0])
        n->children.erase(std::find(n->children.begin(), n->children.end(), n->parent[0]));

    // Recurse and assign the two deepest children
    for(int i = 0; i < n->children.size(); i++)
    {
        node* const child = n->children[i];
        child->depth = n->depth + 1;
        child->parent.push_back(n);
        dfs(child);
        if(child->maxDepth > n->deepestChild[1]->maxDepth)
        {
            if(child->maxDepth > n->deepestChild[0]->maxDepth)
            {
                n->deepestChild[1] = n->deepestChild[0];
                n->deepestChild[0] = child;
            }
            else
                n->deepestChild[1] = child;
        }
    }
    n->maxDepth = std::max(n->depth, std::max(n->deepestChild[0]->maxDepth, 
                                              n->deepestChild[1]->maxDepth));
}

// Calculates depth array positions and granduncles
void dfs2(node* const n)
{
    // This is part of the longest path from our parent to a child, just extend the array
    if(n->parent[0] && n->parent[0]->deepestChild[0] == n)
    {
        n->depthArrayPos = n->parent[0]->depthArrayPos + 1;
        A[n->depthArrayPos] = n;
    }
    else // We start a new array, and a new "longest path"
    {
        n->depthArrayPos = a;
        A[a] = n;
        a += 1 + n->maxDepth - n->depth;
    }
    if(n->parent[0])
    {   // Assign our "granduncle" nodes
        auto candidate = n->parent[0]->deepestChild[0] == n ? n->parent[0]->deepestChild[1] 
                                                            : n->parent[0]->deepestChild[0];
        // If we have a proper candidate for a new granduncle and something to compare it to, and it
        // is actually a longer path, then assign it - otherwise, keep the parents granduncle
        if(candidate->id == 0 || n->parent[0]->grandUncle != 0 
             && candidate->maxDepth - candidate->depth < n->parent[0]->grandUncle->maxDepth 
                - n->parent[0]->grandUncle->depth + n->depth - n->parent[0]->grandUncle->depth)
             n->grandUncle = n->parent[0]->grandUncle;            
        else
             n->grandUncle = candidate;
    }
    for(int i = 0; i < n->children.size(); i++)
        dfs2(n->children[i]);
}

inline int find(int n, int dist)
{
    const node* node = &nodes[n];
    if(dist <= node->maxDepth - node->depth) // Node can be found below us
        return A[node->depthArrayPos + dist]->id;
    else if(dist < node->depth) // Node can be found above us
    {
        for(int i = 0; dist; i++, dist >>= 1)
            if(dist & 1)
                node = node->parent[i];
        return node->id;
    }
    else if(!node->grandUncle)
        return 0;
    // Node can be found by turning at our granduncle and then going down the tree there
    else if(node->grandUncle != 0 && dist <= node->depth - node->grandUncle->depth + 2 
                                      + node->grandUncle->maxDepth - node->grandUncle->depth)
        return A[dist-(node->depth-node->grandUncle->depth+2)+node->grandUncle->depthArrayPos]->id;
    else
        return 0;
}

void init(int n)
{
    for(int i = 1; i <= n; i++)
        nodes[i].id = i;
    nodes[1].parent.push_back(0);
    nodes[1].depth = 1;
    nodes[1].depthArrayPos = a;
    nodes[1].grandUncle = 0;
    dfs(&nodes[1]);
    dfs2(&nodes[1]);
}

inline int getint()
{
    int ret = 0, ch;
    while(!isdigit(ch = getc(stdin)));
    ret = ch - '0';
    while(isdigit(ch = getc(stdin)))
        ret = ret*10 + ch - '0';
    return ret;
}

int main()
{
    const int n = getint(); int q = getint();
    
    for(int i = 1; i < n; i++)
    {
        int x = getint(), y = getint();
        nodes[x].children.push_back(&nodes[y]);
        nodes[y].children.push_back(&nodes[x]);
    }
    init(n);
    while(q--)
    {
        int x = getint(), y = getint();
        printf("%d\n", find(x, y));
    }
}