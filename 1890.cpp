/* 1890. Money out of Thin Air - http://acm.timus.ru/problem.aspx?num=1890
 *
 * Strategy:
 * Map the tree into an array where all descendants of a node correspond to a continuous sequence
 * of the array. All given operations can then be performed by the use of a range-update range-
 * query data structure built on top of the resulting array.
 *
 * Performance:
 * O(q log n), running the test cases in 0.234s using 6820kb memory.
 */

#include <stdio.h>
#include <vector>
#include <cctype>

class SegmentTree
{
    std::vector<long long> A;
    std::vector<long long> lazy;
    int size;
private:
    void updateimp(int i, long long value)
    {
        int l = 1, r = size;
        int node = 1;
        while(true)
        {
            int mid = l+(1+r-l)/2;
            A[node] += value*(i-l+1);
            if(l == r)
                break;
            if(i >= mid)
            {
                lazy[node*2] += value*(mid-l);
                if(i == r)
                {
                    lazy[node*2+1] += value*(r-mid+1);
                    break;
                }
                node = node*2 + 1;
                l = l + (1+r-l)/2;
            }
            else if(i < mid)
            {
                node = node*2;
                r = r - (r-l)/2-1;
                if(lazy[node])
                {
                    if(r == l)
                    {
                        A[node] += lazy[node];
                        lazy[node] = 0;
                    }
                    else
                    {
                        A[node] += lazy[node];
                        lazy[node*2] += lazy[node]/2;
                        lazy[node*2+1] += lazy[node]/2;
                        lazy[node] = 0;
                    }
                }
            }
        }
    }

    long long query(int nodeLeft, int nodeRight, int i)
    {
        long long sum = 0;
        int l = 1, r = size;
        int node = 1;
        while(true)
        {
            int mid = l+(1+r-l)/2;
            if(l == r)
            {
                sum += A[node];
                break;
            }
            if(i >= mid)
            {
                sum += lazy[node*2] + A[node*2];
                if(i == r)
                {
                    sum += lazy[node*2+1] + A[node*2+1];
                    break;
                }
                node = node*2 + 1;
                if(lazy[node])
                {
                    A[node] += lazy[node];
                    lazy[node*2] += lazy[node]/2;
                    lazy[node*2+1] += lazy[node]/2;
                    lazy[node] = 0;
                }
                l = l + (1+r-l)/2;
            }
            else if(i < mid)
            {
                node *= 2;
                r = r - (r-l)/2-1;
                if(lazy[node])
                {
                    if(r == l)
                    {
                        A[node] += lazy[node];
                        lazy[node] = 0;
                    }
                    else
                    {
                        A[node] += lazy[node];
                        lazy[node*2] += lazy[node]/2;
                        lazy[node*2+1] += lazy[node]/2;
                        lazy[node] = 0;
                    }
                }
            }
        }
        return sum;
    }

public:
    long long query(int l, int r)
    {
        return query(1, size, r) - (l > 1 ? query(1, size, l-1) : 0);
    }

    long long query(int p)
    {
        return query(1, size, p) - (p > 1 ? query(1, size, p-1) : 0);
    }

    SegmentTree(int n)
    {
        for(size = 1; size < n; size *= 2);
        A = std::vector<long long>((size*2),0);
        lazy = std::vector<long long>((size*2),0);
    }

    void update(int left, int right, long long value)
    {
        updateimp(right, value);
        if(left > 1)
            updateimp(left-1, -value);
    }

    void update(int p, long long value)
    {
        updateimp(p, value);
        if(p > 1)
            updateimp(p-1, -value);
    }
};

struct node
{
    int id, maxid;
    std::vector<node*> children;
};

node nodes[50001];

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

int dfs(node* node, int id)
{
    node->id = id;
    for(auto& c : node->children)
        id = dfs(c, id+1);
    return node->maxid = id;
}

int main()
{
    int n = getint(), q = getint(), s0 = getint();
    char op[20];
    std::vector<int> ps;
    ps.reserve(50001);
    for(int i = 1; i < n; i++)
    {
        int p = getint(), s = getint();
        nodes[p].children.push_back(nodes + i);
        ps.push_back(s);
    }
    SegmentTree T(n);
    T.update(1, s0);
    dfs(nodes, 1);
    for(int i = 1; i < n; i++)
        T.update(nodes[i].id, ps[i-1]);
    while(q--)
    {
        scanf("%s", op);
        if(op[0] == 'e') 
        {
            int x = getint(), y = getint(), z = getint();
            long long salary = T.query(nodes[x].id);
            if(salary < y)
                T.update(nodes[x].id, z);
        }
        else
        {
            int x = getint(), y = getint(), z = getint();
            long long sum = T.query(nodes[x].id, nodes[x].maxid);
            sum /= (nodes[x].maxid - nodes[x].id + 1);
            if(sum < y)
                T.update(nodes[x].id, nodes[x].maxid, z);
        }
    }
    for(int i = 0; i < n; i++)
        printf("%lld\n", T.query(nodes[i].id));
    return 0;
}