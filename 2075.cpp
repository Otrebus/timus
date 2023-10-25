/* 2075. Take care of your eyebrows! - http://acm.timus.ru/problem.aspx?num=2075
 *
 * Strategy:
 * We use a treap where the nodes are drinks keyed by drink density augmented by height up to the
 * current drink. Since the drink names are short enough, we can map them to 64-bit integers
 * (in a way that preserves their lexicographic ordering) to save a bit of performance.
 *
 * Performance:
 * O(nlog n), runs the tests in 0.796s using 18,532KB memory.
 */

#include <random>
#include <iostream>

using ll = long long;
const ll inf = 1e16;
const int maxn = 100000, maxl = 10, maxc = 'z'-'a'+2;

// To randomize the cartesian tree into a treap
std::random_device rd;
std::default_random_engine gen(rd());
std::uniform_int_distribution<ll> priodist(1, std::numeric_limits<ll>::max());

ll stringToLL(std::string& str) { // Maps a string to a ll
    ll result = 0;
    for(char c : str + std::string(maxl-str.length(), '`'))
        result = result * maxc + (c-'`');
    return result;
}

std::string llToString(ll v) { // Maps a ll to a string
    std::string result;
    do result = char('`' + v % maxc) + result; while((v /= maxc) > 0);
    return result.substr(0, result.find_last_not_of('`') + 1);
}

struct Node {
    ll prio, key;
    Node* left, *right;
    ll h, H; // Height, cumulate height (augmented value)
    Node(ll prio=0, ll key=0, ll h=0) : left(0), right(0), h(h), H(h), key(key), prio(prio) { };
} nodes[maxn];

struct Ans { // A sip
    ll t1, t2, d;
};

int num = 0;
Node* root;
bool drinking;


ll recalc(Node* node) {
    return node->H = node->h + (node->left ? node->left->H : 0) + (node->right ? node->right->H :0);
}


std::pair<Node*, Node*> split(Node* node, ll key) {
    // Splits a treap into two treaps greater and smaller than key
    if(!node)
        return { 0, 0 };
    if(key < node->key) {
        auto [lt, gt] = split(node->left, key);
        node->left = gt;
        recalc(node);
        return { lt, node };
    }
    else if(key >= node->key) {
        auto [lt, gt] = split(node->right, key);
        node->right = lt;
        recalc(node);
        return { node, gt };
    }
}

Node* merge(Node* a, Node* b) {
    // Merges treaps a and b, a's keys all lesser than b
    if(!a || !b)
        return a ? a : b;
    if(a->prio > b->prio) {
        b->left = merge(a, b->left);
        recalc(b);
        return b;
    }
    else {
        a->right = merge(a->right, b);
        recalc(a);
        return a;
    }
}

Node* insert(Node* node, ll key, ll h) {
    // Inserts drink with density key into the treap with h ml
    if(node && key == node->key) {
        node->h += h;
        recalc(node);
        return node;
    } else {
        auto [lt, gt] = split(node, key);
        nodes[num] = Node(priodist(gen), key, h);
        Node* n = &nodes[num++];
        return merge(lt, merge(n, gt));
    }
}

Node* remove(Node* node, ll h, ll h1, ll h2, std::vector<Ans>& ans) {
    // Removes drinks between height h1 and h2 (h keeps track of the current height as we recurse)
    if(!node)
        return node;
    ll hl = h + (node->left ? node->left->H : 0), hr = hl + node->h;
    if(h1 < hl) // Removal range is to the left
        node->left = remove(node->left, h, h1, h2, ans);
    if(h1 < hr && h2 > hl) { // Removal range includes the current node
        auto dh = std::min(h2, hr) - std::max(h1, hl);
        ans.push_back({ h, h+dh, node->key });
        node->h -= dh;
    }
    if(h2 > hr) // Removal range includes the right subtree
        node->right = remove(node->right, hr, h1, h2, ans);
    if(!node->h) // Current node was entirely removed
        return merge(node->left, node->right);
    recalc(node);
    return node;
}

void drink(ll h, ll t1, ll t2, std::vector<Ans>& ans) {
    // Drinks between times t1 and t2 from height h and adds the drinks drunk to the output
    ll L = ans.size();
    root = remove(root, 0, h, h+t2-t1, ans);
    for(ll i = L, t = t1; i < ans.size(); i++) {
        auto dt = ans[i].t2-ans[i].t1;
        ans[i].t1 = t;
        ans[i].t2 = t+dt;
        t += dt;
    }
}

int main() {
    int n;
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::string s;
    ll t, h, dh, lastT = 0;
    std::vector<Ans> ans;

    std::cin >> n;
    while(n--) {
        std::cin >> t >> s;
        if(drinking) {
            // Drink since the last timestamp
            ll rH = root->H;
            drink(dh, lastT, t, ans);
            if(dh + t - lastT > rH)
                drinking = false;
        }
        if(s == "Oleg") {
            std::cin >> dh;
            drinking = true;
        } else if(s == "fire") {
            drinking = false;
        } else {
            std::cin >> h;
            root = insert(root, stringToLL(s), h);
        }
        lastT = t;
    }
    if(drinking) // Drink up the last part
        drink(dh, lastT, inf, ans);

    std::vector<Ans> out(ans.begin(), ans.begin() + !ans.empty());
    for(int i = 1; i < ans.size(); i++) { // Merge output so we don't have same drinks consecutively
        auto& p = out.back(), a = ans[i];
        if(p.t2 == a.t1 && p.d == a.d)
            p.t2 = a.t2;
        else
            out.push_back(a);
    }

    for(auto a : out)
        std::cout << a.t1 << "-" << a.t2 << " " << llToString(a.d) << std::endl;
}
