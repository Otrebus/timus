/* 1939. First Seal - http://acm.timus.ru/problem.aspx?num=1939
 *
 * Strategy:
 * The guards' best chance is to time the lowering of the gate so it would barely nudge the rear of
 * the vehicle if it keeps going without braking, since this would give Jack the shortest possible
 * time to decelerate. Use the equations of motion to calculate where the vehicle stops if he 
 * immediately releases the horse as the gate starts lowering, to see if the vehicle stops soon
 * enough not to hit the gate.
 *
 * Performance:
 * Constant, runs the test cases in 0.001s using 236KB memory.
 */

#include <iostream>

int main()
{
    long long l, h, H, v, x, a;
    std::cin >> l >> h >> H >> v >> x >> a;
    std::cout << ((2*a*v*(H-h)-x*v*v > 2*a*x*l) ? "Safe" : "Crash");
}