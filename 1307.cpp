/* 1307. Archiver - http://acm.timus.ru/problem.aspx?num=1307
 *
 * Strategy:
 * Replace some common digraphs of the string with symbols, output a program which takes that
 * string and performs the reverse replacement.
 *
 * Performance:
 * Linear in the size of the input, runs the tests in 0.031s using 644KB memory.
 */

#include <stdio.h>
#include <string>
#include <unordered_map>

char str[1000021];
std::string out; // The string to be transformed by the output program

std::unordered_map<std::string, char> m = {
    { "th", '#' }, { "er", '$' }, { "on", '*' }, { "an", '@' }, { "re", '^' },
    { "he", '`' }, { "in", '%' }, { "ed", '<'},  { "nd", '>' }, { "ha", '+' },
};

int main() {
    int c, n = 0;
    while ((c = getchar()) != EOF)
        str[n++] = c;
    
    for(int i = 0; i < n; i++) {
        std::string s(str+i, 2);
        if(m.find(s) != m.end()) { // If this is a digraph, replace it with a symbol
            out += m[s];
            i++;
        } else if(str[i] == '\n') // Linebreaks need to be explicit, and ..
            out += "\\n";
        else if(str[i] == '\"')   // .. quotation marks need to be escaped
            out += "\\\"";
        else
            out += str[i];
    }

    std::string prog = // This does the reverse of the above
       "//CPP\n"
       "#include <iostream>\n"
       "#include <unordered_map>\n"

       "std::unordered_map<char, std::string> m = {"
           "{'#',\"th\"},{'$',\"er\"},{'*',\"on\"},{'@',\"an\"},{'^',\"re\"},"
           "{'`',\"he\"},{'%',\"in\"},{'<',\"ed\"},{'>',\"nd\"},{'+',\"ha\"},"
           "{'=',\"\\n\"}"
       "};"

       "std::string out = \""+out+"\";"
       "int main() {"
           "std::string s;"
           "for(int i = 0; i < out.size(); i++) {"
               "if(m.find(out[i]) != m.end())"
                   "s += m[out[i]];"
               "else "
                   "s += out[i];"
            "}"
            "std::cout << s;"
       "}";

    printf("%s", prog.c_str());
}
