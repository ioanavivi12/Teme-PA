#include <bits/stdc++.h>
#define array vector<int>

using namespace std;

vector<array> adj;
array nodes_ordered;
int *number_nodes;

void get_ordered_nodes(int node) {
    nodes_ordered.push_back(node);
    number_nodes[node] = 1;

    int nr = adj[node].size();
    for (int i = 0; i < nr; i++) {
        get_ordered_nodes(adj[node][i]);
        number_nodes[node] += number_nodes[adj[node][i]];
    }
}

int main() {
    ifstream fin("magazin.in");
    ofstream fout("magazin.out");

    int n, q, x;
    fin >> n >> q;

    adj.resize(n + 1);
    for (int i = 2; i <= n; i++) {
        fin >> x;
        adj[x].push_back(i);
    }

    number_nodes = (int *)calloc((n + 1), sizeof(int));
    if (number_nodes == NULL) {
        perror("Error allocating memory");
        exit(EXIT_FAILURE);
    }

    nodes_ordered.push_back(0);
    get_ordered_nodes(1);

    array positions(n + 1);
    for (int i = 1; i <= n; i++)
        positions[nodes_ordered[i]] = i;

    for (int i = 0; i < q; i++) {
        int node, k;
        fin >> node >> k;
        if (k >= number_nodes[node]) {
            fout << "-1\n";
            continue;
        }
        fout << nodes_ordered[positions[node] + k] << "\n";
    }

    for (int i = 0; i <= n; i++)
        adj[i].clear();

    adj.clear();
    nodes_ordered.clear();
    free(number_nodes);

    fin.close();
    fout.close();
    return 0;
}
