/* 1187. Statistical Trouble - http://acm.timus.ru/problem.aspx?num=1187
 *
 * Strategy:
 * Straightforward tabulation. To get the percentages correct, round down all row and column-wise
 * percentage sums with fractional parts, and then iterate over them and increment each of them by
 * one until the sum reaches 100.
 *
 * Performance:
 * Runs the tests in 0.046s using 360KB memory.
 */

#include <iostream>
#include <sstream>
#include <unordered_map>
#include <iomanip>

struct answer {
    std::string name;
    char c;
    int count;
};

std::string surveyName;
struct questions {
    std::string code;
    std::string name;
    std::vector<answer> A; // The answers
    std::unordered_map<char, int> Am; // Associates an answer character to its index
} Q[100];

std::unordered_map<std::string, int> Qm; // Associates a question name to its index
std::vector<std::string> A; // The answer input lines
int q, a, p;

void drawCrossTable(std::string q1n, std::string q2n, std::string name) {
    std::cout << surveyName << " - " << name << std::endl;

    // Write out the input poll answers and questions
    for(auto q : { q1n, q2n }) {
        auto& qu = Q[Qm[q]];
        std::cout << q << " " << qu.name << std::endl;
        for(int i = 0; i < qu.A.size(); i++)
            std::cout << " " << qu.A[i].c << " " << qu.A[i].name << std::endl;
    }
    std::cout << std::endl;

    auto& q1 = Q[Qm[q1n]], q2 = Q[Qm[q2n]];
    std::cout << "       ";
    for(int i = 0; i < q2.A.size(); i++)
        std::cout << q2.code << ":" << q2.A[i].c << " ";
    std::cout << "TOTAL" << std::endl;

    int X[10][10];
    std::memset(X, 0, sizeof(X));

    // Calculate the cross values
    auto q1i = Qm[q1n], q2i = Qm[q2n];
    for(int i = 0; i < A.size(); i++) {
        int a = Q[q1i].Am[A[i][q1i]];
        int b = Q[q2i].Am[A[i][q2i]];
        X[a][b]++;
    }

    // Calculate the vertical sums
    int V[10][10];
    std::memset(V, 0, sizeof(V));
    int sumV[10];
    std::memset(sumV, 0, sizeof(sumV));
    for(int j = 0; j < Q[q2i].A.size(); j++) {
        for(int i = 0; i < Q[q1i].A.size(); i++)
            sumV[j] += X[i][j];

        // Convert to percentages rounded down
        for(int i = 0; i < Q[q1i].A.size(); i++)
            if(sumV[j])
                V[i][j] = (100*X[i][j])/(sumV[j]);

        // Pre-incremental percentage sum
        int sum2 = 0;
        for(int i = 0; i < Q[q1i].A.size(); i++)
            sum2 += V[i][j];

        // If we can round something up and we haven't reached a sum of 100%, do so
        for(int i = Q[q1i].A.size()-1; i >= 0; i--)
            if(sumV[j] && (100*X[i][j])%sumV[j] && sum2 < 100)
                V[i][j]++, sum2++;
    }

    // Same as above but for horizontal sums
    int H[10][10];
    std::memset(H, 0, sizeof(H));
    int sumH[10];
    std::memset(sumH, 0, sizeof(sumH));

    for(int i = 0; i < Q[q1i].A.size(); i++) {
        for(int j = 0; j < Q[q2i].A.size(); j++)
            sumH[i] += X[i][j];

        for(int j = 0; j < Q[q2i].A.size(); j++)
            if(sumH[i])
                H[i][j] = int((100*X[i][j])/(sumH[i]));

        int sum2 = 0;
        for(int j = 0; j < Q[q2i].A.size(); j++)
            sum2 += H[i][j];

        for(int j = Q[q2i].A.size()-1; j >= 0; j--)
            if(sumH[i] && (100*X[i][j])%sumH[i] && sum2 < 100)
                H[i][j]++, sum2++;
    }

    // Global sum
    int sumA = 0;
    for(int i = 0; i < Q[q1i].A.size(); i++)
        sumA += sumH[i];

    int Ph[10];
    std::memset(Ph, 0, sizeof(Ph));

    int sum4 = 0; // Sum of percentages after rounding all down
    for(int i = 0; i < Q[q1i].A.size(); i++)
        sum4 += (Ph[i] = (100*sumH[i])/sumA);

    for(int i = Q[q1i].A.size()-1; i >= 0; i--)
        if(sumA && (sumH[i]*100)%sumA && sum4 < 100)
            Ph[i]++, sum4++;

    // Same as the above but for the vertical sums
    int Pv[10];
    std::memset(Pv, 0, sizeof(Pv));

    int sum6 = 0;
    for(int i = 0; i < Q[q2i].A.size(); i++)
        sum6 += (Pv[i] = (100*sumV[i])/sumA);

    for(int i = Q[q2i].A.size()-1; i >= 0; i--)
        if(sumA && (sumV[i]*100)%sumA && sum6 < 100)
            Pv[i]++, sum6++;

    // Write out all the values of the cross table as requested, not much interesting here
    for(int i = 0; i < q1.A.size(); i++) {
        std::cout << " " << q1.code << ":" << q1.A[i].c << " ";

        for(int j = 0; j < q2.A.size(); j++)
            std::cout << std::right << std::setw(5) << X[i][j] << " ";

        std::cout << std::right << std::setw(5) << sumH[i];
        std::cout << std::endl << "       ";

        for(int j = 0; j < q2.A.size(); j++) {
            if(sumH[i])
                std::cout << std::right << std::setw(4) << H[i][j] << "% ";
            else
                std::cout << "    - ";
        }
        std::cout << (sumH[i] ? " 100%" : "    -") << std::endl << "       ";

        for(int j = 0; j < q2.A.size(); j++) {
            if(sumV[j])
                std::cout << std::right << std::setw(4) << V[i][j] << "% ";
            else
                std::cout << "    - ";
        }
        std::cout << std::setw(4) << std::right << Ph[i] << "%" << std::endl;
    }

    std::cout << " TOTAL ";
    for(int j = 0; j < q2.A.size(); j++)
        std::cout << std::right << std::setw(5) << sumV[j] << " ";
    std::cout << std::right << std::setw(5) << sumA << std::endl;
    
    std::cout << "       ";
    for(int j = 0; j < q2.A.size(); j++)
        std::cout << std::right << std::setw(4) << Pv[j] << "%" << " ";
    std::cout << " 100%" << std::endl << "       ";

    for(int j = 0; j < q2.A.size(); j++)
        std::cout << std::right << std::setw(5) << (sumV[j] ? "100%" : "-") << " ";

    std::cout << std::setw(5) << "100%" << std::endl;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    // Read all the input, boring stuff
    std::getline(std::cin, surveyName);

    std::string s;

    for(std::getline(std::cin, s), q = -1; s != "#"; std::getline(std::cin, s)) {
        if(s[0] != ' ') {
            a = 0;
            std::string qRef = s.substr(0, 3);
            Qm[qRef] = ++q;
            Q[q].name = s.substr(4);
            Q[q].code = qRef;
        } else {
            char c = s[1];
            std::string qAns = s.substr(3);
            Q[q].A.push_back({ qAns, c, 0 });
            Q[q].Am[c] = a++;
        }
    }

    for(std::getline(std::cin, s), p = 0; s != "#"; p++, std::getline(std::cin, s)) {
        A.push_back(s);
        for(int j = 0; j < q; j++)
            Q[j].A[Q[j].Am[s[j]]].count++;
    }

    for(p = 0, std::getline(std::cin, s); s != "#"; p++, std::getline(std::cin, s)) {
        auto q1 = s.substr(0, 3);
        auto q2 = s.substr(4, 3);
        auto name = s.substr(8);
        drawCrossTable(q1, q2, name);
        std::cout << std::endl;
    }
}
