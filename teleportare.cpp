#include <bits/stdc++.h>
#define array vector<int>
#define ll unsigned long long

using namespace std;

vector<vector<pair<int, ll>>> adj;
vector<vector<pair<int, int>>> extra_adj;
vector<vector<ll>> dist;
int n;

int cmmdc(int a, int b) {
    if (b == 0)
        return a;
    return cmmdc(b, a % b);
}

void cmmmc(int &a, int b) {
    a = a * b / cmmdc(a, b);
}

ll dijkstra(int node, int modul) {
    priority_queue<pair<ll, int>,
                   vector<pair<ll, int>>, greater<pair<ll, int>>> q;
    q.push({0, node});
    dist[0][node] = 0;
    while (!q.empty()) {
        int u = q.top().second;
        ll d = q.top().first;
        q.pop();
        for (auto v : adj[u]) {
            ll cost = d + v.second;
            if (dist[cost % modul][v.first] > dist[d % modul][u] + v.second) {
                dist[cost % modul][v.first] = dist[d % modul][u] + v.second;
                q.push({dist[d % modul][u] + v.second, v.first});
            }
        }

        for (auto v : extra_adj[u]) {
            int p = v.second;
            if (d % p != 0)
                continue;
            ll cost = d + 1;
            if (dist[cost % modul][v.first] > dist[d % modul][u] + 1) {
                dist[cost % modul][v.first] = dist[d % modul][u] + 1;
                q.push({dist[d % modul][u] + 1, v.first});
            }
        }
    }

    ll ans = 1e13;
    for (int i = 0; i < modul; i++)
        ans = min(ans, dist[i][n]);
    return ans;
}

int main() {
    ifstream fin("teleportare.in");
    ofstream fout("teleportare.out");

    int m, k;
    fin >> n >> m >> k;

    adj.resize(n + 1);
    for (int i = 0; i < m; i++) {
        int x, y;
        ll c;
        fin >> x >> y >> c;
        adj[x].push_back({y, c});
        adj[y].push_back({x, c});
    }

    extra_adj.resize(n + 1);
    vector<bool> exist_p(9, false);
    for (int i = 0; i < k; i++) {
        int x, y, p;
        fin >> x >> y >> p;
        exist_p[p] = true;
        extra_adj[x].push_back({y, p});
        extra_adj[y].push_back({x, p});
    }

    int modul = 1;
    for (int i = 1; i < 9; i++)
        if (exist_p[i])
            cmmmc(modul, i);

    dist.resize(modul, vector<ll>(n + 1, 1e13));

    fout << dijkstra(1, modul) << '\n';
    return 0;
}
