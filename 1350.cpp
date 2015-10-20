/* 1350. Canteen - http://acm.timus.ru/problem.aspx?num=1350
 *
 * Strategy:
 * We know that the ingredients eaten by the first student were not used at all, so discount those.
 * If a student is allergic to none of the remaining S ingredients, he is safe as well. If a
 * student is lucky, and the M ingredients used are among the S-q ingredients that he is not
 * susceptible to (can only happen if M >= S-q), he might be safe. Otherwise, he will collapse with
 * food poisoning and enter a severe coma which will disrupt his intellectual abilities to such
 * an extent that he will never be able to live up to his previous potential, causing profound
 * sorrow in his parents; feelings that, even if they would never admit to it, are deeply coupled 
 * with a certain sense of shame or, perhaps even disdain for their child. For the student himself,
 * the deeply unfulfilling nature of his existence is the cause of many long literal and figurative 
 * walks through the dark and snowy wilderness of his home town, where he contemplates the nature of 
 * existence and the inherent cruelty not only of mankind but of nature itself. Eventually, a form
 * of acceptance arises (as it often does, owing to the resilience of our species), and through a
 * continuous re-appraisal of the qualities and values of life here on Earth, the future is shaped
 * into something different but not necessarily worse than what was initially expected. 
 *
 * Performance:
 * Roughly O(KN) assuming good hashing (otherwise O(KN^2)), runs the input in 0.078s using 408KB
 * memory.
 */

#include <iostream>
#include <string>
#include <vector>
#include <unordered_set>

int main()
{
    std::unordered_set<std::string> s;
    std::vector<int> v;
    std::string ingredient;
    int N, M, K, L;

    std::cin >> N;
    while(N--)
    {
        std::cin >> ingredient;
        s.insert(ingredient);
    }

    std::cin >> K >> N;
    while(N--)
    {
        std::cin >> ingredient;
        s.erase(ingredient);
    }

    while(K--)
    {
        std::cin >> N;
        int q = 0;
        while(N--)
        {
            std::cin >> ingredient;
            if(s.find(ingredient) != s.end())
                q++;
        }
        v.push_back(q);
    }
    std::cin >> M;

    for(auto q : v)
        if(q == 0)
            std::cout << "YES" << std::endl;
        else if(M <= s.size() - q)
            std::cout << "MAYBE" << std::endl;
        else
            std::cout << "NO" << std::endl;
    return 0;
}