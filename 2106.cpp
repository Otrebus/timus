/* 2106. Deserialization - http://acm.timus.ru/problem.aspx?num=2106
 *
 * Strategy:
 * Straightforward recursive deserialization/parsing.
 *
 * Performance:
 * Linear in the size of the output, runs the tests in 0.031s using 7612KB memory.
 */

#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <map>

unsigned char readChar(int& p, std::string& code) {
    std::stringstream ss(code.substr(p, 2));
    int k;
    ss >> std::hex >> k;
    p += 2;
    return (char) k;
}

unsigned int readInt(int& p, std::string& code) {
    std::stringstream ss(code.substr(p, 8));
    unsigned int k;
    ss >> std::hex >> k;
    p += 8;
    return k;
}

std::string readString(int &p, std::string& code) {
    unsigned size = readInt(p, code);
    std::string st;
    for(int i = 0; i < size; i++)
        st += readChar(p, code);
    return st;
}

void readStruct(int &p, int n, std::string& code, std::vector<std::string>& lines,
                int indent, std::map<std::string, int> st, std::map<int, int> lt) {

    std::string struc, name;
    std::stringstream ss(lines[n++]);
    ss >> struc >> name;

    std::cout << std::string(indent, ' ') << name << std::endl;
    indent += 1;

    while(p < code.size() && n < lines.size()) {
        std::stringstream ss(lines[n++]);
        std::string type;
        ss >> type;
        if(type == "string") {
            auto s = readString(p, code);
            std::cout << std::string(indent, ' ') << "string " << s << std::endl;
        }
        else if(type == "int") {
            auto i = readInt(p, code);
            std::cout << std::string(indent, ' ') << "int " << i << std::endl;
        }
        else if(type == "struct")
            return;
        else
            readStruct(p, st[type], code, lines, indent, st, lt);
    }
}

int main() {
    int N, L;
  
    std::cin >> N >> L;
    std::vector<std::string> lines;
    std::map<std::string, int> st;
    std::map<int, int> lt;
    std::cin.ignore();

    std::string line;
    int n = 0, s = 0;
    for(int i = 0; i < L; i++) {
        std::getline(std::cin, line);
        lines.push_back(line);

        std::stringstream ss(line);
        std::string type;

        ss >> type;
        if(type == "struct") {
            std::string name;
            ss >> name;
            st[name] = i;
            lt[++s] = i;
        }
    }

    std::string code;
    int p = 0, l = 0;
    std::getline(std::cin, code);

    while(p < code.size()) {

        auto n = lt[readInt(p, code)];

        readStruct(p, n, code, lines, 0, st, lt);
    }
}
