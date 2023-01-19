/* 1561. Winnie the Pooh - http://acm.timus.ru/problem.aspx?num=1561
 *
 * Strategy:
 * Matrix row deduction in Z_7. We maintain a linear equation system where every time we Add
 * a honey, we add a row of zeroes to its coefficient matrix, when we Learn a new combination,
 * we add the corresponding row to the matrix and also row reduce it, and when we Eat
 * something, we row reduce a row equivalent to A_0*x_0 + ... A_k*x_k + S = z, solve it for z
 * and check that it gives a result.
 *
 * Performance:
 * O(N^3), runs the tests in 0.343s using 4,044KB.
 */

#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>

int R[7] = { 0, 1, 4, 5, 2, 3, 6 };
int N, k = 0, S, E;
int A[1000][1000];
int B[1000];

bool c;

std::string days[7] = { 
    "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday", "Sunday"
};
std::unordered_map<std::string, int> M;

// Reduces a row corresponding to the equation v[0]x_0 + ... + v[k]x_k = d 
int reduce(std::vector<int>& v, int d) {
   for(int i = 0; i < k; i++) {
       if(A[i][i]) {
           int pt = v[i]; // The pivot
           for(int j = i; j < k; j++)
               v[j] = (7 + v[j] - (A[i][j]*pt)%7)%7;
           d = (7 + d - (pt*B[i])%7)%7;
       }
   }
   return d;
}

int main() {
    scanf("%d", &N);
    for(int i = 0; i < 7; i++)
        M[days[i]] = i;

    for(int i = 0; i < N; i++) {
        char s[10];
        std::cin >> s;
        if(s[0] == 'A') { // Adds a row
            k++;
        } else if(s[0] == 'L') {
            std::vector<int> v(k);
            std::string ss, ee;

            for(int i = 0; i < k; i++) {
                std::cin >> v[i];
                v[i] %= 7;
            }
            std::cin >> ss >> ee;

            int S = M[ss], E = M[ee];
            int b = reduce(v, (7+E-S)%7);

            // Check if there's a nonzero entry in the reduced row
            bool found = false;
            for(int i = 0; i < k && !found; i++) {
                if(v[i]) {
                    found = true;
                    break;
                }
            }
            // No nonzero entry found, if there's an entry on the right side we are inconsistent
            if(!found && b) {
                c = true;
                continue;
            }

            // We have a new pivot point for the coefficient matrix
            for(int i = 0; i < k; i++) {
                if(v[i] && !A[i][i]) {
                    for(int j = i; j < k; j++)
                        A[i][j] = (v[j]*R[v[i]])%7;
                    B[i] = (b*R[v[i]])%7;
                    break;
                }
            }
        } else if(s[0] == 'E') {
            std::vector<int> v(k);
            std::string ss, ee;
            for(int i = 0; i < k; i++) {
                std::cin >> v[i];
                v[i] %= 7;
            }

            std::cin >> ss;
            int S = M[ss];

            if(c) { // In the past, we reached an inconsistency and went nuts
                std::cout << "Already crazy" << std::endl;
                continue;
            }

            // Row-reduce the row corresponding to the equation and check that it gives a result
            // meaning it has no free variables
            auto b = reduce(v, 7-S);
            bool t = true;
            for(int i = 0; i < k; i++) 
                if(v[i])
                    t = false;

            if(!t)
                std::cout << "Don't know" << std::endl;
            else {
                std::cout << days[(7-b)%7] << std::endl;
            }
        }
    }
}
