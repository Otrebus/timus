/* 2030. Awesome Backup System - http://acm.timus.ru/problem.aspx?num=2030
 *
 * Strategy:
 * Number the nodes in the tree incrementally in a breadth-first fashion, and then run the specified
 * operations of the nodes on the corresponding intervals of a ranged-sum, point query structure
 * (in this case a segment tree).
 *
 * Performance:
 * O(n + mlog n), running the tests at a leisurely time of 1.435s, using 9716KB memory.
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
    void update(int node, int nodeLeft, int nodeRight, int left, int right, int value)
    {   // Alternate name might be rangeSum - adds value to all elements between left and right
        if(right < nodeLeft || left > nodeRight)
            return;
        if(left == nodeLeft && right == nodeRight)
            A[node] = (A[node] + value) % 1000000007;
        else if(right <= nodeRight || left >= nodeLeft)
        {
            int lc = node*2, rc = node*2+1;
            int mid = nodeLeft + (nodeRight-nodeLeft+1)/2;
            update(lc, nodeLeft, mid-1, std::max(left, nodeLeft), std::min(right, mid-1), value);
            update(rc, mid, nodeRight, std::max(left, mid), std::min(right, nodeRight), value);
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
        update(1, 1, size, left, right, value);
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

int main()
{
    int n, m;
    std::queue<node*> q;
    std::vector<int> init;
    scanf("%d", &n);
    SegmentTree t(n);

    for(int i = 1; i <= n; i++)
        nodes[i].number = i;

    for(int i = 1; i <= n; i++)
    {
        int x;
        scanf("%d", &x);
        init.push_back(x); // We have to wait with assigning this since we have to do a bfs
    }                      // to figure out the new node numbering first

    for(int i = 0; i < n-1; i++)
    {
        int x, y;
        scanf("%d %d", &x, &y);
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

    scanf("%d", &m);
    while(m--)
    {
        int option, number;
        scanf("%d %d", &option, &number);
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