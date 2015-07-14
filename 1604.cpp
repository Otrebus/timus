/* 1604. Country of Fools - http://acm.timus.ru/problem.aspx?num=1604
 *
 * Strategy:
 * Keep the speed limits in a priority queue and remove the most common one that we did not
 * remove previously.
 *
 * Performance:
 * O(n log k), accepted at 0.031s and using 380KB.
 */

#include <queue>
#include <cstdio>
#include <list>

// To avoid having to reimplement a priority queue just to avoid having the underlying container
// resize - from http://stackoverflow.com/questions/3666387
// /c-priority-queue-underlying-vector-container-capacity-resize
template <class T, class L>
class reservable_priority_queue : public std::priority_queue<T, std::vector<T>, L>
{
public:
    typedef typename std::priority_queue<T, std::vector<T>, L>::size_type size_type;
    void reserve(size_type capacity)
	{
		this->c.reserve(capacity);
	}
};

class Compare
{
public:
	bool operator() (const std::pair<int, int>& a, const std::pair<int, int>& b)
	{
		return (a.first == b.first) ? (a.second > b.second) : a.first < b.first;
	}
};

int main()
{
	int n;
    reservable_priority_queue<std::pair<int, int>, Compare> q;
	scanf("%d", &n);
	q.reserve(n);
	for(int i = 1; i <= n; i++)
	{
		int x;
		scanf("%d", &x);
		q.push(std::make_pair(x, i));
	}
	while(!q.empty())
	{
		auto p1 = q.top(); // Greedily pick the most common sign
		q.pop();
		printf("%d ", p1.second);
		if(!q.empty()) // Make sure we choose another sign next
		{
			auto p2 = q.top();
			q.pop();
			printf("%d ", p2.second);
			if(p2.first > 1)
				q.push(std::make_pair(p2.first-1, p2.second));
		}
		if(p1.first > 1)
			q.push(std::make_pair(p1.first-1, p1.second));
	}
	return 0;
}
