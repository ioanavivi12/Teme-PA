#include <bits/stdc++.h>
#define array vector<int>

using namespace std;

vector<array> adj;
array found;
array low_link;
array visited;
array is_in_stack;
array parent;
int timestamp = 0;
int all_sccs = 1;
array scc;
stack<int> nodes;
vector<array> parents;
vector<array> new_gr_adj;

/*
    Dfs to find the SCCs
*/
void dfs_scc(int node) {
        timestamp++;
        found[node] = timestamp;
        low_link[node] = timestamp;

        nodes.push(node);
        is_in_stack[node] = 1;
        for (int succ : adj[node]) {
            if (visited[succ] == 1)
                continue;
            if (parent[succ] != -1) {
                if (is_in_stack[succ] == 1)
                    low_link[node] = min(found[succ], low_link[node]);

                continue;
            }

            parent[succ] = node;
            dfs_scc(succ);
            low_link[node] = min(low_link[succ], low_link[node]);
        }

        if (low_link[node] == found[node]) {
            int x;
            do {
                x = nodes.top();
                scc[x] = all_sccs;
                is_in_stack[x] = 0;
                nodes.pop();
            }while (x != node);
            all_sccs++;
        }
    }

/*
    Dfs to find the nodes direct reachable from the source
*/
void basic_dfs(int node) {
    visited[node] = 1;
    for (int succ : adj[node]) {
        if (visited[succ] == 0) {
            basic_dfs(succ);
        }
    }
}


/*
    Dfs to find the minimum number of ways to reach all the SCCs
*/
void get_all_ways(int node, bool skip_parents) {
    visited[node] = 1;
    if (skip_parents == false) {
        for (int parent : parents[node]) {
            if (visited[parent] == 0) {
                get_all_ways(parent, false);
                break;
            }
        }
    }

    for (int succ : new_gr_adj[node]) {
        if (visited[succ] == 1)
            continue;
        get_all_ways(succ, true);
    }
}

void free_memory();
void Tarjan();

int main() {
    ifstream fin("ferate.in");
    ofstream fout("ferate.out");

    /*
        read input
    */
    int n, m, source;
    fin >> n >> m >> source;

    /*
        create the adjacency list
    */
    adj.resize(n + 1);
    for (int i = 1; i <= m; i++) {
        int x, y;
        fin >> x >> y;
        adj[x].push_back(y);
    }

    /*
        try to exclude all the nodes direct reachable from the source
    */
    visited.resize(n + 1, 0);
    basic_dfs(source);

    Tarjan();

    /*
        create the new graph with the SCCs as nodes
    */
    parents.resize(all_sccs);
    new_gr_adj.resize(all_sccs);
    for (int i = 1; i <= n; i++) {
        int j = 0;
        if (scc[i] == -1)
            continue;
        while (j < (int)adj[i].size()) {
            if (scc[adj[i][j]] == -1) {
                j++;
                continue;
            }
            if (scc[i] != scc[adj[i][j]]) {
                int x = scc[i];
                int y = scc[adj[i][j]];
                if (find(new_gr_adj[x].begin(), new_gr_adj[x].end(), y)
                        == new_gr_adj[x].end()) {
                    new_gr_adj[x].push_back(y);
                    parents[y].push_back(x);
                }
            }
            j++;
        }
    }


    /*
        find the minimum number of ways to reach all the SCCs
    */
    int count = 0;
    visited.clear();
    visited.resize(all_sccs, 0);

    for (int i = 1; i < all_sccs; i++) {
        if (!visited[i]) {
            get_all_ways(i, false);
            count++;
        }
    }

    fout << count << "\n";

    fin.close();
    fout.close();
    return 0;
}

void Tarjan() {
    /*
        apply Tarjan's algorithm to find the SCCs
    */
    int n = (int)adj.size() - 1;
    found.resize(n + 1);
    low_link.resize(n + 1);
    scc.resize(n + 1);
    is_in_stack.resize(n + 1, 0);
    parent.resize(n + 1, -1);

    all_sccs = 1;
    for (int i = 1; i <= n; i++) {
        if (parent[i] == -1 && visited[i] == 0) {
            parent[i] = i;
            dfs_scc(i);
        } else if (visited[i] == 1) {
            scc[i] = -1;
        }
    }
}
