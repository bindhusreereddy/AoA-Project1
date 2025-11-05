// count_pairs_within_K_tree.cpp
// Count unordered pairs of nodes with tree-distance <= K on a weighted tree.
// Approach: Divide & Conquer via centroid decomposition.
// Build:   g++ -O2 -std=c++17 count_pairs_within_K_tree.cpp -o countK
// Run:     ./countK < input.txt
//
// Input:
//   n K
//   u1 v1 w1
//   ...
//   u_{n-1} v_{n-1} w_{n-1}
//
// Output:
//   number of unordered pairs {u, v} with dist(u, v) <= K

#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>
#include <random>
using namespace std;

using ll = long long;

struct Edge { int to; ll w; };

int n;
ll K;
vector<vector<Edge>> g;
vector<int> sz;
vector<char> removed;
ll answer = 0;

// --- compute subtree sizes (for centroid finding) ---
void dfs_size(int u, int p) {
    sz[u] = 1;
    for (auto &e : g[u]) {
        int v = e.to;
        if (v == p || removed[v]) continue;
        dfs_size(v, u);
        sz[u] += sz[v];
    }
}

// --- find centroid of current component ---
int find_centroid(int u, int p, int total) {
    for (auto &e : g[u]) {
        int v = e.to;
        if (v == p || removed[v]) continue;
        if (sz[v] > total / 2) return find_centroid(v, u, total);
    }
    return u;
}

// --- collect distances from centroid into a vector (prune if > K) ---
void dfs_collect(int u, int p, ll dist, vector<ll>& out) {
    if (dist > K) return;
    out.push_back(dist);
    for (auto &e : g[u]) {
        int v = e.to;
        if (v == p || removed[v]) continue;
        dfs_collect(v, u, dist + e.w, out);
    }
}

// Count pairs between current subtree distances A and accumulated list All (sorted).
static inline ll count_cross(const vector<ll>& A, const vector<ll>& All) {
    ll cnt = 0;
    for (ll d : A) {
        ll limit = K - d;
        auto it = upper_bound(All.begin(), All.end(), limit);
        cnt += (ll)(it - All.begin());
    }
    return cnt;
}

// Merge sorted vectors (in-place replace All with merged).
static inline void merge_into(vector<ll>& All, const vector<ll>& A) {
    vector<ll> merged;
    merged.reserve(All.size() + A.size());
    merge(All.begin(), All.end(), A.begin(), A.end(), back_inserter(merged));
    All.swap(merged);
}

// --- centroid decomposition driver on component rooted at entry ---
void decompose(int entry) {
    dfs_size(entry, -1);
    int c = find_centroid(entry, -1, sz[entry]);

    removed[c] = 1;

    // Count pairs "through" centroid c
    vector<ll> acc;  // distances already aggregated from previous child subtrees
    acc.reserve(sz[entry]);
    acc.push_back(0);  // distance from c to itself (pairs involving c)

    for (auto &e : g[c]) {
        int v = e.to;
        if (removed[v]) continue;

        vector<ll> sub;
        sub.reserve(sz[v]);
        dfs_collect(v, c, e.w, sub);     // distances from c into this subtree
        sort(sub.begin(), sub.end());

        // count cross pairs: this subtree with all previously processed ones (and the centroid)
        answer += count_cross(sub, acc);

        // accumulate distances for next children
        merge_into(acc, sub);
    }

    // Recurse on each remaining component
    for (auto &e : g[c]) {
        int v = e.to;
        if (!removed[v]) decompose(v);
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    if (!(cin >> n >> K)) {
        cerr << "Invalid input\n";
        return 1;
    }
    
    cout << "Input received: n=" << n << ", K=" << K << endl;

    g.assign(n + 1, {});
    for (int i = 0; i < n - 1; ++i) {
        int u, v; ll w;
        cin >> u >> v >> w;
        g[u].push_back({v, w});
        g[v].push_back({u, w});
    }

    sz.assign(n + 1, 0);
    removed.assign(n + 1, 0);

    decompose(1);
    cout << "Total pairs with distance <= " << K << " is: " << answer << "\n";
    return 0;
}
