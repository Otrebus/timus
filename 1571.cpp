/* 1571. Interpreters - http://acm.timus.ru/problem.aspx?num=1571
 *
 * Strategy:
 * See code.
 *
 * Performance:
 * Linear. Runs the tests in 0.001s using 404KB memory.
 */

#include <iostream>
#include <string>
#include <unordered_set>

int main()
{
    std::unordered_set<std::string> s;
    int N;
    std::cin >> N;
    for(int i = 0; i < N; i++)
    {
        std::string language;
        std::cin >> language;
        s.insert(language);
    }
    auto it = s.begin();
    if(N < 2 || N == 2 && s.size() == 1)
        std::cout << "0\n";
    else if(N == 2 && s.size() == 2)
        std::cout << "1\n" << *(s.begin()) << "-" << *(++it) << "\n";
    else if(N > 2 && s.size() != N)
        std::cout << "Impossible\n";
    else 
    {
        std::cout << s.size() << "\n";
        while(it != s.end())
            std::cout << *(it++) << "-otrebian\n";
    }
}