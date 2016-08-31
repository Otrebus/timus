/* 2099. Space Invader - http://acm.timus.ru/problem.aspx?num=2099
 *
 * Strategy:
 * Set up the equation A + (B-A)t = D + (C-D)s and solve it using matrix inversion to find the point
 * in space where these two parametric lines intersect. Since this is an overdetermined system, we
 * need to do some extra checks for consistency. All equations are reordered to make sure we don't
 * have to use floating point arithmetic.
 *
 * Performance:
 * Constant, runs the tests in 0.015s using 292KB memory.
 */

#include <iostream>

bool run()
{
    long long Ax, Ay, Az, Bx, By, Bz, Cx, Cy, Cz, Dx, Dy, Dz;
    std::cin >> Ax >> Ay >> Az >> Bx >> By >> Bz >> Cx >> Cy >> Cz >> Dx >> Dy >> Dz;
    // The constant vector
    auto u = Dx - Ax, v = Dy - Ay, w = Dz - Az;
    // The coefficients of the matrix
    auto a = Bx - Ax, b = Dx - Cx, c = By - Ay, d = Dy - Cy, e = Bz - Az, f = Dz - Cz;
    if((Bx-Ax)*(Cx-Dx) + (By-Ay)*(Cy-Dy) + (Bz-Az)*(Cz-Dz) != 0) // Check for right angle
        return false;
    // Checks that a/b >= 1
    auto cmp = [] (long long a, long long b) { return b > 0 ? a >= b : a <= b; };
    long long t, s, det;
    // Check which rows of the overdetermined 3x3 matrix we can use for the 2x2 matrix
    if(a*d != b*c)
        t = u*d - b*v, s = v*a - u*c, det = a*d - b*c;
    else if(c*f != d*e)
        t = f*v - d*w, s = c*w - e*v, det = c*f - d*e;
    else if(a*f != b*e)
        t = u*f - b*w, s = a*w - e*u, det = a*f - b*e;
    else
        return false;
    if(!cmp(t, det) || !cmp(s, det)) // Make sure we don't turn too early (parameters within bounds)
        return false;
    if(t*a + b*s != det*u || t*c + s*d != det*v || t*e + s*f != det*w) // Check for consistency
        return false;
    return true;
}

int main()
{
    std::cout << (run() ? "Valid" : "Invalid");
}