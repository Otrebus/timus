/* 1231. Turing: One, Two, Three, ... - http://acm.timus.ru/problem.aspx?num=1231
 *
 * Strategy:
 * The algorithm is: scan to the left edge while counting the coordinate of the dash, storing it
 * in the state. Keep the coordinate in the state, and scan to the right side while noting if we
 * have more than two remaining dashes. If so, scan to the left again, move to the coordinate and
 * mark it as a plus, move to the right skipping k dashes (possibly restarting from the left if
 * need be) and then repeat. If there is only one dash left, move to it and end.
 *
 * Performance:
 * N/A.
 */

#include <stdio.h>
#include <vector>

struct Row {
    int instate, outstate;
    char input, output, direction;
    void print() {
        printf("%d %c %d %c %c\n", instate, input, outstate, output, direction);
    }
};

std::vector<Row> rows;

enum State {
    coord = 1,     // Counting the coordinate
    count = 2,     // Counting dashes
    start = 3,     // Moving to the start after having counted
    move = 4,      // Moving to the dash
    seek = 5,      // Seeking the next dash to mark
    seekstart = 6, // Wrapping around while seeking the next dash
    seeklast = 7   // Seeking the last dash to finish at
};

int k;

// Creates a row of the state transition table encoding an action and some state (x and y)
void make_row(State in, int inx, int iny, char symbol, State out,
              int outx, int outy, char outsymbol, char direction) {
    rows.push_back(Row { in | iny << 3 | inx << 5, out | outy << 3 | outx << 5,
                         symbol, outsymbol, direction });
}

const int N = 200;

int main() {
    scanf("%d", &k);

    // Count the coordinate of the dash while moving left
    for(int x = 0; x <= N; x++) {
        make_row(coord, x, 0, '-', coord, x+1, 0, '-', '<');
        make_row(coord, x, 0, '+', coord, x+1, 0, '+', '<');
        make_row(coord, x, 0, '#', count, x, 0, '#', '>');
    }

    // Scan to the right and check if we have enough dashes
    for(int x = 0; x <= N; x++) {
        for(int y = 0; y <= 2; y++) {
            make_row(count, x, y, '-', count, x, std::min(2, y+1), '-', '>');
            make_row(count, x, y, '+', count, x, y, '+', '>');
        }
    }

    // If a single dash is left, move to the last dash
    for(int x = 0; x <= N; x++) {
        make_row(count, x, 1, '#', seeklast, 0, 0, '#', '<');
        make_row(count, x, 2, '#', start, x, 0, '#', '<');
    }

    // Move to the start
    for(int x = 1; x <= N; x++) {
        make_row(start, x, 0, '-', start, x, 0, '-', '<');
        make_row(start, x, 0, '+', start, x, 0, '+', '<');
        make_row(start, x, 0, '#', move, x-1, 0, '#', '>');
    }

    // Move to the the dash we started at
    for(int x = 1; x <= N; x++) {
        make_row(move, x, 0, '-', move, x-1, 0, '-', '>');
        make_row(move, x, 0, '+', move, x-1, 0, '+', '>');
    }

    // Seek out the next dash to mark
    make_row(move, 0, 0, '-', seek, 1, 0, '-', '=');
    make_row(move, 0, 0, '+', seek, 1, 0, '+', '=');
    for(int x = 1; x < k; x++) {
        make_row(seek, x, 0, '-', seek, x+1, 0, '-', '>');
        make_row(seek, x, 0, '+', seek, x, 0, '+', '>');
    }

    // If we find the dash, repeat
    make_row(seek, k, 0, '-', coord, 0, 0, '+', '=');
    make_row(seek, k, 0, '+', seek, k, 0, '+', '>');

    // If we meet a #, move to the start and keep seeking for the next dash
    for(int x = 0; x <= k; x++)
        make_row(seek, x, 0, '#', seekstart, x, 0, '#', '<');

    for(int x = 0; x <= k; x++) {
        make_row(seekstart, x, 0, '-', seekstart, x, 0, '-', '<');
        make_row(seekstart, x, 0, '+', seekstart, x, 0, '+', '<');
    }

    for(int x = 0; x <= k; x++)
        make_row(seekstart, x, 0, '#', seek, x, 0, '#', '>');

    make_row(seeklast, 0, 0, '+', seeklast, 0, 0, '+', '<');
    make_row(seeklast, 0, 0, '-', seeklast, 1, 0, '-', '=');

    printf("%lld\n", rows.size());
    for(auto row : rows)
        row.print();
}
