/* 1158. Censored! - http://acm.timus.ru/problem.aspx?num=1158
 *
 * Strategy:
 * Dynamic programming. Store the solution for sentences of length k in A[k], and store the number
 * of strings that end  with some forbidden word i (0 < i < P), but does not contain any other 
 * forbidden words in F[i][k]. Assume we've calculated the solution for a string of length k. 
 * Iterate all of the following for each character c in the character set:
 * First, simply add A[k] to A[k+1]. This is the number of strings with k+1 characters that end 
 * with c that are valid, at least if you don't consider c. If c is the last letter of a forbidden
 * string, we might however have overestimated our solution, so we need to start subtracting the
 * amount of strings of length k+1 that have a forbidden string as suffix that ends with c.
 * Therefore, for each such string of length l, we subtract A[k-l] from A[k+1], since that is the
 * amount of otherwise valid strings that happen to end exactly in that forbidden string. However,
 * there is one more adjustment we need to make. There might be a string straddling the valid part
 * of the string and the forbidden part of the string. For example, let's say we have forbidden
 * strings aabb and bbcc in this situation:
 * We add c to the string KKKKKK (K here means all possibilities of characters), and end up with
 * KKKKKKc and need to remove all strings that end with bbcc, that is, KKKbbcc, from the solution.
 * This amount is equal to the number of valid strings of three characters (KKK). However, when we
 * do this we also remove strings that look like Kaabbcc, which was never counted in the first 
 * place! This is where the array F comes in - we re-add all the strings of length 5 that end in
 * aabb (but have no other internal forbidden strings, otherwise we would have to recurse).
 * The final number of strings ending in bbcc is then used to update A[i][k+1], and removed from
 * A[k+1] (which we naively just added A[k] to to begin with).
 *
 * Performance:
 * O(MNP^2), runs the test suite in 0.015s using 468KB memory (ranked 10/740 and is the fastest
 * solution for the compiler used).
 */

#include <string>
#include <cstring>
#include <vector>
#include <iostream>
#include <algorithm>

class BigInt // Solution can be up to 50^50
{
    char A[100];
    int n;
public:

    BigInt(int k = 0)
    {
        std::memset(A, 0, sizeof(A));
        A[0] = k;
        n = 0;
        while(k)
            A[n] = k, k /= 10;
    }

    void operator+=(BigInt& b)
    {
        int carry = 0, i;
        for(i = 0; i <= b.n; i++)
        {
            A[i] += (b.A[i]+carry);
            carry = A[i]/10;
            A[i] -= carry*10;
        }
        while(carry)
        {
            A[i] += (b.A[i]+carry);
            carry = A[i]/10;
            A[i++] -= carry*10;
        }
        n = std::max(n, i-1);
    }

    void operator-=(BigInt& b)
    {
        int borrow = 0;
        for(int i = 0; i <= n; i++)
        {
            A[i] -= (b.A[i]+borrow);
            borrow = 0;
            if(A[i] < 0)
                A[i] += 10, borrow = 1;
        }
        while(n && !A[n])
            n--;
    }

    operator std::string ()
    {
        std::string ret;
        for(int i = n; i >= 0 ; i--)
            ret += ('0' + A[i]);
        return ret;
    }
} F[10][51], A[51];

// Returns true if the last k letters of a equal the first k letters of b
bool isPre(const std::string& a, const std::string& b, int k)
{
    for(int i = 0; i < k; i++)
        if(a[a.size()-k+i] != b[i])
            return false;
    return true;
}

int main()
{
    std::ios::sync_with_stdio(false);
    // pre[i] = { j, k } means first k letters of i = last k letters of j
    std::vector<std::pair<int, int>> pre[10];
    std::vector<int> last[256]; // last[i] contains a list of all words that end with char i
    std::vector<std::string> v;
    int N, M, P;
    unsigned char chars[51]; // Input can contain ascii codes > 128!

    std::cin >> N >> M >> P >> chars;
    while(P--)
    {
        std::string s;
        std::cin >> s;
        v.push_back(s);
    }

    // Prepare initial solutions (there is exactly 1 valid empty string)
    A[0] = 1;

    // Remove duplicates and forbidden strings that are superstrings of other forbidden strings
    std::sort(v.begin(), v.end());
    v.resize(std::unique(v.begin(), v.end())-v.begin());
    auto u = v;
    for(auto s : u)
        for(auto it = v.begin(); it < v.end(); )
            if(it->find(s) != std::string::npos && *it != s)
                it = v.erase(it);
            else
                ++it;

    // For each forbidden string j, find all forbidden strings i (including j) that have a suffix
    // equal to a prefix of j (that is, they overlap)
    for(int i = 0; i < v.size(); i++)
        for(int j = 0; j < v.size(); j++)
            for(int k = 1; k < std::min(v[i].size(), v[j].size()); k++)
                if(isPre(v[i], v[j], k))
                    pre[j].push_back( { i, k } );
    for(int i = 0; i < v.size(); i++)
        last[(unsigned char)(v[i][v[i].size()-1])].push_back(i);

    // Actual dynamic programming loop
    for(int k = 1; k <= M; k++) // For each length of the string
    {
        for(int c = 0; c < N; c++) // For each possible character as the last character
        {
            unsigned char i = chars[c];
            A[k] += A[k-1]; // Naively add to solution, but this is of course overestimated
            if(last[i].size() != 0)
            {
                for(auto a : last[i]) // For each forbidden string ending in this char
                {
                    // First remove all solutions that end with that -
                    // "we subtract A[k-l] from A[k+1]" in the header comment
                    BigInt totmin = v[a].size() <= k ? A[k-v[a].size()] : 0;
                    for(auto p : pre[a]) // Each other forbidden string straddling this string
                    {
                        int b = p.first;
                        int n = p.second;
                        int sz = v[a].size();
                        // Adjust for straddling strings (the last paragraph of the header comment)
                        if(k-sz+n > 0)
                            totmin -= F[b][k-sz+n];
                    }
                    F[a][k] = totmin;
                    A[k] -= totmin;
                }
            }
        }
    }
    std::cout << (std::string) A[M];
}