/* 1836. Babel Fish - http://acm.timus.ru/problem.aspx?num=1836
 *
 * Strategy:
 * We categorize our strategy by the number of nonzero points:
 * 
 *  0: The volume is zero.
 *  1: Completely ambiguous.
 *  2: If the two points are diagonal, we necessarily have an error since the surface can't
 *     possibly form a plane, and if they are on the same edge it's ambiguous.
 *  3: Here we have enough information to calculate the intersection of the plane with two
 *     of the bottom edges and can use that information to calculate the total volume, if the
 *     intersection lies within the tank (otherwise we have an error).
 *  4: We check if the points all lie in the same plane, and output the volume if so.
 * 
 * Performance:
 * Constant, runs the tests in 0.001s using 272KB memory.
 */

#include <iostream>
#include <algorithm>
#include <iomanip>

using ll = long long;
using ld = long double;

ll sgn(ll a) { return a >= 0 ? 1 : -1; } // Sign function

ld test0(ll B, ll s[4]) { return 0; } // No nonzero points

ld test1(ll B, ll s[4]) { return -1; } // One nonzero

ld test2(ll B, ll s[4]) { return s[1] == 0 && s[3] == 0 ? -2 : -1; } // Two

ld test3(ll B, ll s[4]) { // Three nonzero points

    ld s0 = s[0], s1 = s[1], s2 = s[2];
    ll l1 = s[0] - s[1], l2 = s[2] - s[1];

    // We are given three intersections of the plane with the box; these are the two other
    // intersections which lie on two of the bottom edges of the box
    ld s4 = -s2*B/l1;
    ld s5 = -s0*B/l2;

    // The intersections need to be within the box
    if(std::abs(l1) < 1 || std::abs(l2) < s[0] || s4 <= 0 || s5 <= 0)
        return -2;

    // The volume is formed by three irregular triangular prisms, volume = base*(h1 + h2 + h3)/3
    ld A1 = B*s4/2.0l, A2 = B*s5/2.0l, A3 = B*B-(B-s4)*(B-s5)/2-A1-A2;
    return (A1*(s2 + s1) + A2*(s0 + s1) + A3*s1)/3;
}

ld test4(ll B, ll s[4]) { // Four nonzero points

    // Transpose all heights so the height of the first point is zero
    ll m = *std::min_element(s, s+4);
    for(int i = 0; i < 4; i++)
        s[i] -= m;
    while(s[0])
        for(int i = 0; i < 3; i++)
            std::swap(s[i], s[i+1]);

    // Check if the points form a plane which is surprisingly easy (this started out as
    // the triple vector product but was simplified)
    if(s[2] - s[3] - s[1] != 0)
        return -2;

    // The height of an irregular rectangular solid, volume = base*(h1 + h2 + h3 + h4)/4
    return ld(B*B)*(m*4 + s[0] + s[1] + s[2] + s[3])/4.0l;
}

ld test(ll B, ll s[4]) {
    ld (*f[5])(ll B, ll s[4]) = { test0, test1, test2, test3, test4 };
    int S = 0;
    for(int i = 0; i < 4; i++) // Determine the number of nonzero points
        S += s[i] != 0;
    while(S < 4 && s[3]) // Rotate the points so the fourth point is zero
        for(int i = 0; i < 3; i++)
            std::swap(s[i], s[i+1]);
    return !B ? 0 : f[S](B, s); // Call the appropriate solver by the number of nonzero points
}

int main() {
    int t;
    std::cin >> t;
    for(int i = 0; i < t; i++) {
        ll B, s[4];
        std::cin >> B >> s[0] >> s[1] >> s[2] >> s[3];
        ld ans = test(B, s);

        if(ans < 0)
            std::cout << (ans == -2 ? "error" : "ambiguous") << std::endl;
        else
            std::cout << std::fixed << std::setprecision(10) << ans << std::endl;
    }
}
