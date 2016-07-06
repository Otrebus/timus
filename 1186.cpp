/* 1186. Chemical Reactions - http://acm.timus.ru/problem.aspx?num=1186
 *
 * Strategy:
 * Standard recursive-descent parsing.
 *
 * Performance:
 * O(NL^2) where L is the length of a line, runs the test cases in 0.015s using 304KB memory.
 */

#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>

class Solution
{
    std::unordered_map<std::string, int> elements;
public:
    Solution (std::string str = "")
    {
        if(!str.empty())
            elements[str] = 1;
    }

    Solution operator* (int t) const
    {
        Solution s = *this;
        for(auto& p : s.elements)
            p.second = p.second*t;
        return s;
    }

    Solution operator+ (const Solution& rhs) const
    {
        Solution s = *this;
        for(auto& p : rhs.elements)
            s.elements[p.first] += p.second;
        return s;
    }

    bool operator== (const Solution& rhs) const
    {
        auto b = rhs.elements == elements;
        return b;
    }
};

std::pair<int, int> parseNumber(std::string str, int pos)
{
    int num = 0;
    while(isdigit(str[pos]))
        num = num*10 + str[pos++]-'0';
    return { num, pos };

}

// These parsing functions all parse the prefix starting at the provided pos of the string according
// to the parse rule given and return the representation of the chemical solution that the prefix
// represents along with the position of the first character after the prefix
std::pair<Solution, int> parseSequence(std::string str, int pos);

std::pair<Solution, int> parseElement(std::string str, int pos)
{
    std::string ret;
    if(str[pos] == '(')
    {
        auto p = parseSequence(str, pos+1);
        return { p.first, p.second + 1 };
    }
    if(!isalpha(str[pos]))
        return { {}, pos };
    ret.push_back(str[pos++]);
    while(isalpha(str[pos]) && islower(str[pos]))
        ret.push_back(str[pos++]);
        
    return { Solution(ret), pos };
}

std::pair<Solution, int> parseSequence(std::string str, int pos)
{
    auto p = parseElement(str, pos);
    if(p.second == pos)
        return { {}, pos };
    auto q = parseNumber(str, p.second);
    int r = !q.first ? 1 : q.first;
    auto rest = parseSequence(str, q.second);
    return { p.first*r + rest.first, rest.second };
}

std::pair<Solution, int> parseFormula(std::string str, int pos)
{
    auto p = parseNumber(str, pos);
    int r = p.second == pos ? 1 : p.first;
    auto q = parseSequence(str, p.second);
    if(str[q.second] == '+')
    {
        auto t = parseFormula(str, q.second+1);
        return { q.first * r + t.first, t.second };
    }
    return { q.first*r, q.second };
        
}

int main()
{
    std::string lhs, rhs;
    std::cin >> lhs;
    int N;
    std::cin >> N;
    while(N--)
    {
        std::cin >> rhs;
        std::cout << lhs;
        std::cout << (parseFormula(lhs, 0).first == parseFormula(rhs, 0).first ? "==" : "!=");
        std::cout << rhs << "\n";
    }
}