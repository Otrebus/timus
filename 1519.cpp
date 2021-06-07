/* 1519. Formula 1 - http://acm.timus.ru/problem.aspx?num=1519
 *
 * Strategy:
 * Dynamic programming. For any horizontal cross-section of track, we represent the endings leading
 * into that section from above by a number like 10012332, where the zeroes means there are no exits
 * from above, and each pair of equal numbers means those track endings are connected above this 
 * level. We maintain a mapping between each such representation to the number of different possible
 * tracks that give rise to that cross-section. For example, for areas of width two, there is only a
 * single track that gives rise to the cross section 11, so 11 is mapped to 1. For each level k of
 * the entire track, we can then generate cross-sections for level k+1 by dynamic programming. 
 * For example, if for height k, m number of tracks has the cross-section 10012332, these tracks
 * will provide m number of tracks that have cross-section 00002332 at height k+1 by connecting the
 * track segments denoted by 1. Numbers are always "normalized", so this latter is actually
 * represented by 00001221. Various other cross-sections can also be yielded from this, like
 * 00112332, 10010000, and even 12213443 by allowing part of the next level of the track peek up
 * and use the space at the zeroes. Each such cross-section that we can generate from 10012332 
 * will have m number of tracks leading into it, so we increase the key each of the derived ones by 
 * m. The actual representation is made more packed since there are at most 6 track components that
 * can go through any given cross-section. Also, to keep track of the topology of the track and in
 * order to be able to join different components, a disjoint-set data structure is maintained in
 * another variable. 
 *   By virtue of being very popular, this problem is extremely underrated and by far the hardest 
 * problem up until this difficulty. Owing to this, I had a couple of false starts which are 
 * provided below.
 *
 *
 * Performance:
 * Runs in 0.826s using 3316KB memory.
 */

#include <stdio.h>
#include <cctype>
#include <unordered_map>
#include <vector>
#include <cstring>

bool A[13][13], B[13][13];
char left[13][13];
int N, M;
int n;
unsigned long long upper;
unsigned long long upperSum;

#define SET(var, field, pos, val) ((~((field) << (pos)) & (var)) | ((val) << (pos)))
#define GET(var, field, pos) (((var) >> (pos)) & (field))

std::unordered_map<unsigned long long, unsigned long long> m[12];
auto s = m, t = m+1;

const int POS_SOURCE = 4;
const int POS_SOURCEINFO = 0;
const int SOURCE_COMPONENT = 3;
const int SOURCE_POSITION = 2;
const int POS_CONNECTIVITY = 6;
const int POS_MAXCOMPONENT = POS_CONNECTIVITY + 6*3;
const int POS_STACKHEIGHT = 0;
const int POS_STACK = 3;

inline unsigned int find(unsigned int& status, unsigned int n)
{
    auto rep = GET(status, 0x7, POS_CONNECTIVITY + 3*(n - 1));
    if(!rep || rep == n)
        return n;
    rep = find(status, rep);
    status = SET(status, 0x7, POS_CONNECTIVITY + 3*(n - 1), rep);
    return rep;
}

inline unsigned int join(unsigned int status, unsigned int x, unsigned int y)
{
    return SET(status, 0x7, POS_CONNECTIVITY + 3*(x - 1), y);
}

inline unsigned int getSourceType(unsigned int status)
{
    return GET(status, 0x3, POS_SOURCE);
}

inline unsigned int setSourceType(unsigned int status, unsigned int type)
{
    return SET(status, 0x3, POS_SOURCE, type);
}

inline unsigned int setSourceInfo(unsigned int status, unsigned int info)
{
    return SET(status, 0xF, POS_SOURCEINFO, info);
}

inline unsigned int getSourceInfo(unsigned int status)
{
    return GET(status, 0xF, POS_SOURCEINFO);
}

inline unsigned int getStackHeight(unsigned int stack)
{
    return GET(stack, 0x7, POS_STACKHEIGHT);
}

inline unsigned int setStackHeight(unsigned int stack, unsigned int height)
{
    return SET(stack, 0x7, POS_STACKHEIGHT, height);
}

inline unsigned int push(unsigned int stack, unsigned int pos, unsigned int component)
{
    return SET(stack, 0x7, pos*3 + POS_STACK, component);
}

inline unsigned int pop(unsigned int stack, unsigned int pos)
{
    return GET(stack, 0x7, pos*3 + POS_STACK);
}

inline unsigned int setMaxComponent(unsigned int status, unsigned int component)
{
    return SET(status, 0x7, POS_MAXCOMPONENT, component);
}

inline unsigned int getMaxComponent(unsigned int status)
{
    return GET(status, 0x7, POS_MAXCOMPONENT);
}

inline unsigned long long setComponent(unsigned long long status, char position, 
                                       unsigned long long component)
{
    return SET(status, 0x7LL, position*3, component);
}

inline long long getComponent(unsigned long long status, char position)
{
    return GET(status, 0x7LL, position*3);
}

inline char getsymbol()
{
    char ch;
    while(isspace(ch = getc(stdin)));
    return ch;
}

void dfsFirst(char j, int cur, int max, unsigned long long lower)
{
    if(j == M)
    {
        if(lower)
            (*s)[lower] = 1;
        return;
    }
    if(!A[n][j])
    {
        if(cur)
            return;
        else
        {
            dfsFirst(j+1, cur, max, lower);
            return;
        }
    }
    if(j < M-1)
        if(!cur)
            dfsFirst(j+1, max+1, max+1, setComponent(lower, j, max+1));
        else
            dfsFirst(j+1, cur, max, lower);
    if(cur)
        dfsFirst(j+1, 0, max, setComponent(lower, j, cur));
}

unsigned long long dfsLast(char j, unsigned int status, unsigned int stack, 
                           unsigned long long upper)
{
    long long sum = 0;
    bool incoming = getSourceType(status) != 0;
    if(j == M)
    {
        if(incoming)
            return 0;
        return (*t)[upper];
    }
    if(!A[n][j])
        return incoming ? 0 : dfsLast(j+1, status, stack, upper);
    auto stackHeight = getStackHeight(stack);
    if(stackHeight + 1 < M-j)
    {
        auto maxcomp = getMaxComponent(status);
        auto pushedStack = setStackHeight(stack, stackHeight+1);
        pushedStack = push(pushedStack, stackHeight, maxcomp+1);
        auto newStatus = setMaxComponent(setSourceInfo(status, maxcomp+1), maxcomp+1);
        if(incoming)
        {
            auto x = find(status, getSourceInfo(status));
            auto p = find(newStatus, x);
            auto q = find(newStatus, maxcomp+1);
            if(p == q && left[n][j])
                return sum;
            newStatus = join(newStatus, p, q);
            newStatus = setSourceInfo(newStatus, maxcomp+1);
        }
        sum += dfsLast(j+1, setSourceType(newStatus, incoming ? 0 : SOURCE_COMPONENT),
                       pushedStack, setComponent(upper, j, maxcomp+1));
    }
    if(incoming && stackHeight < M-j)
        sum += dfsLast(j+1, status, stack, upper);
    if(stackHeight > 0)
    {
        auto poppedStack = setStackHeight(stack, stackHeight-1);
        unsigned int newStatus = status;
        auto y = pop(stack, stackHeight-1);
        if(incoming)
        {
            auto x = find(status, getSourceInfo(status));
            auto p = find(newStatus, x);
            auto q = find(newStatus, y);
            if(p == q && left[n][j])
                return sum;
            newStatus = join(newStatus, p, q);
            newStatus = setSourceInfo(newStatus, y);
        }
        sum += dfsLast(j+1, setSourceType(newStatus, incoming ? 0 : SOURCE_COMPONENT), 
                       poppedStack, setComponent(upper, j, y));
    }
    return sum;
}

inline unsigned long long map(unsigned int status, unsigned long long state)
{
    int max = 0;
    char map[7] = { 0, 0, 0, 0, 0, 0, 0 };
    unsigned long long result = state;
    for(int i = 0; i < M; i++)
    {
        auto c = getComponent(state, i);
        if(c)
        {
            auto component = find(status, c);
            if(!map[component])
                map[component] = ++max;
            result = setComponent(result, i, map[component]);
        }
    }
    return result;
}

inline void dfs(char j, unsigned int status, unsigned long long lower)
{
    if(j == M)
    {
        lower = map(status, lower);
        (*s)[lower] += upperSum;
        return;
    }
    auto source = getSourceType(status);
    if(source)
    {
        if(!A[n][j])
            return;
        auto component = getComponent(upper, j);
        if(!component)
        {
            if(source == SOURCE_COMPONENT)
                dfs(j+1, setSourceType(status, 0), setComponent(lower, j, getSourceInfo(status)));
            else
            {
                auto maxComponent = getMaxComponent(status);
                auto newStatus = setMaxComponent(status, maxComponent-1);
                auto newLower = setComponent(lower, getSourceInfo(status), maxComponent-1);
                newLower = setComponent(newLower, j, maxComponent-1);
                dfs(j+1, setSourceType(newStatus, 0), newLower);
            }
            if(left[n][j])
                dfs(j+1, status, lower);
        }
        else
        {
            if(source == SOURCE_COMPONENT)
            {
                auto x = getSourceInfo(status);
                auto y = getComponent(upper, j);
                if(x == y)
                    return;
                unsigned int newStatus = status;
                auto p = find(newStatus, x);
                auto q = find(newStatus, y);
                if(p != q)
                {
                    newStatus = join(newStatus, p, q);
                    dfs(j+1, setSourceType(newStatus, 0), lower);
                }
                else
                    return;        
            }
            else
                dfs(j+1, setSourceType(status, 0), 
                    setComponent(lower, getSourceInfo(status), component));
        }
    }
    else // !source
    {
        auto component = getComponent(upper, j);
        if(!A[n][j])
        {
            if(component)
                return;
            dfs(j+1, status, lower);
            return;
        }
        if(!component && left[n][j])
            dfs(j+1, setSourceInfo(setSourceType(status, SOURCE_POSITION), j), lower);
        else if(component)
        {
            if(left[n][j])
                dfs(j+1, setSourceInfo(setSourceType(status, SOURCE_COMPONENT), component), lower);
            dfs(j+1, status, setComponent(lower, j, getComponent(upper, j)));
        }
    }
}

void fill(int i, int j)
{
    if(!B[i][j])
        return;
    B[i][j] = false;
    for(auto p : std::vector<std::pair<int, int>> { {i-1, j}, {i+1, j}, {i, j-1}, {i, j+1} })
        if(p.first >= 0 && p.first < N && p.second >= 0 && p.second < M)
            fill(p.first, p.second);
}

bool testConnectivity()
{
    std::memcpy(B, A, sizeof(A));
    bool hadcomp = false;
    for(int i = 0; i < N; i++)
        for(int j = 0; j < M; j++)
            if(B[i][j])
                if(hadcomp)
                    return false;
                else
                    fill(i, j), hadcomp = true;
    return true;
}

int main()
{
    std::memset(A, false, sizeof(A));
    int first = -1, last = 0;
    scanf("%d %d", &N, &M);
    for(int i = 0; i < N; i++)
    {
        for(int j = 0; j < M; j++)
        {
            A[i][j] = getsymbol() == '.';
            if(A[i][j])
            {
                first = first == -1 ? i : first;
                last = i;
            }
        }
    }

    if(!testConnectivity())
    {
        printf("0\n");
        return 0;
    }
    for(int i = 0; i < N; i++)
        for(int j = M-1; j >= 0; j--)
            left[i][j] = left[i][j+1] + (A[i][j+1] ? 1 : 0);
    n = first;
    dfsFirst(0, 0, 0, 0);
    std::swap(s, t);
    for(n = first+1; n < last; n++)
    {
        int sz = t->size();
        s->clear();
        s->reserve(sz*15);
        for(auto& l : *t)
        {
            upper = l.first;
            upperSum = (*t)[upper];
            dfs(0, setMaxComponent(0, 7), 0);
        }
        std::swap(s, t);
    }
    long long result = dfsLast(0, 0, 0, 0);
    printf("%lld\n", result);
}

// Slower solution, here we generate every conceivable configuration for an arbitrary level and then
// check the upper level if it is connectable to any earlier generated cross-section.

/*
#include <stdio.h>
#include <cctype>
#include <unordered_map>

#include <Windows.h>

bool A[13][13];
char left[13][13];
int N, M;
int n;

#define SET(var, field, pos, val) ((~((field) << (pos)) & (var)) | ((val) << (pos)))
#define GET(var, field, pos) (((var) >> (pos)) & (field))

std::unordered_map<unsigned long long, unsigned long long> m[2];
auto s = m, t = m+1;

const int POS_SOURCE = 4;
const int POS_SOURCEINFO = 0;
const int SOURCE_COMPONENT = 3;
const int SOURCE_POSITION = 2;
const int POS_CONNECTIVITY = 6;

const int POS_STACKHEIGHT = 0;
const int POS_MAXCOMPONENT = 3;
const int POS_STACK = 6;

inline unsigned int find(unsigned int& status, unsigned int n)
{
    auto rep = GET(status, 0x7, POS_CONNECTIVITY + 3*(n - 1));
    if(!rep || rep == n)
        return n;
    rep = find(status, rep);
    status = SET(status, 0x7, POS_CONNECTIVITY + 3*(n - 1), rep);
    return rep;
}

inline unsigned int join(unsigned int status, unsigned int x, unsigned int y)
{
    return SET(status, 0x7, POS_CONNECTIVITY + 3*(x - 1), y);
}

inline unsigned int getSourceType(unsigned int status)
{
    return GET(status, 0x3, POS_SOURCE);
}

inline unsigned int setSourceType(unsigned int status, unsigned int type)
{
    return SET(status, 0x3, POS_SOURCE, type);
}

inline unsigned int setSourceInfo(unsigned int status, unsigned int info)
{
    return SET(status, 0xF, POS_SOURCEINFO, info);
}

inline unsigned int getSourceInfo(unsigned int status)
{
    return GET(status, 0xF, POS_SOURCEINFO);
}

inline unsigned int getStackHeight(unsigned int stack)
{
    return GET(stack, 0x7, POS_STACKHEIGHT);
}

inline unsigned int setStackHeight(unsigned int stack, unsigned int height)
{
    return SET(stack, 0x7, POS_STACKHEIGHT, height);
}

inline unsigned int push(unsigned int stack, unsigned int pos, unsigned int component)
{
    return SET(stack, 0x7, pos*3 + POS_STACK, component);
}

inline unsigned int pop(unsigned int stack, unsigned int pos)
{
    return GET(stack, 0x7, pos*3 + POS_STACK);
}

inline unsigned int setMaxComponent(unsigned int stack, unsigned int component)
{
    return SET(stack, 0x7, POS_MAXCOMPONENT, component);
}

inline unsigned int getMaxComponent(unsigned int stack)
{
    return GET(stack, 0x7, POS_MAXCOMPONENT);
}

inline unsigned long long setComponent(unsigned long long status, char position, 
                                       unsigned long long component)
{
    return SET(status, 0x7LL, position*3, component);
}

inline long long getComponent(unsigned long long status, char position)
{
    return GET(status, 0x7LL, position*3);
}

inline char getsymbol()
{
    char ch;
    while(isspace(ch = getc(stdin)));
    return ch;
}

void dfsFirst(char j, int cur, int max, unsigned long long lower)
{
    if(j == M)
    {
        if(lower)
            (*s)[lower] = 1;
        return;
    }
    if(!A[n][j])
    {
        if(cur)
            return;
        else
        {
            dfsFirst(j+1, cur, max, lower);
            return;
        }
    }
    if(j < M-1)
    {
        if(!cur)
            dfsFirst(j+1, max+1, max+1, setComponent(lower, j, max+1)); // Start new
        else
            dfsFirst(j+1, cur, max, lower); // Continue current
    }
    if(cur)
        dfsFirst(j+1, 0, max, setComponent(lower, j, cur)); // End current
}

unsigned long long dfsLast(char j, unsigned int status, 
                           unsigned int stack, unsigned long long upper)
{
    long long sum = 0;
    bool incoming = getSourceType(status) != 0;
    if(j == M)
    {
        if(incoming)
            return 0;
        return upper ? (*t)[upper] : 0;
    }
    if(!A[n][j])
    {
        if(incoming)
            return 0;
        else
            return dfsLast(j+1, status, stack, upper);
    }

    auto stackHeight = getStackHeight(stack);
    if(stackHeight + 1 < M-j)
    {
        auto maxcomp = getMaxComponent(stack);
        auto pushedStack = setMaxComponent(setStackHeight(stack, stackHeight+1), maxcomp+1);
        pushedStack = push(pushedStack, stackHeight, maxcomp+1);
        auto newStatus = setSourceInfo(status, maxcomp+1);
        if(incoming)
        {
            auto x = find(status, getSourceInfo(status));
            auto p = find(newStatus, x);
            auto q = find(newStatus, maxcomp+1);
            if(p == q && left[n][j])
          return sum;
            newStatus = join(newStatus, p, q);
            newStatus = setSourceInfo(newStatus, maxcomp+1);
        }
        sum += dfsLast(j+1, setSourceType(newStatus, incoming ? 0 : SOURCE_COMPONENT), 
                                          pushedStack, setComponent(upper, j, maxcomp+1));
    }
    if(incoming && stackHeight < M-j)
        sum += dfsLast(j+1, status, stack, upper);
    if(stackHeight > 0)
    {
        auto poppedStack = setStackHeight(stack, stackHeight-1);
        unsigned int newStatus = status;
        auto y = pop(stack, stackHeight-1);
        if(incoming)
        {
            auto x = find(status, getSourceInfo(status));
            auto p = find(newStatus, x);
            auto q = find(newStatus, y);
            if(p == q && left[n][j])
                return sum;
            newStatus = join(newStatus, p, q);
            newStatus = setSourceInfo(newStatus, y);
        }
        sum += dfsLast(j+1, setSourceType(newStatus, incoming ? 0 : SOURCE_COMPONENT), 
                       poppedStack, setComponent(upper, j, y));
    }
    return sum;
}

inline void map(unsigned int status, unsigned long long& state)
{
    for(int i = 0; i < M; i++)
    {
        auto c = getComponent(state, i);
        if(c)
            state = setComponent(state, i, find(status, c));
    }
}

inline void rectify(unsigned long long& status)
{
    int max = 0;
    char map[7] = { 0, 0, 0, 0, 0, 0, 0 };
    for(int i = 0; i < M; i++)
    {
        auto component = getComponent(status, i);
        if(component)
        {
            if(!map[component])
                map[component] = ++max;
            status = setComponent(status, i, map[component]);
        }
    }
}

void dfs(char j, unsigned int status, unsigned int stack,
         unsigned long long upper, unsigned long long lower)
{
    auto source = getSourceType(status);
    if(j == M)
    {
        if(source)
            return;
        rectify(upper);
        map(status, lower);
        rectify(lower);
        auto b = (*t)[upper];
        if(b)
            (*s)[lower] += b;
        return;
    }
    else if(!A[n][j])
    {
        if(source || getComponent(upper, j))
            return;
        else
            return dfs(j+1, status, stack, upper, lower);
    }
    if(!source) // No incoming component; we have to create a new one
    {
        auto stackHeight = getStackHeight(stack);
        if(stackHeight + 1 < M-j)
        {
            auto maxcomp = getMaxComponent(stack);
            auto pushedStack = setMaxComponent(setStackHeight(stack, stackHeight+1), maxcomp+1);
            pushedStack = push(pushedStack, stackHeight, maxcomp+1);
            // New component, going top to bottom
            dfs(j+1, status, pushedStack, setComponent(upper, j, maxcomp+1), 
                setComponent(lower, j, maxcomp+1));
            // New component, going top to right
            if(stackHeight + 2 < M-j)
            {
                auto newSource = setSourceInfo(setSourceType(status, SOURCE_COMPONENT), maxcomp+1);
                dfs(j+1, newSource, pushedStack, setComponent(upper, j, maxcomp+1), lower);
            }
        }
        if(stackHeight > 0)
        {
            // Existing component, going top to bottom
            auto component = pop(stack, stackHeight-1);
            dfs(j+1, status, setStackHeight(stack, stackHeight-1), 
                setComponent(upper, j, component), setComponent(lower, j, component));

            // Existing component, going top to right
            if(stackHeight <= M-j)
            {
                auto newSource = setSourceInfo(setSourceType(status, SOURCE_COMPONENT), component);
                dfs(j+1, newSource, setStackHeight(stack, stackHeight-1), 
                    setComponent(upper, j, component), lower);
            }
        }
        // Unspecified component, going bottom to right
        if(stackHeight < M-j)
        {
            auto newSource = setSourceInfo(setSourceType(status, SOURCE_POSITION), j);
            dfs(j+1, newSource, stack, upper, lower);
        }
    }
    else // Incoming component from the left
    {
        auto stackHeight = getStackHeight(stack);
            
        if(stackHeight < M-j)
        {
            dfs(j+1, status, stack, upper, lower);
            if(getSourceType(status) == SOURCE_COMPONENT )
                dfs(j+1, setSourceType(status, 0), stack, upper, 
                    setComponent(lower, j, getSourceInfo(status)));
            else
            {
                auto maxcomp = getMaxComponent(stack);
                auto newStack = setMaxComponent(stack, maxcomp+1);
                // From down, left, to down
                auto newLower = setComponent(lower, getSourceInfo(status), maxcomp + 1);
                dfs(j+1, setSourceType(status, 0), newStack, 
                    upper, setComponent(newLower, j, maxcomp+1));  
            }
            if(stackHeight + 1 < M-j) // Make new component
            {
                auto maxcomp = getMaxComponent(stack);
                auto pushedStack = setMaxComponent(setStackHeight(stack, stackHeight+1), maxcomp+1);
                pushedStack = push(pushedStack, stackHeight, maxcomp+1);
                unsigned int newStatus = status;
                if(getSourceType(status) == SOURCE_COMPONENT)
                {
                    auto x = find(newStatus, getSourceInfo(status));
                    newStatus = join(newStatus, x, maxcomp+1);
                    // New component, from up, left to up
                    dfs(j+1, setSourceType(newStatus, 0), pushedStack, 
                        setComponent(upper, j, maxcomp+1), lower);
                }
                else
                    // New component, from down, left to up
                    dfs(j+1, setSourceType(newStatus, 0), pushedStack, 
                        setComponent(upper, j, maxcomp+1), 
                        setComponent(lower, getSourceInfo(status), maxcomp+1));
            }
        }
        if(stackHeight > 0)
        {
            if(getSourceType(status) == SOURCE_COMPONENT)
            {   
                auto x = find(status, getSourceInfo(status));
                auto y = pop(stack, stackHeight-1);
                unsigned int newStatus = status;
                auto p = find(newStatus, x);
                auto q = find(newStatus, y);
                if(p != q)
                {
                    newStatus = join(newStatus, p, q);
                    // Existing component, from up, left to up
                    dfs(j+1, setSourceType(newStatus, 0), setStackHeight(stack, stackHeight-1),
                        setComponent(upper, j, y), lower);
                }
            }
            else
            {
                auto component = pop(stack, stackHeight-1);
                auto newStack = setStackHeight(stack, stackHeight-1);
                // Existing component, from down, left, to up
                dfs(j+1, setSourceType(status, 0), newStack, setComponent(upper, j, component), 
                    setComponent(lower, getSourceInfo(status), component));
            }
        }
    }
}

int main()
{
    std::memset(A, false, sizeof(A));
    scanf("%d %d", &N, &M);
    for(int i = 0; i < N; i++)
        for(int j = 0; j < M; j++)
            A[i][j] = getsymbol() == '.';
    for(int i = 0; i < N; i++)
        for(int j = M-1; j >= 0; j--)
            left[i][j] = left[i][j+1] + (A[i][j+1] ? 1 : 0);
    dfsFirst(0, 0, 0, 0);
    std::swap(s, t);
    for(n = 1; n < N-1; n++)
    {
        s->clear();
        dfs(0, 0, 0, 0, 0);
        std::swap(s, t);
    }
    long long result = dfsLast(0, 0, 0, 0);
    printf("%lld\n", result);
}
*/

// Below is yet another (but incorrect) solution that does not consider topology, so this one
// actually generates the number of ways to fill up the given area with an arbitrary number of
// closed tracks.

/*
#include <stdio.h>
#include <cctype>
#include <unordered_map>

bool A[13][13];
int N, M;
int n;

std::unordered_map<int, unsigned long long> m[2];
auto s = m, t = m+1;

inline char getsymbol()
{
    char ch;
    while(isspace(ch = getc(stdin)));
    return ch;
}

const char NONE = 0, UP = 1, DOWN = 2, CROSSED = 4;

inline int setInfo(int info, char i, char status)
{
    return info |= status << (2*i);
}

void dfsFirst(char i, char status, int lower)
{
    if(i == M)
    {
        if(status & UP)
            (*s)[lower] = 1;
    }
    else if(!A[0][i])
        dfsFirst(i+1, status, lower);
    else
    {
        if(status & DOWN)
            dfsFirst(i+1, 0, setInfo(lower, i, DOWN));
        else
        {
            dfsFirst(i+1, UP, setInfo(lower, i, UP));
            if(A[0][i+1])
                dfsFirst(i+1, 0, lower);
        }
    }
}

unsigned long long dfsLast(char i, char status, int upper)
{
    if(i == M)
    {
        if(status & UP)
        {
            return (*s)[upper];
        }
        else
            return 0;
    }
    else if(!A[N-1][i])
        return dfsLast(i+1, status, upper);
    else
    {
        if(status & UP)
            return dfsLast(i+1, 0, setInfo(upper, i, DOWN));
        else
        {
            unsigned long long sum = dfsLast(i+1, UP, setInfo(upper, i, UP));
            if(A[N-1][i+1])
                sum += dfsLast(i+1, 0, upper);
            return sum;
        }
    }
}

void dfs(char i, char status, int upper, int lower)
{
    if(i == M)
    {
        if(status == (UP | CROSSED))
        {
            (*s)[lower] += (*t)[upper];
        }
    }
    else if(!A[n][i])
        dfs(i+1, status, upper, lower);
    else if(status & UP)
    {
        if(status & DOWN)
        {
            dfs(i+1, DOWN | (status & CROSSED), setInfo(upper, i, DOWN), lower);
            dfs(i+1, UP | (status & CROSSED), upper, setInfo(lower, i, UP));
            if(A[n][i+1])
                dfs(i+1, status, upper, lower);
        }
        else
        {
            dfs(i+1, DOWN | CROSSED, setInfo(upper, i, DOWN), setInfo(lower, i, DOWN));
            dfs(i+1, status & CROSSED, setInfo(upper, i, DOWN), lower);
            dfs(i+1, DOWN | UP | (status & CROSSED), upper, setInfo(lower, i, DOWN));
        }
    }
    else
    {
        if(status & DOWN)
        {
            dfs(i+1, UP | CROSSED, setInfo(upper, i, UP), setInfo(lower, i, UP));
            dfs(i+1, DOWN | UP | CROSSED, setInfo(upper, i, UP), lower);
            dfs(i+1, CROSSED, upper, setInfo(lower, i, UP));
        }
        else
        {
            dfs(i+1, UP | (status & CROSSED), setInfo(upper, i, UP), lower);
            dfs(i+1, DOWN | (status & CROSSED), upper, setInfo(lower, i, DOWN));
            if(A[n][i+1])
                dfs(i+1, status, upper, lower);
        }
    }
}

int main()
{
    std::memset(A, false, sizeof(A));
    scanf("%d %d", &N, &M);
    for(int i = 0; i < N; i++)
        for(int j = 0; j < M; j++)
            A[i][j] = getsymbol() == '.';
    dfsFirst(0, DOWN, 0);
    for(n = 1; n < N-1; n++)
    {
        t->clear();
        std::swap(s, t);
        dfs(0, UP, 0, 0);
    }
    long long result = dfsLast(0, UP, 0);
    printf("%lld\n", result);
}
*/