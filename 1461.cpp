/* 1461. Christmas Garland - http://acm.timus.ru/problem.aspx?num=1461
 *
 * Strategy:
 * Scan the string in reverse until the pattern u*(h?)d* (reversed) ends, from that place forward,
 * insert as many d's as possible and from then alternate between u and d (throwing in a 'h' to
 * bring parity at the end if necessary).
 *
 * Performance:
 * O(n), runs the tests in 0.015s using 548KB memory.
 */

#include <string>
#include <iostream>

int getp(std::string in) {
    // Returns the point the pattern ends
    int i = in.size() - 1;
    int nh = 0;
    while(in[i] == 'd') // First the d's
        i--;
    while(in[i] == 'h') { // Then at most one h
        i--;
        if(nh++)
            return i+1;
    }
    while(in[i--] == 'u') // Then a bunch of u's
        if(!i)
            return -1; // Reached the beginning
    return i+1;
}

std::string solve(std::string in) {
    int p = getp(in);
    if(p < 0)
        return "No solution";

    // Calculate the height at the breakoff point
    int h = 1, c[256]; c['u'] = 1, c['d'] = -1, c['h'] = 0;
    for(int i = 0; i <= p; i++)
        h += c[in[i]];
    
    in[p] = in[p] == 'h' ? 'u' : 'h';
    
    int i = p + 1;
    while(h > 1) // Spam a bunch of 'd''s until height is one
        in[i++] = 'd', h--;
    
    while(i < in.size()) { // Maintain as low height as possible for the rest of the string
        int j = in.size() - i;
        if(h) { // If we have height, make a 'd' unless we necessarily need a 'h' close to the end
            if(j != 2)
                in[i++] = 'd', h--;
            else
                in[i++] = 'h';
        }
        else // No height, we're forced to make an 'u'
            in[i++] = 'u', h++;
    }
    return in;
}

int main() {
    int n;
    std::string s;
    std::cin >> n >> s;
    std::cout << solve(s);
}

/*

    // Bonus: A program that helped me visualize the garland transitions, press enter to refresh

    #include <string>
    #include <map>
    #include <iostream>
    #include <vector>

    int w = 10;

    std::string nextp(std::string inp) {
        std::string sp = "dhu";

        std::map<char, int> m = {{ 'd',  0}, {'h', 1}, {'u', 2}};

        for(int i = inp.size()-1; i >= 0; i--) {
            inp[i] = sp[(m[inp[i]]+1)%3];
            if(inp[i] != 'd')
                break;
        }
        return inp;
    }

    bool valid(std::string inp) {
        int h = 0;
        for(int i = 0; i < inp.size(); i++) {
            if(h == 0 && inp[i] == 'h')
                return false;
            if(inp[i] == 'u')
                h++;
            if(inp[i] == 'd')
                h--;
            if(h < 0)
                return false;
        }
        return h == 0;
    }

    std::string print(std::string inp) {
        std::vector<std::string> out(inp.size(), std::string(inp.size()+1, ' '));
        for(int i = 0; i < inp.size(); i++) {
            out[i][inp.size()] = '\n';
        }

        std::map<char, int> m = {{ 'u',  '/'}, {'h', '_'}, {'d', '\\'}};
        int h = 0;
        for(int i = 0; i < inp.size(); i++) {
            if(inp[i] == 'd')
                h--;
            out[h][i] = m[inp[i]];
                    if(inp[i] == 'u')
                h++;
        }
    
        std::string o;
        for(auto s = out.rbegin(); s < out.rend(); s++)
            o += *s;
        return o;
    }

    int main() {
        std::string s = std::string(w, 'd');
        while(true) {
            s = nextp(s);
            if(valid(s)) {
                std::cin.ignore();
                std::cout << print(s) << std::endl;
            }
        }
    }

*/
