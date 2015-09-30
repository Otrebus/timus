/* 1015. Test the Difference! - http://acm.timus.ru/problem.aspx?num=1015
 *
 * Strategy:
 * Pre-map every possible permutation into a representation of its unique die (which contains a 
 * list), and iterate over the input, adding each input die into its proper place.
 *
 * Performance:
 * O(N), running the tests in 0.015s using 440KB memory. The solution is ranked 31/2934 and is the 
 * fastest solution for the compiler used.
 */

#include <stdio.h>
#include <unordered_map>
#include <algorithm>
#include <cctype>

std::vector<int> dies[30];
auto nextFree = dies;
std::unordered_map<int, std::vector<int>*> permToDie;
std::unordered_map<int, std::vector<int>*> combToDie;

// Maps the key index to the index of the opposite side
char opposite[6] = { 1, 0, 4, 5, 2, 3 };
// Maps the key index to the indices of the sides clockwise around its axis
char rotate[6][4] = { { 2, 3, 4, 5 }, { 5, 4, 3, 2 }, { 0, 5, 1, 3 }, 
                      { 1, 4, 0, 2 }, { 0, 3, 1, 5 }, { 0, 4, 1, 2 } };

// Tries to add (combination, vector) pair to the mapping, returns the pointer to the vector
inline std::vector<int>* addCombination(int combination)
{
    auto it = combToDie.find(combination);
    if(it == combToDie.end())
    {
        combToDie.insert( { combination, nextFree++ } );
        return nextFree-1;
    }
    else
        return it->second;
}

// Turns a permutation into its unique combination representation. The latter representation is 
// an integer whose digits from the most significant digit downwards is thusly: the value opposite
// to the 6 first, followed by the clockwise values around the axis of the 6, with the greatest 
// number first.
inline int permToComb(const std::vector<int>& perm)
{
    int comb = 0;
    int pos = find(perm.begin(), perm.end(), 6) - perm.begin(); // Position of the side opposite 6
    comb += perm[opposite[pos]]*10000;
    int nm = perm[opposite[pos]] == 5 ? 4 : 5; // The greatest value except 6 and its opposite
    int pos2 = std::find(perm.begin(), perm.end(), nm) - perm.begin(); // The position of the above
    // Find that position in the rotation array so that we can gyrate and record the values
    int pos3 = std::find(rotate[pos], rotate[pos] + 4, pos2) - rotate[pos];
    int mul = 1000;
    for(int i = 0; i < 4; i++, mul /= 10)
        comb += perm[rotate[pos][(pos3 + i) % 4]]*mul;
    return comb;
}

inline int getint()
{
    int ret = 0, ch;
    while(!isdigit(ch = getc(stdin)));
    ret = ch - '0';
    while(isdigit(ch = getc(stdin)))
        ret = ret*10 + ch - '0';
    return ret;
}

int main()
{
    int N = getint();
    std::vector<int> perm = { 1, 2, 3, 4, 5, 6 };
    do
    {
        int comb = permToComb(perm);
        auto p = addCombination(comb);
        int pint = 0;
        // Turn { 1, 2, 3, 4, 5, 6 } into 123456 etc
        for(int i = 0, mul = 100000; i < 6; i++, mul /= 10)
            pint += perm[i]*mul;
        permToDie.insert( { pint, p } );
    } 
    while(next_permutation(perm.begin(), perm.end()));

    std::vector<std::vector<int>*> output;
    for(int i = 1; i <= N; i++)
    {
        int pint = 0; // See previous comment
        for(int j = 0, mul = 100000; j < 6; j++, mul /= 10)
        {
            int d = getint();
            pint += d*mul;
        }

        // Every time we have get as input some unique die (not belonging to an earlier group)
        // it will necessarily be the first die of a new group of dies that we will output:
        if(permToDie[pint]->empty())
            output.push_back(permToDie[pint]);
        permToDie[pint]->push_back(i);
    }
    printf("%d\n", output.size());
    for(int i = 0; i < output.size(); i++)
    {
        for(int j = 0; j < output[i]->size(); j++)
            printf("%d ", (*output[i])[j]);
        printf("\n");
    }
}