/* 1583. Cheese - http://acm.timus.ru/problem.aspx?num=1583
 *
 * Strategy:
 * Do binary search over the length of the cheese to find the position that gives the correct
 * volume; repeat for each new slice.
 * 
 * Performance:
 * O(nlog w) where w is the length of the cheese, runs the tests in 0.015s using 180KB memory.
 */

#include <stdio.h>
#include <algorithm>
#include <vector>

using ld = long double;
const ld pi = 3.141592653589793238462643383279;

struct hole {
    ld x, r;
} holes[100];
int n, i;

ld calc(ld x1, ld x) { // Calculates the area of a block of cheese between x1 and x
    ld V = (x-x1)*100;
    auto clamp = [] (ld x, ld b) { return std::min(std::max(x, 0.l), b); };

    // Removes the volume of any holes between x1 and x
    for(int i = 0; i < n; i++) {
        auto& h = holes[i];
        ld a = clamp(x1-(h.x-h.r), h.r*2), b = clamp(x-(h.x-h.r), h.r*2);
        V -= pi*(b*b*(3*h.r-b)-a*a*(3*h.r-a))/3; // Volume of a sphere between x=a and x=b
    }
    return V;
}

ld find(ld x1) { // Binary search over calc
    ld L = x1, R = 100;
    while(R - L > 1e-9)
        *(calc(x1, (R+L)/2) < 500 ? &L : &R) = (R+L)/2;
    return L;
}

int main() {
    scanf("%d", &n);
    for(int i = 0; i < n; i++)
        scanf("%*Lf %Lf %*Lf %Lf", &holes[i].x, &holes[i].r);

    std::vector<int> out;
    // Use binary search to find slice positions until we're close enough to the end
    for(ld x = find(0); x < 100-(1e-4)/2;x = find(x)) {
        auto b = int((x+1e-4/2)*10000);
        out.push_back(b);
        x = ld(b)/10000;
    }

    printf("%d\n", out.size());
    for(auto o : out)
        printf("%d\n", o);
}
