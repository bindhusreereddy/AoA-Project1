// greedy_benchmark.cpp
// g++ -O2 -std=c++17 greedy_benchmark.cpp -o greedy_benchmark
#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>
#include <random>
using namespace std;

// -----------------------------
// Algorithm: maxBoxesGreedy
// -----------------------------
int maxBoxesGreedy(vector<int>& boxes, const vector<int>& warehouse) {
    int m = warehouse.size();
    vector<int> effective(m);
    effective[0] = warehouse[0];
    for (int i = 1; i < m; ++i)
        effective[i] = min(effective[i - 1], warehouse[i]);

    sort(boxes.begin(), boxes.end());
    int i = m - 1, j = 0, placed = 0;
    while (i >= 0 && j < (int)boxes.size()) {
        if (boxes[j] <= effective[i]) {
            placed++; j++; i--;
        } else {
            i--;
        }
    }
    return placed;
}

// -----------------------------
// Experimental harness
// -----------------------------
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    mt19937 rng(42); // fixed seed for reproducibility
    uniform_int_distribution<int> H(1, 10000);

    cout << "n_boxes,m_rooms,time_ms\n";

    for (int n : {200, 400, 800, 1600, 3200, 6400, 12800}) {
        int m = n; // same number of rooms as boxes
        for (int t = 0; t < 5; ++t) { // multiple trials for averaging
            vector<int> boxes(n), warehouse(m);
            for (int &b : boxes) b = H(rng);
            for (int &h : warehouse) h = H(rng);

            auto start = chrono::steady_clock::now();
            int placed = maxBoxesGreedy(boxes, warehouse);
            auto end = chrono::steady_clock::now();

            double ms = chrono::duration<double, milli>(end - start).count();
            cout << n << "," << m << "," << ms << "\n";
        }
    }
}