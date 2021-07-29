/* 1345. HTML - http://acm.timus.ru/problem.aspx?num=1345
 *
 * Strategy:
 * Straightforward parsing.
 *
 * Performance:
 * O(n), where n is the size of the input, runs the tests in 0.015s using 56KB memory.
 */

#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>

std::string str;
std::string keywords[] = {
    "and", "array", "begin", "case", "class", "const", "div", "do", "else", "end", "for",
    "function", "if", "implementation", "interface", "mod", "not", "of", "or", "procedure",
    "program", "record", "repeat", "shl", "shr", "string", "then", "to", "type", "unit", "until",
    "uses", "var", "with", "while"
};

std::string lower(std::string str) {
    std::transform(str.begin(), str.end(), str.begin(), std::tolower);
    return str;
}

std::string parse(int& i, std::string& type) {
    int j = i;
    if(str[i] == '{') {
        int nest = 1;
        for(++i; nest; i++)
            nest += str[i] == '{' ? 1 : str[i] == '}' ? -1 : 0;
        type = "comment";
    }
    else if(str[i] == '/' && str[i+1] == '/') {
        while(str[++i] != '\n');
        type = "comment";
    }
    else if(str[i] == '\'') {
        while(str[++i] != '\'');
        i++;
        type = "string";
    }
    else if(str[i] == '#' && std::isdigit(str[i+1])) {
        while(std::isdigit(str[++i]));
        type = "string";
    }
    else if(str[i] == '_' || std::isalpha(str[i])) {
        int j = i;
        while(std::isalpha(str[i]) || str[i] == '_' || std::isdigit(str[i]))
            i++;
        type = "identifier";
        for(auto k : keywords) {
            if(lower(str.substr(j, i-j)) == k) {
                type = "keyword";
                return str.substr(i-k.size(), k.size());
            }
        }
    }
    else if(std::isdigit(str[i])) {
        while(std::isdigit(str[i]))
            i++;
        if(str[i] == '.' && std::isdigit(str[i+1]))
            for(i++; std::isdigit(str[i]); i++);
        type = "number";
    }
    if(type.empty())
        i++;
    return str.substr(j, i-j);
}

int main() {
    std::string s;
    std::stringstream ss;
    while(std::getline(std::cin, s))
        str += s + '\n';
    str += '\0';
    for(int i = 0; i < str.length() && str[i];) {
        std::string type;
        s = parse(i, type);
        if(type.size() && type != "identifier")
            ss << "<span class=" << type << ">" << s << "</span>";
        else
            ss << s;
    }
    std::cout << ss.str();
}
