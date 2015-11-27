/* 2048. History - http://acm.timus.ru/problem.aspx?num=2048
 *
 * Strategy:
 * Use the fact that each period of a consecutive 400-year cycle starting on some given year begins
 * with the same day of the week and has the same number of Friday the 13ths, to quickly iterate 
 * between the given years.
 *
 * Performance:
 * O(1), runs in 0.015s using 384KB memory.
 */

#include <iostream>

int result[13];
int m[12] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

int printResult()
{
    for(int i = 0; i < 13; i++)
        std::cout << i << ": " << result[i] << "\n";
    return 0;
}

bool isLeap(int n)
{
    return(n % 4 == 0 && (n % 400 == 0 || n % 100));
}

void addToResult(int fst, int y)
{
    m[1] = isLeap(y) ? 29 : 28;
    int n = 0;
    for(int i = 0; i < 13; i++)
    {
        if(fst == 6) // Month that starts on a sunday has a Friday the 13th
            n++;
        fst = (fst + m[i]) % 7;
    }
    result[n]++;
}

int main()
{
    int y1, y2;
    std::cin >> y1 >> y2;
    int fst = 1; // First day of the year; 1 January 1918 is Tuesday

    // First calculate the first day of the week year
    int y = 1918+400*((y1-1-1918)/400); // Skip all first 400 year periods
    for(++y; y < y1; y++)
        fst = (fst + 365 + int(isLeap(y-1))) % 7; // Iterate the rest, calculating the first day

    if(y1+400 < y2)
    {   // Calculate how many Friday the 13ths the 400-year period has 
        for(y = y1; y < y1+400; y++)
        {
            fst = (fst + 365 + int(isLeap(y-1))) % 7;
            addToResult(fst, y);
        }
        y = y1 + 400;
    }
    else
        y = y1;
    for(int i = 0; i < 13; i++) // Add the results of all 400-year-periods in the given range
       result[i] += result[i]*((y2-1-y)/400);
    y += 400*((y2-1-y)/400);

    for(; y <= y2; y++) // Add the last snippet of the range given
    {
        fst = (fst + 365 + int(isLeap(y-1))) % 7;
        addToResult(fst, y);
    }
    printResult();
    return 0;
}