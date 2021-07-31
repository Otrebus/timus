/* 1347. Blog - http://acm.timus.ru/problem.aspx?num=1347
 *
 * Strategy:
 * Straightforward parsing.
 *
 * Performance:
 * O(n + u^2 log u) where n is the size of the input and u is the number of users, runs in 0.031s
 * using 3,136KB memory
 */

#include <iostream>
#include <string>
#include <sstream>
#include <map>
#include <set>
#include <vector>

std::map<std::string, std::set<std::string>> friends; // Friends of user
std::map<std::string, std::set<std::string>> friendOf; // Users who have user as friend
std::vector<std::string> users; // All blog authors

std::string str; // The input

template<typename T> void read(int& i, T& s) {
    // Reads something from position i in the input string
    std::stringstream ss;
    while(std::isspace(str[i]))
        i++;
    while(str[i] != '\n')
        ss << str[i++];
    ss >> s;
    i++;
}

void readTag(int& i, std::string& tag, int& a, int& b) {
    // Reads a <tag>blah</tag> from the input string at position str, giving a and b as the
    // endpoints of the interior string and i the first position after the closing tag
    tag = "";
    while(str[i++] != '<');
    while(str[i] != '>')
        tag += str[i++];
    a = ++i;
    b = str.find("</" + tag + ">", a);
    i = b;
    while(str[i++] != '>');
}

void readBlog(int& i, std::string user) {
    // Reads a blog entry, assigning the friend relationships
    std::string tag;
    int a, b;
    readTag(i, tag, a, b);
    for(int j = a; j < b;) { // Loop through the blog contents
        if(str[j] == '<') {  // Start of a tag
            std::string tag;
            int a, b;
            readTag(j, tag, a, b);
            if(tag == "friend") { // Assign the relationships
                std::string user2 = str.substr(a, b-a);
                if(user != user2) {
                    friends[user].insert(user2);
                    friendOf[user2].insert(user);
                }
            }
        } else
            j++;
    }
}

template<typename T> void outputList(int n, T l) { // Outputs the string contents of some iterable
    std::cout << n << ": ";
    for(auto it = l.begin(); it != l.end(); it++) {
        if(it != l.begin())
            std::cout << ", ";
        std::cout << *it;
    }
    std::cout << std::endl;
}

int main() {
    // Read the input into string str
    std::string s;
    while(std::getline(std::cin, s))
        str += s + '\n';
    str += '\0';
    int N, i = 0;

    // Start parsing str, starting with N
    read(i, N);
    for(int j = 0; j < N; j++) { // Read each user and his blog
        std::string name;
        read(i, name);
        readBlog(i, name);
        users.push_back(name);
    }

    for(auto u : users) { // Output the info
        std::cout << u << std::endl;

        outputList(1, friends[u]);
        outputList(2, friendOf[u]);

        std::set<std::string> s;
        for(auto f : friends[u]) // Mutual friends
            if(friendOf[u].find(f) != friendOf[u].end())
                s.insert(f);
        outputList(3, s);

        std::cout << std::endl;
    }
}
