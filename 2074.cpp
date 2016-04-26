/* 2074. Timus problems classifier - http://acm.timus.ru/problem.aspx?num=2074
 *
 * Strategy:
 * Straightforward; associate every problem with the categories it belongs to, then from each
 * problem, create a category set that we add the problem to. Finally sort all category sets and
 * output them.
 *
 * Performance:
 * 0.015s, 732KB.
 */

#include <iostream>
#include <sstream>
#include <unordered_map>
#include <vector>
#include <string>
#include <algorithm>

// From http://stackoverflow.com/questions/20511347/a-good-hash-function-for-a-vector
template <typename T> class VecHash
{
public:
    std::size_t operator()(std::vector<T> const& vec) const 
    {
        std::size_t seed = vec.size();
        for(auto& i : vec)
            seed ^= std::hash<T>()(i) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        return seed;
    }
};

std::unordered_map<std::vector<int>, std::string, VecHash<int>> topicNames;
std::unordered_map<std::string, std::vector<int>> topicSets;
std::unordered_map<int, std::vector<std::vector<int>>> problems;

bool isTopic(std::string str) // Checks if a string has a dot in it
{
    for(int i = 0; i < str.size(); i++)
        if(str[i] == '.')
            return true;
    return false;
}

std::vector<int> parseNum(std::string num) // Parses the ip-like numbers into vectors of numbers
{
    std::vector<int> result;
    std::string n;
    std::stringstream ss(num);
    while(std::getline(ss, n, '.'))
        result.push_back(std::stoi(n));
    return result;
}

std::vector<int> getTopics(std::string num) // Parses a string of numbers into a vector of numbers
{
    std::vector<int> ret;
    std::stringstream ss(num);
    int x;
    while(ss >> x)
        ret.push_back(x);
    return ret;
}

// Parses a topic header into { vector<int> numbers, name }
std::pair<std::vector<int>, std::string> parseTopic(std::string str)
{
    std::stringstream ss(str);
    std::string num, name;
    ss >> num >> std::ws;
    std::getline(ss, name);
    return { parseNum(num), name };
}

// Converts a string 1.2.3. into foo.bar.baz
std::string toStr(std::vector<int> v)
{
    std::string str;
    std::vector<int> t;
    for(int i = 0; i < v.size(); i++)
    {
        t.push_back(v[i]);
        str += topicNames[t];
        if(i < v.size() - 1)
            str += '.';
    }
    return str;
}

// Puts into a comma separated string the names of all categories a problem belongs to
std::string getTopicSetStr(int v)
{
    std::vector<std::string> ts;
    std::vector<std::vector<int>> topics = problems[v];
    for(int i = 0; i < topics.size(); i++)
        ts.push_back(toStr(topics[i]));
    std::sort(ts.begin(), ts.end());
    std::string ret;
    for(int i = 0; i < topics.size(); i++)
    {
        ret += ts[i];
        if(i < topics.size() - 1)
            ret += ", ";
    }
    return ret;
}

int main()
{
    std::string input;
    std::vector<int> curTopic;
    while(std::getline(std::cin, input))
    {
        if(isTopic(input))
        {
            auto p = parseTopic(input);
            curTopic = p.first;
            topicNames[p.first] = p.second;
        }
        else
        {
            auto v = getTopics(input);
            for(auto t : v)
                problems[t].push_back(curTopic);
        }
    }

    std::vector<std::vector<int>> v;
    // Go through the problems and form topic sets
    for(auto x : problems)
        topicSets[getTopicSetStr(x.first)].push_back(x.first);
    for(auto& s : topicSets)
        std::sort(s.second.begin(), s.second.end());

    // Sort and print the output
    std::vector<std::pair<std::string, std::vector<int>>> 
               output(topicSets.begin(), topicSets.end());
    std::sort(output.begin(), output.end(), [] ( std::pair<std::string, std::vector<int>> a,
                                                 std::pair<std::string, std::vector<int>> b) 
                                               { return a.second[0] < b.second[0]; } );
    std::vector<std::string> outstr; // Not sure if this is needed since the lines are probably
                                     // already ordered lexicographically
    for(int i = 0; i < output.size(); i++)
    {
        std::stringstream line;
        for(int j = 0; j < output[i].second.size(); j++)
        {
            line << output[i].second[j];
            if(j < output[i].second.size()-1)
                line << ", ";
        }
        line << " : " << output[i].first;
        outstr.push_back(line.str());
    }
    std::sort(outstr.begin(), outstr.end());
    for(auto s : outstr)
        std::cout << s << std::endl;
}