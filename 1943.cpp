/* 1943. Space Rummy - http://acm.timus.ru/problem.aspx?num=1943
 *
 * Strategy:
 * Randomized brute-force. Winning decks always seem to exist for N > 4, with between 20% and 50%
 * or so of all possible permutations being winning decks. Thus, for N <= 4 we simply simulate games
 * for all possible permutations and for N > 4 we just shuffle and simulate until we win.
 *
 * Performance:
 * O(N), runs in 0.031s using 308KB memory.
 */

#include <vector>
#include <algorithm>
#include <iostream>
#include <queue>
#include <random>

// Simulates a game where we have deck A and the opponent deck B, returns true if we win
bool simulate(std::vector<int>& A, std::vector<int>& B)
{
    std::deque<int> a(A.begin(), A.end());
    std::deque<int> b(B.begin(), B.end());
    int nloops = 0;
    while(!b.empty() && !a.empty())
    {
        nloops++;
        if(a.front() == b.front())
            a.pop_front(), b.pop_front();
        else
        {
            std::deque<int>* m = a.front() < b.front() ? &a : &b;
            std::deque<int>* M = m == &a ? &b : &a;
            int x = a.front(), y = b.front();
            a.pop_front(); 
            b.pop_front();
            M->push_back(std::min(x, y));
            M->push_back(std::max(x, y));
        }
        if(nloops > 1000000) // We're in a cycle most likely
            return false;
    }
    return !a.empty();
}

int main()
{
    bool win = true;
    int N, x;
    std::cin >> N;
    std::mt19937 mt(N);
    std::vector<int> A, B;
    for(int i = 1; i <= N; i++)
    {
        std::cin >> x;
        B.push_back(x);
        A.push_back(i);
    }

    if(N > 4)
        while(!simulate(A, B))
            std::shuffle(A.begin(), A.end(), mt);
    else 
        do win = simulate(A, B); 
           while (!win && std::next_permutation(A.begin(), A.end()));

    if(win)
    {
        std::cout << "YES\n";
        for(auto a : A)
            std::cout << a << " ";
    }
    else
        std::cout << "NO\n";
}