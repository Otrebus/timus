/* 1891. Language Ocean - http://acm.timus.ru/problem.aspx?num=1891
 *
 * Strategy:
 * Naive parsing and bookmaking. Given the uninteresting nature of this problem, this is the only
 * comment.
 * 
 * Performance:
 * 0.015s, 692 KB.
 */

#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <map>

struct function
{
    std::string name;
    std::string ret;
    std::vector<std::string> param;
};

struct variable
{
    std::string name;
    std::string type;
    bool isAuto;
};

std::vector<function> f;
std::vector<variable> v;

function* findfun(function b)
{
    for(int i = 0; i < f.size(); i++)
    {
        auto& a = f[i];
        if(a.name == b.name)
        {
            if(a.param.size() == b.param.size())
            {
                bool match = true;
                for(int i = 0; i < a.param.size(); i++)
                    if(a.param[i] != b.param[i] && !(a.param[i] == "auto" || b.param[i] == "auto"))
                        match = false;
                if(match)
                    return &a;
            }
        }
    }
    return 0;
}

variable* findvar(std::string s)
{
    for(int i = 0; i < v.size(); i++)
        if(v[i].name == s)
            return &v[i];
    return 0;
}

class ParseException
{
    int n; std::string str;
public:
    ParseException(int n, std::string str) : n(n), str(str)
    {
    }
    std::string what()
    {
        return "Error on line " + std::to_string(n) + ": " + str;
    }
};

function parseFunction(std::string s, bool declaration)
{
    int p = 0;
    std::string name;
    std::string ret;
    std::vector<std::string> par;
    while(s[p++] != '(')
        name.push_back(s[p-1]);
    while(true)
    {
        std::string arg;

        while(s[p] != ')' && s[p] != ' ' && s[p] != ',')
            arg.push_back(s[p++]);
        if(!arg.empty())
            par.push_back(arg);
        while(s[p] == ' ' || s[p] == ',')
            p++;
        if(s[p] == ')')
            break;
    }
    p++;
    while(s[p] == ' ' || s[p] == ':')
        p++;
    while(p < s.size())
        ret += s[p++];
    function fn { name, ret, par };
    if(declaration)
    {
        f.push_back(fn);
        return fn;
    }
    else
        return fn;
}

void parseVariable(int line, std::string s)
{
    std::stringstream ss(s);
    std::string name, type, str, fstr;
    ss >> type >> name >> str;
    auto isAuto = false;
    if(type == "auto")
        isAuto = true;
    while(ss >> str)
        fstr += str;
    auto fn = parseFunction(fstr, false);
    std::vector<variable> vars;
    if(findvar(name))
        throw ParseException(line, "Double declaration");
    std::map<std::string, std::string> m;
    for(auto p : fn.param)
    {
        auto it = findvar(p);
        if(!it)
            throw ParseException(line, "Unknown variable");
        else
            vars.push_back(*it);
    }
    function ft = fn;
    for(int i = 0; i < ft.param.size(); i++)
        ft.param[i] = vars[i].type;
    auto itf = findfun(ft);
    if(!itf)
        throw ParseException(line, "No such function");
    if(type == "auto")
        type = itf->ret;
    else if(itf->ret != type)
        throw ParseException(line, "Invalid initialization");
    for(int i = 0; i < itf->param.size(); i++)
        if(vars[i].type == "auto")
            vars[i].type = itf->param[i];
    for(int i = 0; i < vars.size(); i++)
    {
        if(vars[i].type != "auto")
            continue;
        else
            findvar(vars[i].name)->type = itf->param[i];
    }
    v.push_back( { name, type, isAuto } );
}

void parse()
{
    std::ios::sync_with_stdio(false);
    std::string input;
    std::getline(std::cin, input);
    int N = std::stoi(input);
    int line = 0;
    while(N--)
    {
        std::getline(std::cin, input);
        parseFunction(input, true);
    }
    std::getline(std::cin, input);
    int M = std::stoi(input);
    while(M--)
    {
        std::getline(std::cin, input);
        parseVariable(++line, input);
    }
    for(auto x : v)
        if(x.isAuto)
            std::cout << x.name << " : " << x.type << std::endl;
}

int main()
{
    try
    {
        parse();
    }
    catch(ParseException e)
    {
        std::cout << e.what() << std::endl;
    }
}