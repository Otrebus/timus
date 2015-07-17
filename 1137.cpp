/* 1137. Bus Routes - http://acm.timus.ru/problem.aspx?num=1137
 *
 * Strategy:
 * Maintain a linked list, and have each node point to the element in the linked list that we can
 * add another loop to. Do a depth-first search through the graph and add loops wherever we can. 
 * This solution and its code is a horrid abomination.
 *
 * Performance:
 * O(mn), which is optimal since the input is of this size. I can't test the performance or the
 * validity of this algorithm since Timus is down.
 */

#include <stdio.h>
#include <vector>
#include <unordered_set>

struct node;

struct vertex
{
	std::vector<std::pair<vertex*, bool>> out;
	int n;
	node* node;

	vertex() : node(0), n(0)
	{
	}
};

struct node
{
	node* next, *prev;
	vertex* v;

	node(vertex* v, node* prev, node* next) : next(next), v(v), prev(prev) {}
};

void makeLoop(vertex* u, std::pair<vertex*, bool>* p, std::vector<node>& nodes)
{   // Add a loop from u in the direction of p
	node* original = u->node;
	if(original) // We save the original outgoing to append it last, and create a new node to loop from
	{
		nodes.push_back(node(u, original->prev, 0));
		u->node = &nodes.back();
		original->prev->next = &nodes.back();
		nodes.back().prev = original->prev;
	}
	else // Create a new node in the linked list since we had no one to start out with
	{
		nodes.push_back(node(u, 0, 0));
		u->node = &nodes.back();
		nodes.back().v = u;
	}
	vertex* v = u;
	vertex* w;

	do  // Loop through the graph and build a linked list
	{
		w = p->first;
		p->second = true;
		nodes.push_back(node(w, v->node, 0));
		w->node = &nodes.back();
		v->node->next = w->node;

		for(auto it = w->out.begin(); it < w->out.end(); it++)
		{
			if(!it->second)
			{
				it->second = true;
				p = &(*it);
				break;
			}
		}
		v = w;
	} while(p->first != u);

	if(original) // Attach our built linked list back to the origin of this loop
	{
		v->node->next = original;
		original->prev = v->node;
	}
	else
	{
		v->node->next = u->node;
		u->node->prev = v->node;
	}
}

int main()
{
	int n; 
	std::vector<vertex> v(10001);   // Graph nodes (I call them vertices for some reason)
	std::vector<vertex*> s(100001); // Stack for the depth-first search
	std::vector<node> nodes;        // The nodes of the linked list
	nodes.reserve(100001);          // We do push_back and have pointers to inside the vector
	                                // so make sure we don't resize it and invalidate all pointers
	scanf("%d", &n);
	int m, last, tot = 0;

	for(int i = 0; i < n; i++)
	{
		scanf("%d", &m);
		scanf("%d", &last);
		for(int j = 1; j <= m; j++)
		{
			int u;
			scanf("%d", &u);
			tot++;
			if(!v[u].n)
				v[u].n = u;
			v[last].out.push_back(std::make_pair(&v[u], false));
			last = u;
		}
	}

	int top = 0;
	s[top++] = &v[last];
	while(top != 0) // Do depth-first search and spam loops wherever we can
	{
		vertex* u = s[--top];
		for(auto it = u->out.begin(); it < u->out.end(); it++)
		{
			if(!it->second)
			{
				s[top++] = it->first;
				makeLoop(u, &(*it), nodes);
				it->second = true;
			}
		}

	}
	printf("%d ", tot);
	auto node = &nodes.front();
	while(tot--)
	{
		printf("%d ", node->v->n);
		node = node->next;
	}
	printf("%d ", nodes.front().v->n);
	std::system("pause");
	return 0;
}