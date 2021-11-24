/* 1050. Preparing an Article - http://acm.timus.ru/problem.aspx?num=1050
 *
 * Strategy:
 * Straightforward parsing according to the rules given.
 *
 * Performance:
 * Runs the tests in 0.015s using 144KB memory.
 */

#include <stdio.h>
#include <ctype.h>
#include <cstring>

enum Type { Text, Quote, Paragraph };

char S[30000]; // The input
char M[30000]; // The output

struct token {
    int i, j; // Start, end
    Type type;
    bool eq(const char* s) { // Compares the text token with a string
        return strncmp(&S[i], s, j-i+1) == 0;
    }
} T[30000];

int n, m, p, t, r; // Various pointers for iterating through stuff

bool streq(char* c, const char* u) { // Compares strings
    return strncmp(c, u, strlen(u)) == 0;
}

token readToken() { // Reads a token (a single character, or one of the mentioned specials)
    token tok { m, m, Text };
    if(S[m] == '\\') { // Formatting
        if(S[m+1] == '\"') { // Diaresis
            tok.type = Type::Text;
            m += 2;
        } else { // A command
            while(isalpha(S[++m]))
                tok.j = m;
            if(tok.eq("\\par"))
                tok.type = Type::Paragraph;
            else
                tok.type = Type::Text;
        }
    } else if(S[m] == 0) { // Treat EOF as a paragraph splitter, because why not
        tok.type = Type::Paragraph;
        m++;
    } else if(S[m] == '\n') {
        // Checks for a "blank line", meaning any whitespace between a pair of '\n's 
        int i = m+1;
        bool allws = true;
        for(; S[i] && S[i] != '\n'; i++) // Check for all-ws
            allws &= isspace(S[i]);
        if(allws) { // The line only had whitespace, treat it as paragraph splitter
            tok.type = Type::Paragraph;
            m = i+1;
        } else { // We had actual text in there, treat the first '\n' as just text
            tok.type = Type::Text;
            m++;
        }
    }
    else if(streq(&S[tok.i], "\"")) { // Actual quote
        tok.type = Type::Quote;
        m += 1;
    } else { // Anything else is just a character
        tok.type = Type::Text;
        m++;
    }
    tok.j = m;
    return tok;
}

void outputPara(int q) { // Outputs a paragraph with q quotes
    int Q = q;
    for(int i = 0; i < t; i++) {
        if(T[i].type == Quote && (q > 1 || !(Q%2))) { // Fill in the quotes right
            strcpy(&M[r], (Q-q)&1 ? "''" : "``");
            r += 2, q--;
        } else if(T[i].type != Quote) // Everything else, output verbatim
            for(int c = T[i].i; c < T[i].j; c++)
                M[r++] = S[c];
    }
    M[r] = 0;
    printf("%s", M);
}

int main() {
    char c;
    while ((c = getchar()) != EOF)
        S[n++] = c;

    int q = 0;
    while(m <= n) {
        auto tok = readToken();
        T[t++] = tok;
        if(tok.type == Type::Quote)
            q++;
        if(tok.type == Type::Paragraph) {
            outputPara(q);
            q = t = r = 0;
        }
    }
}
