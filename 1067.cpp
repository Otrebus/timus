/* 1067. Disk Tree - http://acm.timus.ru/problem.aspx?num=1067
 *
 * Strategy:
 * Straightforward; the code serves as its own comments.
 *
 * Performance:
 * Linearithmic in the size of the input, runs in 0.062s using 4,540KB memory.
 */

#include <iostream>
#include <map>
#include <sstream>
#include <string>

struct Dir
{
    std::map<std::string, Dir*> subs;
} dirs[50001];
int p = 1;

Dir* addDir(Dir* dir, std::string str)
{
    auto& d = dir->subs[str];
    if(!d)
        d = &dirs[p++];
    return d;
}

void print(Dir* dir, int depth = 0)
{
    for(auto s : dir->subs)
    {
        for(int i = 0; i < depth; i++)
            std::cout << " ";
        std::cout << s.first << "\n";
        print(s.second, depth+1);
    }
}

int main()
{
    int N;
    std::cin >> N;
    for(int i = 0; i < N; i++)
    {
        std::string str, dirstr;
        std::cin >> str;
        std::stringstream ss(str);
        Dir* dir = &dirs[0];
        while (std::getline(ss, dirstr, '\\'))
            dir = addDir(dir, dirstr);
    }
    std::stringstream out;
    print(&dirs[0]);
}