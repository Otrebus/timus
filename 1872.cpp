/* 1872. Spacious Office - http://acm.timus.ru/problem.aspx?num=1872
 *
 * Strategy:
 * First, sort the room sizes. For each room size in the ordered list, take the unassigned team with
 * the smallest upper size requirement to be assigned to that room. This is done with a
 * combination of binary search and an RMQ structure: sort the teams by their lower requirement and
 * do binary search to find the range of teams that fit, and then do a RMQ query on this range over
 * their rightmost segments to find the largest team that fits.
 *
 * To make sure the given assignment can't be permuted to another valid assignment, we go through
 * the ordered room/assigned team list, and for each such entry i, check that there is no entry j
 * down the list whose lower requirement satisfies the requirement of room i.
 *
 * Performance:
 * O(nlog n), runs the tests in 0.015s using 336KB memory.
 */

#include <vector>
#include <algorithm>
#include <iostream>

const int inf = std::numeric_limits<int>::max();

class Rmq // Range minimum query data structure
{
    std::vector<std::pair<int, int>> A;
    int size;

    std::pair<int, int> query(int l, int r, int i, int left, int right)
    {
        if(l <= left && r >= right)
            return A[i];
        int newl = (left+right)/2, newr = newl + 1;
        std::pair<int, int> minleft = { inf, 0 }, minright = minleft;
        if(l <= newl)
            minleft = query(l, r, i*2, left, newl);
        if(r >= newr)
            minright = query(l, r, i*2+1, newr, right);
        return std::min(minleft, minright);
    }

public:
    Rmq(int s = 1) :size(s)
    {
        int vSize = 2;
        for(--s; s; s >>= 1)
            vSize *= 2;
        A = std::vector<std::pair<int, int>>(vSize, { inf, 0 });
    }

    std::pair<int, int> query(int l, int r)
    {
        return query(l, r, 1, 1, A.size()/2);
    }

    void set(int i, std::pair<int, int> val)
    {
        i = A.size()/2+i-1;
        A[i] = val;
        while(i >>= 1)
            A[i] = std::min(A[i*2], A[i*2+1]);
    }
};

struct Team
{ 
    int i, l, r; // Input position, left and right bounds
    bool operator < (const Team& b) const { return l < b.l; } 
};

int main()
{
    std::ios::sync_with_stdio(false);
    int N;
    std::cin >> N;
    std::vector<std::pair<int, int>> v(N+1); // The rooms ordered by size
    for(int i = 1; i <= N; i++) 
    {
        int x;
        std::cin >> x;
        v[i] = { i, x };
    }
    Rmq right(N); // Rightmost endpoints of teams, min-queryable
    Rmq left(N);
    std::vector<Team> p(N+1); // The teams in their original given order
    std::vector<int> ans(N+1); // Output
    std::vector<std::pair<int, int>> w(N+1); // The assigned teams in the same order as v above

    for(int i = 1; i <= N; i++)
    {
        int x, y;
        std::cin >> x >> y;
        p[i] = { i, x, y };
    }
    std::vector<Team> s = p; // The teams sorted by leftmost endpoint
    std::sort(s.begin(), s.end());
    for(int i = 1; i <= N; i++)
        right.set(i, { s[i].r, i });
    auto pRev = [] (std::pair<int, int> a, std::pair<int, int> b) { return a.second < b.second; };
    std::sort(v.begin(), v.end(), pRev );

    for(int i = 1; i <= N; i++) // For each room
    {
        // Find the last team whose left bound satisfies this room
        auto it = std::upper_bound(s.begin()+1, s.end(), Team { 0, v[i].second, 0 });
        it = it > s.begin()+1 ? it-1 : s.end();
        // Query that team and all previous teams for the team with the leftmost right boundary ..
        auto l = (it == s.end()) ? std::make_pair(inf, 0) : right.query(1, it-s.begin());
        // And see if that team fits the requirement of the room; if not, no other team will
        if(l.first < v[i].second || l.first == inf)
        {
            std::cout << "Let's search for another office.";
            return 0;
        }

        w[i] = { p[s[l.second].i].l, p[s[l.second].i].r };
        ans[p[s[l.second].i].i] = v[i].first;
        right.set(l.second, { inf, 0 }); // Setting this endpoint to infinity effectively erases it
        left.set(i, { w[i].first, i }); 
    }

    // Check for alternate assignments
    for(int i = 1; i <= N; i++)
    {
        // Check if we can switch assignments of this team and some team further down the ordered
        // list
        auto it = std::upper_bound(v.begin()+i+1, v.end(), w[i], pRev );
        it = it > v.begin()+1 ? it-1 : v.end();
        auto l = it == v.end() ? std::make_pair(inf, 0) : left.query(i+1, it-v.begin());
        if(l.first <= v[i].second)
        {
            std::cout << "Ask Shiftman for help.";
            return 0;
        }
    }

    std::cout << "Perfect!\n";
    for(int i = 1; i <= N; i++)
        std::cout << ans[i] << " ";
}