/* 1299. Psylonians - http://acm.timus.ru/problem.aspx?num=1299
 *
 * Strategy:
 * N/A.
 *
 * Performance:
 * O(1), runs the tests in 0.015s using 276KB memory.
 */

#include <iostream>
#include <string>

int E, A, M, MP, N, NP, D, DW;
std::string P;

void shoot() {
    std::cout << "FIRE " << std::min(20, A);
}

void move(std::string dir) {
    std::cout << dir << " " << std::min(E, 100);
}

void stop() {
    std::cout << "STOP";
}

void guard() {
    if(M == 0)
        stop();
    else if(std::abs(D) < 5)
        shoot();
    else if(D >= 5)
        move("LEFT");
    else if(D <= -5)
        move("RIGHT");
}

void retreatReturnFire() {
    if(!M || std::abs(D) >= 5)
        move("BACKWARD");
    else
        shoot();
}

void advanceReturnFire() {
    if(!M || std::abs(D) >= 10)
        move("FRONT");
    else
        shoot();
}

void attack() {
    if(N*NP > M*MP*3)
        advanceReturnFire();
    else
        retreatReturnFire();
}

void defend() {
    if(M*20 >= A)
        retreatReturnFire();
    else
        guard();
}

void route() {
    if(std::abs(DW) <= 20)
        move("FRONT");
    else if(std::abs(DW) >= 160)
        move("BACKWARD");
    else {
        if(DW > 20 && DW <= 90 || DW < -90 && DW > -160)
            move("LEFT");
        else
            move("RIGHT");
    }
}

void patrol() {
    if(!M)
        route();
    else
        defend();
}

int main() {
    std::cin >> E >> A >> P >> M >> MP >> D;

    if(P == "G") {
        guard();
    } else if(P == "D") {
        defend();
    } else if(P == "A") {
        std::cin >> N >> NP;
        attack();
    } else if(P == "P") {
        std::cin >> DW;
        patrol();
    }
}
