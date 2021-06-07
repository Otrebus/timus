/* 1700. Awakening - http://acm.timus.ru/problem.aspx?num=1700
 *
 * Strategy:
 * Map each association to an integer and each object to a set of associations. For each given 
 * object set, initialize a list of associations from the first object, and iterating through
 * each object and each association we remove the association if it can't be found in an object.
 * The remaining associations are then reported. 
 *
 * Performance:
 * O(m*M log M), where M is the maximum number of associations for each object.
 */

#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <string>
#include <sstream>
#include <list>
#include <algorithm>

std::unordered_set<int> objSet[1000]; // The associations of each object set in integer form
std::unordered_map<std::string, int> dict; // The mapping from association to integer
std::vector<std::string> inv; // The inverse above mapping
int nWords = 0;
int nObjSets = 0;

int main()
{   
    // Mapping from string to object set
    std::unordered_map<std::string, std::unordered_set<int>*> strToObj;
    std::ios::sync_with_stdio(false);

    std::string input;
    std::getline(std::cin, input);
    int N = std::stoi(input);
    while(N--)
    {
        std::getline(std::cin, input);
        std::stringstream ss(input);
        std::string str;
        ss >> str;
        str.pop_back();
        if(strToObj.find(str) == strToObj.end())
            strToObj.insert( { str, objSet+nObjSets++ } );
        // For each association ..
        while(ss >> str)
        {
            // .. insert the association into our association->int mapping
            auto it = dict.insert( { str, nWords } ).first;
            inv.push_back(str);
            nWords++;
            objSet[nObjSets-1].insert(it->second);
        }
    }

    std::getline(std::cin, input);
    int M = std::stoi(input);
    while(M--)
    {
        std::getline(std::cin, input);
        std::stringstream ss(input);
        std::string str;
        // From the first given object we initialize the list of comon associations
        ss >> str;
        std::list<int> list(strToObj[str]->begin(), strToObj[str]->end());
        while(ss >> str)
        {
            // Remove the associations from the first list that aren't found in each set
            auto set = strToObj[str];
            for(auto it = list.begin(); it != list.end(); )
                if(set->find(*it) == set->end())
                    it = list.erase(it);
                else
                    it++;
        }
        // Report answer using the inverse mapping from our int representation of the associations
        // to the string representation
        std::vector<std::string> ans;
        for(auto it = list.begin(); it != list.end(); it++)
            ans.push_back(inv[*it]);
        std::sort(ans.begin(), ans.end());
        if(ans.size())
            for(auto it = ans.begin(); it < ans.end(); it++)
                std::cout << *it << " ";
        else
            std::cout << "No solution.";
        std::cout << std::endl;
    }
}