/* 1089. Verification with a Vocabulary - http://acm.timus.ru/problem.aspx?num=1089
 *
 * Strategy:
 * For each word, read through the entire dictionary and see if there is a word that differs by one
 * letter. If so, substitute the word. Otherwise, leave everything else intact.
 *
 * Performance:
 * Running time is proportional to the number of input words times the amount of words in the
 * dictionary; runs the test set in 0.001s using 432KB memory.
 */

#include <iostream>
#include <string>
#include <vector>

std::vector<std::string> voc;

bool trans(std::string& str)
{
    for(int i = 0; i < voc.size(); i++)
    {
        if(str.length() == voc[i].length())
        {
            int cor = 0;
            for(int j = 0; j < voc[i].length(); j++)
                cor += voc[i][j] != str[j];
            if(cor == 1)
            {
                str = voc[i];
                return true;
            }
        }
    }
    return false;
}

int main()
{
    std::string input;

    std::getline(std::cin, input);
    while(input != "#")
    {
        voc.push_back(std::move(input));
        std::getline(std::cin, input);
    }

    std::string output, word;
    output.reserve(input.length());
    char c;
    int cors = 0;

    while(std::cin.read(&c, 1))
    {
        if(c < 'a' || c > 'z')
        {
            if(trans(word))
                cors++;
            output += word + c;
            word = "";
        }
        else
            word += c;
    }
    if(!word.empty())
        output += trans(word);
    std::cout << output << cors;
}