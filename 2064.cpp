/* 2064. Caterpillars - http://acm.timus.ru/problem.aspx?num=2064
 *
 * Strategy:
 * Treat the caterpillars' positions as triangle waves, and build and maintain a queue that keeps
 * track of the order of the maximum waves (the envelope), and also maintain a priority queue that
 * tracks the following events, ordered from t = 0 and up and by decreasing sizes of waves:
 * 
 *  1) Intersections between waves and the farthest wave in the envelope
 *  2) Minimum points
 *
 * When an intersection is reached, we update the envelope and calculate the earliest next time the
 * current waveform reaches a minimum that could intersect the envelope wave again. When a minimum
 * point is reached, we check if an intersection with the envelope is within the rise of this
 * wave. If not, we recalculate the next minimum point for the wave.
 * 
 * Performance:
 * The complexity is not entirely easy to reason about but if the maximum time width is T
 * (up to 1e6), it feels like O(Tlog^2 T) since we have up to T + T/2 + T/3 + ... = O(T log T)
 * minimum locations and much fewer intersection points, that we apply the O(log T) data structure
 * on. The program finishes executing in 1.872s using 49,112KB memory.
 */

#include <queue>
#include <unordered_set>
#include <stdio.h>
#include <algorithm>

struct entry // Entry in the priority queue
{
    int pos;
    int size;
    bool operator () (entry& e1, entry& e2)
    {
        return e1.pos == e2.pos ? e1.size < e2.size : e1.pos > e2.pos;
    }
};

// Checks if wave a starting at point p intersects wave b within this period of b
int intersect(int p, int a, int b)
{
    int b1 = p - p%(2*b);
    if(b1 == p) // Collinear, doesn't count
        return -1;
    int x = (b1 + 2*b + p)/2;
    int y = x - p;
    return y < a ? x : -1;
}

// Calculates the last minimum point of wave a before the next minimum of wave b after point p
int nextStart(int p, int a, int b)
{
    return ((1 + (p+1)/(2*b))*2*b)/(2*a)*2*a;
}

// Evaluates the value of wave w at position x
int eval(int x, int w)
{
    return -std::abs(x % (2*w) - w) + w;
}

int main()
{
    std::queue<int> envelope; // The order of the maximum waves
    std::priority_queue<entry, std::vector<entry>, entry> entries;
    std::unordered_set<int> waves; // Each wave described by its amplitude
    std::vector<int> queries;
    std::vector<std::pair<int, int>> points; // The points where we get a new leader

    int n, q, x, maxq = 0;
    scanf("%d", &n);
    while(n--)
    {
        scanf("%d", &x);
        waves.insert(x); // We use an unordered_set here to remove duplicates
    }
    for(auto w : waves)
        entries.push( { 0, w } );
    scanf("%d", &q);
    for(int i = 0; i < q; i++)
    {
        scanf("%d", &x);
        queries.push_back( x );
        maxq = std::max(maxq, x);
    }
    std::vector<int> vals(maxq+1);
    envelope.push(entries.top().size);
    points.push_back( { 0, entries.top().size } );

    while(true)
    {
        auto e = entries.top();
        entries.pop();
        if(e.pos > maxq)
            break;
        if(!e.size) // Intersection point
        {
            envelope.pop(); // We're now past the front wave of the envelope
            points.push_back( { e.pos, envelope.front() } );
        }
        else // Minimum point
        {
            int isec = intersect(e.pos, e.size, envelope.back());
            if(isec != -1)
            {
                entries.push( { isec, 0 } );
                envelope.push(e.size); // This is the next maximum wave in the envelope
            }
            int ns = nextStart(e.pos, e.size, envelope.back());
            entries.push( { ns, e.size } );
        }
    }
    points.push_back( { maxq+1, 1 } );
    int cw = 0;
    for(int x = 0; x <= maxq; x++)
    {
        while(x > points[cw+1].first)
            cw++;
        vals[x] = eval(x, points[cw].second);
    }
    for(int i = 0; i < q; i++)
        printf("%d\n", vals[queries[i]]);
}