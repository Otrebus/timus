/* 1019. Line Painting - http://acm.timus.ru/problem.aspx?num=1019
 *
 * Strategy:
 * Go through the carefully sorted input endpoints from left to right, while maintaining a priority 
 * queue that keeps track of the most recently painted line, and assemble the solution from this
 * information.
 *
 * Performance:
 * O(N log N), runs in 0.156s using 936KB memory.
 */

#include <stdio.h>
#include <set>
#include <vector>
#include <algorithm>

struct event
{
    int n; // The order of this segment in the sequence
    bool begin; // Starting point or ending point?
    int pos;
    bool color;
};

struct Comp
{
    bool operator() (const event& a, const event& b)
    {
        return a.n > b.n;
    }
};

int main()
{
    std::set<event, Comp> prio;
    std::vector<event> events;
    int N;
    scanf("%d", &N);
    events.push_back( { 0, true, 0, true } );
    events.push_back( { 0, false, 1000000000, true } );
    for(int i = 1; i <= N; i++)
    {
        int x, y;
        char c;
        scanf("%d %d %c", &x, &y, &c);
        events.push_back( { i, true, x, c == 'w' } );
        events.push_back( { i, false, y, c == 'w' } );
    }

    // We sort the input by position first, and then order in a way depending on if the point starts
    // or ends a line segment
    auto sortfun = [] (const event& a, const event& b)
    { 
        if(a.pos == b.pos)
        {
            if(!a.begin && !b.begin)
                return a.n < b.n;
            else if(a.begin && b.begin)
                return a.n > b.n;
            else if(a.begin && !b.begin)
                return true;
            else
                return false;
        }
        else
            return a.pos < b.pos;
        return a.pos == b.pos ? (!a.begin && !b.begin ? a.n < b.n : a.n > b.n ) : a.pos < b.pos; 
    };
    std::sort(events.begin(), events.end(), sortfun);
    std::vector<std::pair<int, bool>> output;
    int curn = -1;
    bool curcolor = false;

    // Go through the ordered input and maintain a priority queue that keeps track of the topmost
    // segment. Whenever we switch colors we take note of that
    for(auto& e : events)
    {
        if(e.begin)
        {
            if(e.n > curn)
            {
                curn = e.n;
                if(e.color != curcolor)
                {
                    output.push_back( { e.pos, e.color } );
                    curcolor = e.color;
                }
            }
            prio.insert(e);
        }
        else
        {
            prio.erase(e);
            if(curn == e.n)
            {
                if(!prio.empty())
                {
                    auto newe = *prio.begin();
                    if(newe.color != curcolor)
                    {
                        output.push_back( { e.pos, newe.color } );
                        curcolor = newe.color;
                    }
                    curn = newe.n;
                }
                else
                    output.push_back( { 1000000000, true } );
            }
        }
    }

    int run = 0;
    int maxrun = -1;
    int maxpos;
    int lastpos = 0;
    curcolor = false;
    // Finally, figure out the longest segment
    for(auto& e : output)
    {
        run += e.first - lastpos;
        if(run > maxrun && curcolor)
        {
            maxrun = run;
            maxpos = e.first;
        }
        if(curcolor != e.second)
        {
            run = 0;
        }
        lastpos = e.first;
        curcolor = e.second;
    }
    printf("%d %d\n", maxpos-maxrun, maxpos);
}