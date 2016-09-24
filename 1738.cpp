/* 1738. Computer Security - http://acm.timus.ru/problem.aspx?num=1738
 *
 * Strategy:
 * Straightforward; for each personal number, find all other similar personal numbers by performing
 * the given mutations to it, and record by how many places each such similar number differs from
 * the original.
 *
 * Performance:
 * O(nlog n), runs the tests in 0.312s using 596KB memory.
 */

#include <vector>
#include <iostream>

int pow10[6] = { 1, 10, 100, 1000, 10000, 100000 };
int pow16[6] = { 1, 0x10, 0x100, 0x1000, 0x10000, 0x100000 };

// Functions for inserting, removing, replacing and accessing a digit from a given number
int insert(int x, char i, char c)  { return (x - x%pow10[i])*10 + pow10[i]*c + x%pow10[i]; }
int remove(int x, char i)          { return x/pow10[i+1]*pow10[i] + x%pow10[i]; }
int replace(int x, char i, char c) { return x - x%pow10[i+1]/pow10[i]*pow10[i] + c*pow10[i]; }
int at(int x, char i)              { return x%pow10[i+1]/pow10[i]; }
int at16(int x, char i)            { return (x >> (i*4)) & 0xF; }

int ans[4];
int num[(1 << 16) + 1];

std::vector<int> getSimilar(int x, int n)
{
    std::vector<int> v;
    int l = 0; // Length of the id
    int digit[5]; // Each digit of the number for convenience
    for(int i = 0; i < 5; i++)
        digit[i] = at(x, i);
    for(int k = x; k; k /= 10)
        l++;
    // Insert digits
    if(l < 5)
    {
        for(int prev = 0, i = l; i >= 0; i--)
        {
            for(int c = 0; c < 10; c++)
            {
                auto k = insert(x, i, c);
                if(c != prev && k <= n) // Don't make duplicates - consider adding 5 to the number 5
                    v.push_back(k);     // - if we add it both before and after the 5 we get 2 55's
            }
            if(i)
                prev = digit[i-1];
        }
    }
    // Remove digits
    if(l > 1)
    {
        for(int prev = 0, i = l-1; i >= 0; prev = digit[i--])
        {
            if(i == l-1 && !digit[l-2]) // Removing the leading digit can't reveal a zero behind it
                continue;
            auto k = remove(x, i);
            if(digit[i] != prev && k)
                v.push_back(k);
        }
    }
    // Replace digits
    for(int prev = 0, i = l-1; i >= 0; i--)
    {
        for(int c = (i == l-1 ? 1 : 0); c < 10; c++) // Can't replace the first digit with 0
        {
            auto k = replace(x, i, c);
            if(k <= n && k != x)
                v.push_back(k);
        }
    }
    return v;
}

void update(int a, int b)
{
    int n = 0;
    for(int i = 0; i < 4; i++)
        n += (at16(a, i) == at16(b, i));
    ans[3-n]++;
}

int main()
{
    int n;
    std::cin >> n;
    for(int i = 1; i <= n; i++)
        std::cin >> std::hex >> num[i];
    for(int i = 1; i <= n; i++)
    {
        auto v = getSimilar(i, n);
        for(int j = 0; j < v.size(); j++)
            update(num[i], num[v[j]]);
    }
    for(int i = 0; i < 4; i++)
        std::cout << ans[i]/2 << " "; // We counted each pair twice
}