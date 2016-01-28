/* 1101. Robot in the Field - http://acm.timus.ru/problem.aspx?num=1101
 *
 * Strategy:
 * Recursive descent (parsed into a tree) + simulation.
 *
 * Performance:
 * Runs the test cases in 0.001s using 456KB memory.
 */

#include <iostream>
#include <string>
#include <functional>
#include <memory>
#include <sstream>

int pos = 0;
std::string input;
bool regs['Z'-'A'+1];
char field[201][201];

template<typename T, typename... Args>
std::unique_ptr<T> mu(Args&&... args) // instead of std::make_unique
{
    return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
}

class ParseNode // A node in a parse tree, returned by parseExpr() and his friends
{
public:
    virtual bool eval() = 0;
};

template<typename T> class BinaryNode : public ParseNode // Represents a binary expression
{
    std::unique_ptr<ParseNode> left, right;
    T op;
public:
    BinaryNode(std::unique_ptr<ParseNode> l, std::unique_ptr<ParseNode> r)
        : left(std::move(l)), right(std::move(r)) { }
    virtual bool eval() { return op(left->eval(), right->eval()); }
};

template<typename T> class UnaryNode : public ParseNode // Represents "NOT"
{
    std::unique_ptr<ParseNode> down;
    T op;
public:
    UnaryNode(std::unique_ptr<ParseNode> node) : down(std::move(node)) {}
    virtual bool eval() { return op(down->eval()); }
};

class RegisterNode : public ParseNode // Represents a letter "A"-"Z"
{
    char reg;
public:
    RegisterNode(char reg) : reg(reg) {}
    virtual bool eval() { return regs[reg-'A']; }
};

class ConstNode : public ParseNode // Represents "TRUE" or "FALSE"
{
    bool b;
public:
    ConstNode(bool b) : b(b) {}
    virtual bool eval() { return b; }
};

std::string getToken(bool consume) // Reads the next token in the stream, consume = false just peeks
{
    int k = pos;
    if(k >= input.size()) // EOF returns empty string
        return "";
    std::string ret;
    while(input[k] == ' ') // Skip whitespace
        k++;
    if(input[k] == '(' || input[k] == ')')
        ret = input[k++];
    else
        while(input[k] >= 'A' && input[k] <= 'Z')
            ret += input[k++];
    if(consume)
        pos = k;
    return ret;
}

std::unique_ptr<ParseNode> parseExpr();

std::unique_ptr<ParseNode> parseAtom() // Parses the tightest bound stuff (NOT, ()'s and registers)
{
    std::string token = getToken(true);
    if(token == "(")
    {
        auto ptr = parseExpr();
        getToken(true);
        return ptr;
    }
    else if(token == "NOT")
        return mu<UnaryNode<std::logical_not<bool>>>(parseAtom());
    else if(token == "TRUE")
        return mu<ConstNode>(true);
    else if(token == "FALSE")
        return mu<ConstNode>(false);
    else
        return mu<RegisterNode>(token[0]);
}

std::unique_ptr<ParseNode> parseAndExpr() // Parses AND expressions 
{
    auto left = parseAtom();
    std::string str = getToken(false);
    if(str != "AND") // Parse the rest of the AND expression optionally
        return std::move(left);
    getToken(true);
    return mu<BinaryNode<std::logical_and<bool>>>(std::move(left), std::move(parseAndExpr()));
}

std::unique_ptr<ParseNode> parseExpr() // Same as above but for OR
{
    auto left = parseAndExpr();
    std::string str = getToken(false);
    if(str != "OR")
        return std::move(left);
    getToken(true);
    return mu<BinaryNode<std::logical_or<bool>>>(std::move(left), std::move(parseExpr()));
}

int main()
{
    std::ios::sync_with_stdio(false);
    int N, M, K, x, y;
    std::getline(std::cin, input);
    auto p = parseExpr();
    std::string str;
    std::getline(std::cin, str);
    std::stringstream ss(str);
    ss >> N >> M >> K;
    while(M--)
    {
        std::getline(std::cin, str);
        std::stringstream ss(str);
        ss >> x >> y;
        field[100+x][100+y] = '>';
    }
    while(K--)
    {
        std::getline(std::cin, str);
        std::stringstream ss(str);
        char c;
        ss >> x >> y >> c;
        field[100+x][100+y] = c;
    }
    x = y = 0;
    std::pair<int, int> dir = { 1, 0 };
    do
    {
        std::cout << x << " " << y << std::endl;
        char c = field[100+x][100+y];
        if(c == '>')
        {
            std::swap(dir.first, dir.second); // Rotate ccw
            dir.first = -dir.first;
            if(p->eval()) // True = reverse direction to cw
                dir.first = -dir.first, dir.second = -dir.second;
        }
        else if(c)
            regs[c-'A'] = !regs[c-'A'];
        x += dir.first; y += dir.second;
    } while(x >= -N && y >= -N && x <= N && y <= N);
}