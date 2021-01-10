/* 1147. Shaping Regions - http://acm.timus.ru/problem.aspx?num=1147
 *
 * Strategy:
 * Sort the y coordinates of the horizontal segments of all regions (here called patches), step
 * through them one by one, and for every vertical band (formed by a y coordinate and its
 * predecessor) go through all the horizontal parts of the regions from left to right and
 * insert/remove the corresponding region into a priority queue ordered by their input order as
 * we pass from segment to segment adding the area bound by the y coordinates and the horizontal
 * segments.
 * 
 * Performance:
 * O(N^2 log N), runs the tests in 0.109s using 400KB.
 */

#include <stdio.h>
#include <vector>
#include <algorithm>
#include <set>

struct Segment { // Vertical segments
    int x, y1, y2;
    bool begin; // Left part of a segment?
    int color;
    int c; // Patch this belongs to
};

struct Patch { // Heap representation of a patch
    int c, p;
};

const int MAX_C = 2500;
const int MAX_N = 1000;

Patch H[MAX_N+2]; // The heap of the priority queue
int J[MAX_N+1]; // The position in the heap of a patch
int S[MAX_C+1]; // The output answer
int R[MAX_N+1]; // The color of each patch
int N; // Number of patches


void heapSwap(int a, int b) {
    std::swap(H[a], H[b]);
    std::swap(J[H[a].c], J[H[b].c]);
}

void increaseKey(int c) {
    // Increases the key of the given patch to its index
    H[J[c]].p = c;
    for(int j = J[c]; j/2 && c > H[j/2].p; j = j/2)
        heapSwap(j, j/2);
}

void decreaseKey(int c) {
    // Decreases the key of the given patch to -1
    int j = J[c];
    H[j].p = -1;
    for(int m = j*2; m <= N; j = m, m = j*2) {
        if(m < N && H[m+1].p > H[m].p)
            m++;
        if(H[m].p == -1)
            break;
        heapSwap(j, m);
    }
}

int main() {
    int A, B;
    std::vector<Segment> v;
    std::set<int> ys;
    
    scanf("%d %d %d", &A, &B, &N);

    v.push_back( { 0, 0, B, true, 1, 0 } );
    v.push_back( { A, 0, B, false, 1, 0 } );
    ys.insert( { 0, B } );
    R[0] = 1;
    N++;

    for(int i = 0; i < N; i++) {
        H[i+1] = { i, -1 };
        J[i] = i+1;
    }

    for(int i = 1; i < N; i++) {
        int llx, lly, urx, ury, color;

        scanf("%d %d %d %d %d", &llx, &lly, &urx, &ury, &color);

        v.push_back( { llx, lly, ury, true, color, i });
        v.push_back( { urx, lly, ury, false, color, i });
        ys.insert( { lly, ury } );
        R[i] = color;
    }

    // Sort all vertical segments
    std::sort(v.begin(), v.end(), [] (auto& a, auto& b) {
        return a.x == b.x && a.begin > b.begin || a.x < b.x;
    });
    
    // For each vertical coordinate
    for(auto it = std::next(ys.begin()); it != ys.end(); it++) {
        int y2 = *it;
        int y1 = *std::prev(it); // For the vertical band

        for(auto it2 = v.begin(), itprev = v.end(); it2 < v.end(); it2++) {
            if(it2->y1 >= y2 || it2->y2 <= y1) // If a patch is outside this band, skip
                continue;

            if(itprev != v.end())
                // Add the area to the color of the patch on the top of the heap
                S[R[H[1].c]] += (y2-y1)*(it2->x-itprev->x);

            // Increase or decrease the key of the current region depending if we enter or leave it
            (it2->begin ? increaseKey : decreaseKey)(it2->c);
            itprev = it2;
        }
    }

    for(int i = 1; i <= MAX_C; i++)
        if(S[i])
            printf("%d %d\n", i, S[i]);
}

// Version using a quadtree, but this would have been too slow
//#include <iostream>
//
//struct node {
//    int color;
//    int x1, y1, x2, y2;
//    node* children[4];
//};
//
//int result[2501];
//
//void createNode(node* n, int i, int x1, int y1, int x2, int y2, int c) {
//    if(x1 != n->x1 || y1 != n->y1 || x2 != n->x2 || y2 != n->y2)
//        n->children[i] = new node { c, x1, y1, x2, y2, { 0, 0, 0, 0 } };
//}
//
//void fillNode(node* n, int llx, int lly, int urx, int ury, int color) {
//
//    // If this node is degenerate, ignore
//    if((urx-llx)*(ury-lly) == 0 || (n->y2-n->y1)*(n->x2-n->x1) == 0)
//        return;
//
//    // If the rectangle is outside the bounds of this node, ignore
//    if(llx >= n->x2 || urx <= n->x1 || lly >= n->y2 || ury <= n->y1)
//        return;
//
//    // If the rectangle covers the entire node, make it a leaf
//    if(llx <= n->x1 && lly <= n->y1 && urx >= n->x2 && ury >= n->y2) {
//        n->color = color;
//    }
//
//    // Else split this node
//    else {
//        if(n->color) {
//            int mx = (n->x2 + n->x1)/2;
//            int my = (n->y2 + n->y1)/2;
//
//            createNode(n, 0, mx, my, n->x2, n->y2, n->color);
//            createNode(n, 1, n->x1, my, mx, n->y2, n->color);
//            createNode(n, 2, n->x1, n->y1, mx, my, n->color);
//            createNode(n, 3, mx, n->y1, n->x2, my, n->color);
//
//            n->color = 0;
//        }
//
//        for(auto c : n->children)
//            if(c)
//                fillNode(c, llx, lly, urx, ury, color);
//    }
//}
//
//void countNode(node* n) {
//    if(!n)
//        return;
//    if(n->color)
//        result[n->color] += (n->y2-n->y1)*(n->x2-n->x1);
//    else
//        for(auto c : n->children)
//            countNode(c);
//}
//
//int main() {
//    std::ios::sync_with_stdio(false);
//    int A, B, N;
//    std::cin >> A >> B >> N;
//    node* n = new node { 1, 0, 0, A, B, { 0, 0, 0, 0 } };
//
//    for(int i = 0; i < N; i++) {
//        int llx, lly, urx, ury, color;
//        std::cin >> llx >> lly >> urx >> ury >> color;
//        fillNode(n, llx, lly, urx, ury, color);
//    }
//    countNode(n);
//    for(int i = 1; i <= 2500; i++)
//        if(result[i])
//            std::cout << i << " " << result[i] << std::endl;
//}

