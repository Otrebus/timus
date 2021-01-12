/* 1546. Japanese Sorting - http://acm.timus.ru/problem.aspx?num=1546
 *
 * Strategy:
 * Insert each word into a trie, letter by letter, with the following exceptions. When we reach a
 * number, we insert a node corresponding to the length of the number without zeroes, followed
 * by nodes corresponding to each digit of the number like normal. At the end of the entire word,
 * insert nodes corresponding to the lengths of the zero prefixes we encountered in the numbers,
 * in the order of encountering them, and at the last node save a reference to the word itself.
 * When we've inserted all the words we can do a straightforward DFS of the tree and print things
 * according to the given priorities.
 *
 * Performance:
 * Linear in the size of the input, runs the tests in 0.14s using 60060KB memory.
 */

#include <vector>
#include <iostream>
#include <map>
#include <unordered_map>
#include <string>

#pragma comment(linker, "/STACK:16777216")

std::vector<std::string> lines;

struct node {
    std::map<char, node> ch; // Zeros and digits
    std::map<int, node> len; // Lengths of numbers without the zero prefix
    std::map<int, node> zero; // The lengths of the zero prefixes, inserted last
};
node root;

std::map<node*, std::vector<int>> lineMap; // Maps leafs to words

void addLine(std::string& line, int i) {
    // Adds a line into the trie
    node* n = &root;
    std::vector<int> zeros;
    for(int i = 0; i < line.size();) {
        if(!std::isdigit(line[i])) { // Insert normal characters
            n = &(n->ch[line[i++]]);
        }
        else { // For numbers
            int z = 0;
            while(i < line.size() && line[i] == '0') // Strip away the zeroes, count them
                z++, i++;
            std::string num;
            while(i < line.size() && std::isdigit(line[i])) {
                num.push_back(line[i++]);
            }
            n = &(n->len[num.size()]); // Insert a node equal to the length of the number
            for(auto u : num)
                n = &(n->ch[u]); // Insert the nonzero digits

            zeros.push_back(z); // Save the length of the zero-prefix for last
        }
    }
    for(auto z : zeros)
        n = &(n->zero[z]); // Insert all the zero-prefix lengths
    lineMap[n].push_back(i); // Save a reference to the word at the resulting node
}

void printTree(node* n) {
    // Recursively prints the tree
    for(auto& l : lineMap[n])
        std::cout << lines[l] << std::endl; // First print the lines that ended at this leaf

    for(auto it = n->zero.rbegin(); it != n->zero.rend(); it++)
        printTree(&it->second); // Then follow the zero-prefix tie breakers

    for(auto& p : n->len)
        printTree(&(p.second)); // Then follow numbers by lengths 

    for(auto& p : n->ch)
        printTree(&(p.second)); // Finally print characters
}

int main() {
    std::string line;

    int i = 0;
    std::vector<std::string> output;
    while (std::cin >> line) {
        lines.push_back(line);
        addLine(line, i++);
    }
    printTree(&root);

    for(auto& o : output)
        std::cout << o << std::endl;
}