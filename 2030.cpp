/* 2030. Awesome Backup System - http://acm.timus.ru/problem.aspx?num=2030
 *
 * Strategy:
 * Number the nodes in the tree incrementally in a breadth-first fashion, and then run the specified
 * operations of the nodes on the corresponding intervals of a ranged-sum, point query structure
 * (in this case a segment tree).
 *
 * Performance:
 * O(n + mlog n), running the tests in 0.608s, using 9668KB memory.
 */

#include <algorithm>
#include <iostream>
#include <vector>
#include <queue>
#include <cstring>

class SegmentTree
{
    int* A;
    int size;
private:
    void updateimp(int i, int value) // Updates all values from 1 to i
    {
        int l = 1, r = size;
        int node = 1;
        while(true)
        {
            int mid = l+(1+r-l)/2;
            if(i == l && i == r)
            {
                A[node] = (A[node] + value) % 1000000007;
                break;
            }
            if(i >= mid)
            {
                A[node*2] = (A[node*2] + value) % 1000000007;
                node = node*2 + 1;
                l = l + (1+r-l)/2;                
            }
            else if(i < mid)
            {
                r = r - (r-l)/2-1;
                node = node*2;
            }
        }
    }

    int query(int node, int nodeLeft, int nodeRight, int n)
    {
        int sum = 0;
        while(true)
        {
            sum = (sum + A[node]) % 1000000007;
            if(n == nodeLeft && n == nodeRight)
                break;
            int mid = nodeLeft + (nodeRight-nodeLeft+1)/2;
            if(n < mid)
                node = node*2, nodeRight = mid-1;
            else
                node = node*2+1, nodeLeft = mid;
        }
        return sum;
    }

public:
    int query(int n)
    {
        return query(1, 1, size, n);
    }

    SegmentTree(int n)
    {
        for(size = 1; size < n; size *= 2);
        A = new int[size*2];
        std::memset(A, 0, (size*2)*sizeof(int));
    }

    void update(int n, int value)
    {
        A[size+n-1] = (A[size+n-1] + value) % 1000000007;
    }

    void update(int left, int right, int value)
    {
        updateimp(right, value);
        if(left > 1)
            updateimp(left-1, 1000000007-value);
    }
};

struct node
{
    int left, right, id, number; // id is the bfs-made numbers, number is the input supplied number
    node* parent;
    std::vector<node*> children;
};

node nodes[100001];
int map[100001]; // Maps the supplied input node id numbers to the bfs-made id numbers

inline int getint()
{
    int ret = 0;
    int ch;
    while(!isdigit(ch = getc(stdin)));
    ret = ch-'0';
    while(isdigit(ch = getc(stdin)))
        ret = ret*10+ch-'0';
    return ret;
}

int main()
{
    int n, m;
    std::queue<node*> q;
    std::vector<int> init;
    n = getint();
    SegmentTree t(n);

    for(int i = 1; i <= n; i++)
        nodes[i].number = i;

    for(int i = 1; i <= n; i++)
    {
        int x = getint();
        init.push_back(x); // We have to wait with assigning this since we have to do a bfs
    }                      // to figure out the new node numbering first

    for(int i = 0; i < n-1; i++)
    {
        int x, y;
        x = getint(); y = getint();
        nodes[x].children.push_back(nodes+y);
        nodes[y].children.push_back(nodes+x);
    }

    q.push(nodes+1+rand() % n); // Root
    q.front()->parent = q.front();
    int cur = q.front()->id = 1; // cur is the incremental id
    while(!q.empty()) // Bfs
    {
        node* n = q.front();
        q.pop();
        map[n->number] = n->id;
        int pre = cur;
        for(auto it = n->children.begin(); it < n->children.end(); ++it)
        {
            node* v = *it;
            if(!v->parent)
            {
                v->parent = n;
                v->id = ++cur;
                q.push(v);
            }
        }
        if(cur != pre)
        {
            n->left = pre+1;
            n->right = cur;
        }
        else // No kids
            n->left = n->right = -1;
    }

    for(int i = 1; i <= n; i++)
        t.update(map[i], init[i-1]);

    m = getint();
    while(m--)
    {
        int option, number;
        option = getint(); number = getint();
        if(option == 1)
        {
            auto node = nodes+number;
            if(node->left != -1) // Update the interval of numbers corresponding to the children
                t.update(node->left, node->right, t.query(map[number]));
            if(node->parent != node) // If there is a parent to this node, update it
                t.update(map[node->parent->number], t.query(map[number]));
        }
        else
        {
            int num = t.query(map[number]);
            printf("%d\n", num);
        }
    }
    return 0;
}