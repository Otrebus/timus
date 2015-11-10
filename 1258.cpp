/* 1258. Pool - http://acm.timus.ru/problem.aspx?num=1258
 *
 * Strategy:
 * Calculate the total horizontal and vertical distances travelled from the bounce data given, and
 * then report their hypotenuse.
 *
 * Performance:
 * Linear in the size of the input, uses 0.015s time and 456KB memory to execute the test cases.
 */

#include <iostream>
#include <string>
#include <cmath>
#include <iomanip>

int main()
{
    long long W, H, x, y, xf, yf;
    std::string str;
    std::cin >> W >> H >> x >> y >> xf >> yf >> str;
    long long xd = 0, yd = 0;

    for(auto c : str)
    {
        switch(c)
        {
        case 'F':
            yd += y;
            y = 0;
            break;
        case 'B':
            yd += H-y;
            y = H;
            break;
        case 'R':
            xd += W-x;
            x = W;
            break;
        case 'L':
            xd += x;
            x = 0;
            break;
        }
    }
    xd += std::abs(x-xf), yd += std::abs(y-yf);
    std::cout << std::fixed << std::setprecision(4) << std::sqrt(double(xd*xd + yd*yd));
}