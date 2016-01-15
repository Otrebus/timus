/* 2009. Canteen Line - http://acm.timus.ru/problem.aspx?num=2009
 *
 * Strategy:
 * Straightforward simulation using linked lists to represent the lines.
 * 
 * Performance:
 * O(n^2), runs the test cases in 0.015s using 1292KB memory (rated 3/197).
 */

#include <stdio.h>
#include <vector>
#include <algorithm>

struct node // Represents a student
{
    int i; // Id of student
    node* next; // Student behind this
} nodes[1001];

node head1, head2; // Sentinel heads of the lines

struct group // Represents a group of students arriving
{
    int t; // Timestamp
    std::vector<int> s; // Ids of students in the group
} grp[1001];

bool b[1001][1001]; // b[i][j] means student i can get behind student j
std::pair<int, bool> ans[1001]; // Ans[i] = { t, true } means student i arrived at time t
                                // in the right line

// Assigns a group of students into their optimal positions in the lines
void assign(group* g)
{
    for(int i = 0; i < g->s.size(); i++) // For each student in the group
    {
        int id = g->s[i], pos = 0;
        struct result { int pos; node* head; } res1, res2; // Best positions found
        // Iterate through the line to find the best place we can stand
        for(node* n = &head1; n; n = n->next)
        {
            res1 = { pos++, n };
            if(b[id][n->i]) // We can stand behind this student so we break
                break;
        }
        // Same as above but for the other line
        pos = 0;
        for(node* n = &head2; n; n = n->next)
        {
            res2 = { pos++, n };
            if(b[id][n->i])
                break;
        }
        // Splice us into the best line
        if(res1.pos <= res2.pos)
        {
            auto tmp = res1.head->next;
            res1.head->next = nodes+id;
            nodes[id].next = tmp;
        }
        else
        {
            auto tmp = res2.head->next;
            res2.head->next = nodes+id;
            nodes[id].next = tmp;
        }
    }
}

int main()
{
    int n, m;
    scanf("%d %d", &n, &m);
    for(int i = 1; i <= n; i++)
    {
        nodes[i].i = i;
        int j = 1;
        while(j)
        {
            scanf("%d", &j);
            if(j)
                b[i][j] = true;
        }
    }
    std::vector<group*> groups;
    for(int i = 0; i < m; i++)
    {
        int t, n;
        scanf("%d %d", &t, &n);
        grp[i].t = t;
        while(n--)
        {
            scanf("%d", &t);
            grp[i].s.push_back(t);
        }
        groups.push_back(grp+i);
    }
    // Sorts the groups by time, and if equal time, by input order (in reverse for easy popping)
    std::sort(groups.begin(), groups.end(), [] (group* a, group* b) 
                                            { return a->t == b->t ? a > b : a->t > b->t; } );
    int t = 1;
    while(head1.next || head2.next || !groups.empty()) // Simulation loop
    {
        // If students are in line, let the first one leave and add to solution
        if(head1.next)
        {
            ans[head1.next->i] = { t, true };
            head1.next = head1.next->next;
        }
        if(head2.next)
        {
            ans[head2.next->i] = { t, false };
            head2.next = head2.next->next;
        }
        // We have a group whose time stamp is equal to the current time
        while(!groups.empty() && groups.back()->t == t)
        {
            group* g = groups.back();
            groups.pop_back();
            assign(g);
        }
        // Lines empty, fast forward to the timestamp of the next group
        if(!head1.next && !head2.next && !groups.empty())
            t = groups.back()->t;
        else
            t++; // Lines not empty, increment timestamp so we can process the next students in line
    }
    for(int i = 1; i <= n; i++)
        printf("%d %s\n", ans[i].first, ans[i].second ? "right" : "left");
}