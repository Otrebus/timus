/* 1553. Caves and Tunnels - http://acm.timus.ru/problem.aspx?num=1553
 *
 * Strategy:
 * Use heavy-light decomposition to represent the tunnel tree, and use an RMQ structure to query
 * the chains of the decomposition quickly. RMQ is also used to implement the required LCA queries.
 *
 * Performance:
 * O(Q*log^2 N), runs the test cases in 1.809s using 22232KB memory.
 */

#include <vector>
#include <algorithm>

const int maxn = 100000;

struct Rmq
{
    std::vector<int> A;
    int size;

    int query(int l, int r, int i, int left, int right)
    {
        if(l <= left && r >= right)
            return A[i];
        int newl = (left+right)/2, newr = newl + 1;
        int minleft = std::numeric_limits<int>::max(), minright = minleft;
        if(l <= newl)
            minleft = query(l, r, i*2, left, newl);
        if(r >= newr)
            minright = query(l, r, i*2+1, newr, right);
        return std::min(minleft, minright);
    }

public:
    int getSize() const
    {
        return size;
    }

    Rmq(int s = 1) :size(s)
    {
        int vSize = 2;
        for(--s; s; s >>= 1)
            vSize *= 2;
        A = std::vector<int>(vSize);
    }

    int query(int l, int r)
    {
        return query(l, r, 1, 1, A.size()/2);
    }

    void inc(int i, int val)
    {
        i = A.size()/2+i-1;
        A[i] += val;
        while(i >>= 1)
    
        A[i] = std::min(A[i*2], A[i*2+1]);
    }
};

struct Node;

struct Chain // Represents one 
{
    Rmq chain;
    Node* parent; // Parent chain, the one this is attached to
} chains[maxn+1];

int nchains = 0;

struct Node
{
    std::vector<Node*> v; // Children
    Chain* chain; // Chain that this node belongs to
    int chainPos, depth, id, lcaid; // Position in chain, depth of this node, preorder id, lca id
} nodes[maxn+1];

Rmq lcaRmq;
int nodeMap[maxn*2];

int calcDepth(Node* node)
{
    int d = 1;
    for(auto& n : node->v)
        d = std::max(d, calcDepth(n)+1);
    node->depth = d;
    return d;
}

void createChains(Node* node) // Creates the heavy-light chains
{
    auto deepestNode = node->v.empty() ? 0 
        : *std::min_element(node->v.begin(), node->v.end(),
                            [] (Node* a, Node* b) { return a->depth > b->depth; } );
    for(auto n : node->v)
    {
        if(n != deepestNode) // Create new chains for these nodes
        {
            n->chain = &chains[++nchains];
            chains[nchains].chain = Rmq(n->depth);
            chains[nchains].parent = node;
            n->chainPos = 1;
        }
        else // Deepest node contains this chain
        {
            n->chain = node->chain;
            n->chainPos = node->chainPos + 1;
        }
        createChains(n);
    }
}

int preOrder(Node* node, int id) // Assigns each node in the tree a preorder traversed id number
{
    node->id = id++;
    nodeMap[node->id] = node-nodes;
    for(int i = 0; i < node->v.size(); i++)
        id = preOrder(node->v[i], id);
    return id;
}

int lcaOrder(Node* node, int id) // Assigns each node its euler-traversed id number
{
    for(int i = 0; i < node->v.size(); i++)
    {
        Node* n = node->v[i];
        n->lcaid = id;
        lcaRmq.inc(id++, n->id);
        id = lcaOrder(n, id);
        lcaRmq.inc(id++, node->id);
    }
    return id;
}

void eraseParents(Node* node) // Goes through the tree and erases the parent from each children list
{
    for(int i = 0; i < node->v.size(); i++)
        node->v[i]->v.erase(std::remove_if(node->v[i]->v.begin(), node->v[i]->v.end(), 
                            [node] (Node* n) { return n == node; }), node->v[i]->v.end());
    for(int i = 0; i < node->v.size(); i++)
        eraseParents(node->v[i]);
}

int lca(int a, int b)
{
    int i = nodes[a].lcaid;
    int j = nodes[b].lcaid;
    if(i > j)
        std::swap(i, j);
    return nodeMap[lcaRmq.query(i, j)];
}

int query(int a, int b) // Queries the tree between a and b for the minimum value
{
    Chain* cur = nodes[a].chain;
    int curPos = nodes[a].chainPos;
    int min = std::numeric_limits<int>::max();
    Node* lc = &nodes[lca(a, b)];
    Chain* l = lc->chain;
    while(true)
    {
        if(cur == nodes[b].chain) // This chain contains both a and b
        {
            int bPos = nodes[b].chainPos;
            if(curPos > bPos)
                std::swap(curPos, bPos);
            min = std::min(cur->chain.query(curPos, bPos), min);
            break;
        }
        else if(cur == l) // We reached the chain that contains the lca
        {
            int bPos = lc->chainPos;
            if(curPos > bPos)
                std::swap(curPos, bPos);
            min = std::min(cur->chain.query(curPos, bPos), min);
            break;
        }
        else // Recurse upwards
            min = std::min(min, cur->chain.query(1, curPos));
        curPos = cur->parent->chainPos;
        cur = cur->parent->chain;
    }
    return min;
}

void initLca(Node* root)
{
    lcaRmq.inc(1, root->id);
    root->lcaid = 1;
    lcaOrder(root, 2);
}

void initChains(Node* root)
{
    root->chain = &chains[++nchains];
    chains[nchains].chain = Rmq(root->depth);
    chains[nchains].parent = 0;
    root->chainPos = 1;
    createChains(root);
}

void init(int a)
{
    auto root = nodes + a;
    eraseParents(root);
    calcDepth(root);
    initChains(root);
    preOrder(root, 1);
    initLca(root);
}

int main()
{
    int N, Q, a = 1, b = 1;
    scanf("%d", &N);
    lcaRmq = (N*2);

    for(int i = 0; i < N-1; i++)
    {
        scanf("%d %d", &a, &b);
        nodes[a].v.push_back(nodes+b);
        nodes[b].v.push_back(nodes+a);
    }
    init(a);
    scanf("%d", &Q);
    std::vector<int> out;
    while(Q--)
    {
        char c[2];
        int u, v;
        scanf("%s %d %d", c, &u, &v);
        if(c[0] == 'I')
            nodes[u].chain->chain.inc(nodes[u].chainPos, -v);
        else
            printf("%d\n", -std::min(query(u, v), query(v, u)));
    }
}