/* 1439. Battle with You-Know-Who - http://acm.timus.ru/problem.aspx?num=1439
 *
 * Strategy:
 * Use an augmented logarithmic lookup/insert data structure, that keeps track of the amount of
 * elements in each "subdivision", to represent the deleted doors. In this case, an augmented
 * skip list is used.
 *
 * Performance:
 * O(M log M), runs the test cases in 0.405s using 944KB memory.
 */

#include <vector>
#include <random>
#include <stack>
#include <algorithm>

class SkipList
{
    struct Node;
    struct Link { Node* next; int skip; };
    struct Node { std::vector<Link> links; int val; } nodes[10001];

    int size;
    std::random_device rd;
    std::mt19937 mt;
    std::uniform_int_distribution<unsigned int> rnd;

    int randomHeight()
    {
        int l = 0;
        for(auto a = rnd(mt); a & 1; a >>= 1)
            l++;
        return l;
    }

public:
    SkipList()
    {
        size = 1;
        mt = std::mt19937(rd());
        rnd = std::uniform_int_distribution<unsigned int>(0, 0xFFFF);
    }

    void insert(int val) // Inserts a deleted door
    {
        struct StackEntry { Link* link; int pos; };
        std::stack<StackEntry> s;

        Node* curNode = &nodes[0], *newNode = &nodes[size++];
        int addH = randomHeight(), pos = 0;
        int off = 1 + addH - (int)curNode->links.size();
        curNode->links.reserve(std::max(curNode->links.size(), curNode->links.size() + off));
        newNode->val = val;

        while(off-- > 0)
            curNode->links.push_back( { 0, 0 } );
        int curH = nodes[0].links.size()-1;
        while(curH >= 0)
        {
            auto nextNode = curNode->links[curH].next;
            if(!nextNode || nextNode->val > val)
            {
                if(addH >= curH)
                    s.push( { &curNode->links[curH], pos } );
                else
                    curNode->links[curH].skip++;
                curH--;
            }
            else
            {
                pos += curNode->links[curH].skip;
                curNode = nextNode;
            }
        }

        while(!s.empty())
        {
            auto entry = s.top();
            s.pop();
            newNode->links.push_back( { entry.link->next, entry.link->skip + entry.pos - pos } );
            entry.link->next = newNode;
            entry.link->skip = 1 + pos - entry.pos;
        }
    }

    int search(int val) // Finds the original number of the nth door
    {
        Node* curNode = &nodes[0];
        int curH = curNode->links.size()-1;
        int pos = 0;

        while(curH >= 0)
        {
            auto nextNode = curNode->links[curH].next;
            if(pos + val + curNode->links[curH].skip <= 
              (nextNode ? nextNode->val : std::numeric_limits<int>::max()))
                curH--;                
            else
            {
                pos += curNode->links[curH].skip;
                curNode = nextNode;
            }
        }
        return val + pos;
    }
};

SkipList s;

int main()
{
    int N, M;
    scanf("%d %d", &N, &M);
    while(M--)
    {
        char str[2];
        scanf("%s", str);
        if(str[0] == 'L')
        {
            int w;
            scanf("%d", &w);
            printf("%d\n", s.search(w));
        }
        else
        {
            int w;
            scanf("%d", &w);
            s.insert(s.search(w));
        }
    }
}