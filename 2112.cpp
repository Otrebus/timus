/* 2112. Battle log - http://acm.timus.ru/problem.aspx?num=2112
 *
 * Strategy:
 * Straightforward simulation according to the given rules while checking for inconsistent actions
 * followed by greedily combining the derived team fragments.
 *
 * Performance:
 * Linear in the size of the input, runs the tests in 0.031s using 552KB memory.
 */

#include <iostream>
#include <unordered_map>
#include <string>
#include <vector>

struct player {
    int hp; // Health points
    std::string name;
    player* parent;
    int rank;
} players[1000];

std::unordered_map<std::string, player*> P; // Maps strings to players

// Maps team fragment representatives to a team fragment
std::unordered_map<player*, std::vector<player*>> T;

void link(player* a, player* b) { // Makes player a and b part of the same team fragment
    if(a->rank > b->rank)
        b->parent = a;
    else {
        a->parent = b;
        if(a->rank == b->rank)
            b->rank++;
    }
}

player* find(player* s) { // Disjoint team fragment find
    if(s != s->parent)
        s->parent = find(s->parent);
    return s->parent;
}

void join(player* a, player* b) { // Disjoint team fragment union
    link(find(a), find(b));
}

int main() {
    int n, m;
    std::cin >> n;
    for(int i = 0; i < n; i++) {
        std::string p;
        std::cin >> p;
        P[p] = &players[i];
        P[p]->parent = P[p];
        P[p]->hp = 3;
        P[p]->name = p;
    }
    bool fake = false;
    std::cin >> m;
    while(m--) {
        std::string player, action, player2, dummy, action2;
        std::cin >> player >> action;
        if(P[player]->hp < 2)
            fake = true;
        if(action == "HIT") {
            std::cin >> player2 >> dummy >> action2;
            P[player2]->hp = std::max((action2 == "HEAD") ? P[player2]->hp-2 : P[player2]->hp-1, 0);
        } else if(action == "USES") {
            std::cin >> dummy;
            P[player]->hp = 3;
        } else if(action == "REVIVE") {
            std::cin >> player2;
            if(P[player2]->hp != 1)
                fake = true;
            P[player2]->hp = 2;
            join(P[player], P[player2]);
        }
    }

    // Map team sizes to groups
    for(int i = 0; i < n; i++)
        T[find(&players[i])].push_back(players+i);
    std::vector<std::vector<player*>> S[1001];
    for(auto t = T.begin(); t != T.end(); t++)
        S[t->second.size()].push_back(t->second);

    // Try to combine team fragments
    std::vector<player*> team;
    std::vector<std::vector<player*>> teams;
    for(int s = 4; s >= 1; s--) { // For groups of each size
        while(!S[s].empty()) {
            team = S[s].back();
            S[s].pop_back();

            // Combine the fragment with other fragments towards a team, try the best fit first
            for(int t = 4-s; t >= 0; t--) {
                while(!S[t].empty() && team.size() + t <= 4) {
                    team.insert(team.end(), S[t].back().begin(), S[t].back().end());
                    S[t].pop_back();
                }
                if(team.size() == 4) {
                    teams.push_back(team);
                    break;
                }
            }
            if(team.size() != 4) {
                fake = true;
                break;
            }
        }
    }

    if(teams.size() != n/4)
        fake = true;
    for(auto& t : teams)
        if(team.size() != 4)
            fake = true;
    if(fake) {
        std::cout << "FAKE";
    } else {
        std::cout << "CORRECT" << std::endl;
        for(auto& team : teams) {
            for(auto player : team)
                std::cout << player->name << " ";
            std::cout << std::endl;
        }
    }
}
