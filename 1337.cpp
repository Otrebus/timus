/* 1337. Bureaucracy - http://acm.timus.ru/problem.aspx?num=1337
 *
 * Strategy:
 * Straightforward; for each day, acquire the corresponding document if it has no predecessors.
 *
 * Performance:
 * O(N), runs the tests in 0.031s using 372KB memory.
 */

#include <iostream>
#include <unordered_set>
#include <vector>

std::unordered_set<int> P[101]; // Required documents for document [i]
std::unordered_set<int> d, t; // All documents, required documents
std::vector<int> ans; // Output 
std::vector<int> ng; // Dummy
int W[101]; // Document available at day [i]

int N, L, x, w, i, p;

void remove(int x) { // Removes this document from everywhere, including all requirements
    P[x].clear();
    for(int i = 1; i <= N; i++)
        P[i].erase(x);
    d.erase(x);
    t.erase(x);
}

int main() {
    std::cin >> N >> L;
    for(int i = 1; i <= N; i++) {
        int x;
        std::cin >> x;
        W[x] = i;
        d.insert(i);
    }

    for(int i = 1; i <= N; i++)
        for(std::cin >> x; x; std::cin >> x)
            P[i].insert(x);

    std::cin >> w;

    for(std::cin >> x; x; std::cin >> x)
        ng.push_back(x); // Save until later

    for(std::cin >> x; x; std::cin >> x)
        t.insert(x); // Add to required

    for(auto g : ng)
        remove(g); // Remove the documents we have from everything

    for(p = i = w; !t.empty(); !t.empty() && i++) {
        int j = 1 + (i-1)%L;

        if(i - p > L) // We went more than a period without being able to get doc, abort
            return std::cout << "No Solution", 0;

        if(d.find(W[j]) != d.end() && P[W[j]].empty()) { // No prereq and we don't have doc
            remove(W[j]);
            ans.push_back(W[j]);
            p = i;
        }
    }
    std::cout << i-w << std::endl;
    for(auto a : ans)
        std::cout << a << " ";
}
