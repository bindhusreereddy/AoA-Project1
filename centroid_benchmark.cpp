// centroid_benchmark.cpp
// g++ -O2 -std=c++17 centroid_benchmark.cpp -o centroid_benchmark
#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>
#include <random>
using namespace std;
using ll = long long;

struct Edge {int to; ll w;};
vector<vector<Edge>> g;
vector<int> sz;
vector<char> removed;
ll K, answer;

// ---------- Subroutines ----------
int dfs_size(int u, int p) {
    sz[u] = 1;
    for (auto &e : g[u])
        if (e.to != p && !removed[e.to])
            sz[u] += dfs_size(e.to, u);
    return sz[u];
}

int find_centroid(int u, int p, int total) {
    for (auto &e : g[u])
        if (e.to != p && !removed[e.to] && sz[e.to] > total / 2)
            return find_centroid(e.to, u, total);
    return u;
}

void dfs_collect(int u, int p, ll dist, vector<ll>& out) {
    if (dist > K) return;
    out.push_back(dist);
    for (auto &e : g[u])
        if (e.to != p && !removed[e.to])
            dfs_collect(e.to, u, dist + e.w, out);
}

ll count_cross(const vector<ll>& A, const vector<ll>& B) {
    ll cnt = 0;
    for (ll d : A) {
        ll limit = K - d;
        cnt += upper_bound(B.begin(), B.end(), limit) - B.begin();
    }
    return cnt;
}

vector<ll> merge_sorted(const vector<ll>& A, const vector<ll>& B) {
    vector<ll> out; out.reserve(A.size()+B.size());
    merge(A.begin(), A.end(), B.begin(), B.end(), back_inserter(out));
    return out;
}

// ---------- Divide & Conquer ----------
void decompose(int entry) {
    int total = dfs_size(entry, -1);
    int c = find_centroid(entry, -1, total);
    removed[c] = 1;

    vector<ll> acc = {0};
    for (auto &e : g[c]) if (!removed[e.to]) {
        vector<ll> sub;
        dfs_collect(e.to, c, e.w, sub);
        sort(sub.begin(), sub.end());
        answer += count_cross(sub, acc);
        acc = merge_sorted(acc, sub);
    }
    for (auto &e : g[c]) if (!removed[e.to])
        decompose(e.to);
}

// ---------- Random Tree Generator ----------
vector<vector<Edge>> random_tree(int n, int max_w=20) {
    mt19937 rng(42);
    uniform_int_distribution<int> W(1, max_w);
    vector<vector<Edge>> G(n);
    for (int v = 1; v < n; ++v) {
        int u = rng() % v;
        int w = W(rng);
        G[u].push_back({v, w});
        G[v].push_back({u, w});
    }
    return G;
}

// ---------- Benchmark Main ----------
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    cout << "n_nodes,time_ms\n";

    for (int n : {200, 400, 800, 1200, 1600, 2200, 3000}) {
        for (int t = 0; t < 3; ++t) {
            g = random_tree(n);
            sz.assign(n, 0);
            removed.assign(n, 0);
            K = 60;  // distance threshold
            answer = 0;

            auto start = chrono::steady_clock::now();
            decompose(0);
            auto end = chrono::steady_clock::now();

            double ms = chrono::duration<double, milli>(end - start).count();
            cout << n << "," << ms << "\n";
        }
    }
}