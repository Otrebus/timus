/* 1575. Yekaterinburg Subway 2 - http://acm.timus.ru/problem.aspx?num=1575
 *
 * Strategy:
 * Floyd-Warshall.
 *
 * Performance:
 * Cubic in the number of stations, runs the tests in 0.031s using 468KB memory.
 */

#include <vector>
#include <string>
#include <unordered_map>
#include <iostream>

std::vector<std::vector<std::string>> lines = {
    { "7_klyuchey", "Sortirovochnaya", "China_town", "Zarechny", "City", "1905_year_square",
      "Kuybyshevskaya", "Sibirskaya", "Siniye_kamni", "Lechebnaya", "Varshavskaya",
      "Kompressornaya", "Koltsovo" },
    { "Zelyony_ostrov", "Tatishchevskaya", "Verh_Isetskaya", "Kommunarov_square",
      "1905_year_square", "Teatralnaya", "Vostochnaya", "Vtuzgorodok", "Kamennye_palatki",
      "University"},
    { "MEGA", "Metallurgov", "Kraulya", "Central_stadium", "Moskovskaya", "1905_year_square",
      "Shevchenko", "Pionerskaya", "Turbinnaya", "Elmash", "Taganskaya"},
    { "Akademicheskaya", "Yugo_zapadnaya", "Volgogradskaya", "Posadskaya", "Geologicheskaya",
      "Teatralnaya", "Gagarinskaya", "Komsomolskaya", "Shefskaya", "Ozyornaya", "Italyanskaya",
      "Kalinovskaya" },
    { "Sovhoznaya", "Voennaya", "Aviatsionnaya", "Dvorets_sporta", "Geologicheskaya",
      "Kuybyshevskaya", "Vostochnaya", "Gagarinskaya", "Vilonovskaya" },
    { "Keramicheskaya", "Vtorchermet", "Samolyotnaya", "Botanicheskaya", "Parkovaya",
      "Mayakovskaya", "Oborony_square", "Kuybyshevskaya", "Teatralnaya", "Shevchenko", "Uralskaya",
      "Zvezda", "I_Pyatiletki_square", "Pobedy" },
    { "Himmash", "Nizhne_Isetskaya", "Uktusskie_Gory", "Shcherbakovskaya", "Botanicheskaya",
      "Chkalovskaya", "Bazhovskaya", "Geologicheskaya", "1905_year_square", "Dinamo", "Uralskaya",
      "Mashinostroiteley", "Uralmash", "Prospekt_Kosmonavtov", "Bakinskih_Komissarov" },
    { "Moskovskaya", "Kommunarov_square", "City", "Uralskaya", "Pionerskaya", "Gagarinskaya",
      "Vtuzgorodok", "Sibirskaya", "Oborony_square", "Bazhovskaya", "Dvorets_sporta", "Posadskaya",
      "Moskovskaya" }
};

int A[70][70];
int n = 0;

std::unordered_map<std::string, int> m; // Maps names to station numbers

int reg(std::string s) { // Assigns or returns a station number
    if(m.find(s) == m.end())
        return (m[s] = n++);
    return m[s];
}

int main() {

    for(int i = 0; i < 70; i++) // Initialize the matrix
        for(int j = 0; j < 70; j++)
            A[i][j] = i == j ? 0 : 100000;

    for(auto line : lines) { // Assign the station connections
        for(int i = 0; i < line.size() - 1; i++) {
            auto a = reg(line[i]), b = reg(line[i+1]);
            A[a][b] = A[b][a] = 1;
        }
    }

    for(int k = 0; k < 70; k++) // Run Floyd-Warshall
        for(int i = 0; i < 70; i++)
            for(int j = 0; j < 70; j++)
                if(A[i][j] > A[i][k] + A[k][j])
                    A[i][j] = A[i][k] + A[k][j];

    std::cin >> n;
    while(n--) { // Read and print the results
        std::string a, b;
        std::cin >> a >> b;
        std::cout << A[m[a]][m[b]] << std::endl;
    }
}
