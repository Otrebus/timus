/* 1521. War Games 2 - http://acm.timus.ru/problem.aspx?num=1521
 *
 * Strategy:
 * Create a complete tree with the n leafs having keys of 1 (corresponding to our soldiers), and 
 * each other node having the sum of its two descendants as its key. Each time we calculate
 * the "successor" (as defined by the problem) of some soldier, we start at his leaf, bubble up
 * a subtraction through the tree, and start traversing the tree from the leaf with the node 
 * keys to our help to find his successor - at each node of the tree we can use its key to 
 * skip an number of soldiers by not visiting the left subtree. For example, if we need to skip
 * an additional 5 soldiers, and are at a node whose left child has a key of 3, we skip that subtree
 * and keep searching in the right subtree for the other 2 soldiers to skip. The solution is
 * somewhat lengthy and there exist shorter ones.
 *
 * Performance:
 * O(n log n), which runs in 0.14s using 1296KB of memory - which is decent, but there are even 
 * faster solutions around.
 */

#include <vector>
#include <algorithm>

class List
{
    std::vector<int> v; // Our tree, indexed like a heap
    int size, bottom, step, current, remains;

    // Calculates the next power of 2 equal or greater to x
    inline int nextpow(int x)
    {
        x--;
        for(int i = 1; i <= 16; i *= 2)
            x |= x >> i;
        return x + 1;
    }

    int log(int x)
    {
        int ret = 0;
        while(x /= 2)
            ret++;
        return ret;
    }

    inline int pow2(int n)
    {
        int ret = 1;
        while(n-- > 0)
            ret *= 2;
        return ret;
    }

    inline int parent(const int& node) const
    {
        return node/2;
    }

    inline int rightchild(const int& node) const
    {
        return node*2 + 1;
    }

    inline int leftchild(const int& node) const
    {
        return node*2;
    }

    inline bool intree(const int& node) const
    {
        return node <= bottom + size && node > 0;
    }

public:
    List(int n, int step) : size(n), step(step), current(step), remains(n)
    {
        v.resize(nextpow(n*2));
        int height = log(n*2 - 1);
        bottom = pow2(height) - 1; // The node immediately preceding the first leaf

        for(int i = bottom + 1; i <= bottom + n; i++) // Leafs have key 1
            v[i] = 1;
        for(int i = bottom; i >= 1; i--) // Internal nodes' keys are the sum their childrens keys
            v[i] = (intree(leftchild(i)) ? v[leftchild(i)] : 0)
                + (intree(rightchild(i)) ? v[rightchild(i)] : 0);
    }

    int remaining()
    {
        return remains;
    }

    int getNext()
    {
        if(remains-- == size) // We're already at the first one so we handle this one directly
            return current;

        int node = bottom + current;
        while(node) // Bubble up a subtraction through the tree
        {
            v[node]--;
            node = parent(node);
        }
        node = bottom + current;

        int k = step; // The remaining number of soldiers that we need to skip
        enum source { left, right, up }; // The direction we arrived from to a node
        source source = right;
        
        // Our main routine - start at a leaf, search our way through the tree until we have
        // skipped the number of soldiers that need to be skipped
        while(k)
        {
            int lc = leftchild(node);
            int rc = rightchild(node);
            if(source == up)
            {
                if(intree(lc) && k > v[lc]) // We can skip the entire left subtree
                {
                    k -= v[lc];
                    source = up;
                    node = rc;
                }
                else if(!intree(lc) && k == v[node]) // Final leaf found
                    k--;
                else // The answer lies in our left subtree, enter it
                {
                    source = up;
                    node = lc;
                }
            }
            else if(source == right) // Always go up when we've come from a right subtree
            {
                source = node == rightchild(parent(node)) ? right : left;
                node = parent(node);
            }
            else // Coming from the left
            {
                if(intree(rc) && k > v[rc]) // We can skip the entire subtree in this case
                {
                    source = node == rightchild(parent(node)) ? right : left;
                    k -= v[rc];
                    node = parent(node);
                }
                else // Our answer lies within the right subtree; we must pursue our destiny there
                {
                    node = rc;
                    source = up;
                }
            }
            if(!intree(node))    // This happens whenever we started somewhere and couldn't find k
            {                    // successors to the right of us in the tree - in this case, we
                node = bottom+1; // wrap around from the leftmost soldier leaf
                k -= v[node];
                source = right;
            }
        }
        return current = node-bottom; // Transform from tree index to soldier number
    }
};

int main()
{
    int N, K;
    scanf("%d %d", &N, &K);

    List list(N, K);
    while(list.remaining())
    {
        int ans = list.getNext();
        // So this is just a faster way of printing a number than having to call printf and
        // do their generalized stuff. It shaves off another 0.1s from the solution
        char s[9];
        int l = 0;
        while (ans)
        {
            s[l++] = ans % 10;
            ans /= 10;
        }
        while (l--)
            putc('0' + s[l], stdout);
        putc(' ', stdout);
    }
    return 0;
}