/* 2080. Wallet - http://acm.timus.ru/problem.aspx?num=2080
 *
 * Strategy:
 * Read the input in reverse, and for each given number we find the position of that number in 
 * the stack (or, if that number is not in the stack yet, pretend it is just outside the stack),
 * output that position (in reverse), and move that number to the top of the stack. In order to
 * quickly find the position of a given number in the stack, it is implemented as a skip list.
 *
 * Performance:
 * O(k log k), runs the tests in 0.093s using 5,836KB memory.
 */

#include <stdio.h>
#include <vector>
#include <random>

const int maxn = 100000;

struct Node
{
    // Previous and next linked node of the given level and the distance to the next linked node
    struct Link { Node* prev, *next; int dist; };
    std::vector<Link> v; // The skip levels - higher indices = longer distances
} nodes[maxn+1];

std::mt19937 mt(10654);
std::uniform_int_distribution<unsigned int> dd(0, 0xFFFF);

int count = 0;

int pop(Node* node)
{
    Node* cur = node;
    int dist; // Position of the given node, to be returned
    if(!node->v.empty())
    {
        dist = node->v.back().prev->v[node->v.size()-1].dist - 1;
        // Remove the node and update the links around it
        for(int i = 0; i < node->v.size(); i++)
        {
            node->v[i].prev->v[i].next = node->v[i].next;
            node->v[i].prev->v[i].dist += node->v[i].dist - 1;
            if(node->v[i].next)
                node->v[i].next->v[i].prev = node->v[i].prev;
        }
        cur = node->v.back().prev;
        // Iterate back to the start and update the links that overshoot the deleted node
        for(int i = node->v.size()-1; nodes[0].v[i].next;)
        {
            while(i == cur->v.size() - 1)
                dist += (cur = cur->v[i].prev)->v[i].dist;
            while(i < cur->v.size() - 1)
                cur->v[++i].dist--;
        }
    }
    else
        dist = count++;
    // Randomize the height (number of skip levels) of this node
    int m = dd(mt), h = 1;
    while((m >>= 1) & 1)
        h++;
    node->v.resize(h);
    // Add the node to the front and add links accordingly
    for(int i = 0; i < h; i++)
    {
        node->v[i] = { &nodes[0], nodes[0].v[i].next, nodes[0].v[i].dist } ;
        nodes[0].v[i] = { 0, node, 1 };
        if(node->v[i].next)
            node->v[i].next->v[i].prev = node;
    }
    // Update the links that overshoot the added front node
    for(int i = h; nodes[0].v[i].next; i++)
        nodes[0].v[i].dist++;
    return dist;
}

int main()
{
    int n, k, x;
    scanf("%d %d", &n, &k);
    std::vector<int> input, output;
    nodes[0].v.resize(33);
    for(int i = 0; i < k; i++)
    {
        scanf("%d", &x);
        input.push_back(x);
    }
    // Read the input in reverse and output in reverse
    for(auto it = input.rbegin(); it < input.rend(); it++)
        output.push_back(pop(&nodes[*it]));
    // Print out the initial stack configuration
    for(Node* n = nodes[0].v[0].next; n; n = n->v[0].next)
        printf("%d ", n - nodes);
    // The numbers that were never mentioned in the input are added to the end of the initial stack
    for(int i = 1; i <= n; i++)
        if(nodes[i].v.empty())
            printf("%d ", i);
    for(auto it = output.rbegin(); it < output.rend(); it++)
        printf("\n%d", *it);
}