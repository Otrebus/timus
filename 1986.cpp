/* 1986. Paul's Salads - http://acm.timus.ru/problem.aspx?num=1986
 *
 * Strategy:
 * Solve the resulting linear system by Gaussian elimination.
 *
 * Performance:
 * O(n^3), runs the tests in 0.015s using 324KB memory.
 */

#include <iomanip>
#include <vector>
#include <map>
#include <string>
#include <iostream>

double A[10][20]; // The matrix

void addTo(int j, int i, double d, int m) // Adds a multiple of one row of the matrix to another
{
    for(int k = 0; k < m; k++)
        A[j][k] += d*A[i][k];
}

int main()
{
    std::ios::sync_with_stdio(false);
    std::map<std::string, int> eq[10]; // Each salad equation, as map from string to integer
    std::map<std::string, int> ing; // Maps each ingredient to an index
    std::string ingi[20]; // The inverse of the above
    std::string str, num;
    int n = 0;
    while(std::cin >> str >> num)
    {
        if(num == ":") // A new salad
        {
            eq[n][str] = -100;
            ingi[n] = str;
            ing[str] = n++;
        }
        else // An ingredient of a salad
        {
            eq[n-1][str] += stoi(num);
            if(ing.find(str) == ing.end())
                ing[str] = -1;
        }
    }
    int m = n;
    for(auto& p : ing) // Map ingredients (names not assigned yet) to indices
        if(p.second == -1)
            p.second = m++;
    for(int i = 0; i < n; i++) // Fill in the matrix
        for(auto p : eq[i])
            A[i][ing[p.first]] = p.second, ingi[ing[p.first]] = p.first;
    for(int i = 0; i < n; i++) // For each diagonal ..
    {
        double p = A[i][i];
        // Normalize the row by the pivot position
        for(int j = i; j < m; j++)
            A[i][j] /= p;
        // Subtract all above rows by this row
        for(int j = i + 1; j < n; j++)
            addTo(j, i, -A[j][i], m);
        // Subtract them downwards
        for(int j = i - 1; j >= 0; j--)
            addTo(j, i, -A[j][i], m);
    }
    for(int i = 0; i < n; i++) // Output
    {
        std::cout << ingi[i] << " : ";
        for(int j = n; j < m; j++)
            std::cout << ingi[j] << " " << std::setprecision(10) << -A[i][j]*100.0 << " ";
        std::cout << "\n";
    }
}