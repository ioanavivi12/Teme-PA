#include <bits/stdc++.h>
#define array vector<int>

using namespace std;

/*
    copy the queues
*/
void copy_queue(queue<int> *q, queue<int> *aux_q) {
    aux_q[1] = q[1];
    aux_q[2] = q[2];
}


/*
    copy the dependencies
*/
void copy_dependencies(array dependencies, array *aux_dependencies) {
    for (int i = 1; i < (int)dependencies.size(); i++) {
        (*aux_dependencies)[i] = dependencies[i];
    }
}

int minim_switches(array v, vector<array> adj, array dependencies,
                    queue<int> *q, int type) {
    int context_switches = 0;
    while (!q[1].empty() || !q[2].empty()) {
        /*
            if the queue is empty, we need to switch the context
        */
        if (q[type].empty()) {
            context_switches++;
            type = 3 - type;
        }

        int node = q[type].front();
        q[type].pop();

        for (int succ : adj[node]) {
            dependencies[succ]--;
            if (dependencies[succ]== 0) {
                q[v[succ]].push(succ);
            }
        }
    }

    return context_switches;
}

int main() {
    ifstream fin("supercomputer.in");
    ofstream fout("supercomputer.out");

    /*
        read input
    */
    int n, m;
    fin >> n >> m;
    array v(n + 1);
    for (int i = 1; i <= n; i++) {
        fin >> v[i];
    }

    vector<array> adj(n + 1);
    array dependencies(n + 1, 0);
    for (int i = 0; i < m; i++) {
        int a, b;
        fin >> a >> b;
        adj[a].push_back(b);
        dependencies[b]++;
    }

    /*
        initialize queues
    */
    queue<int> q[3];
    for (int i = 1; i <= n; i++)
        if (dependencies[i] == 0)
            q[v[i]].push(i);

    int ans1 = 0, ans2 = 0;

    queue<int> aux_q[3];
    copy_queue(q, aux_q);

    array aux_dependencies(n + 1, 0);
    copy_dependencies(dependencies, &aux_dependencies);
    /*
        compute the answer for the first type
    */
    ans1 = minim_switches(v, adj, dependencies, q, 1);
    /*
        compute the answer for the second type
    */
    ans2 = minim_switches(v, adj, aux_dependencies, aux_q, 2);

    fout << min(ans1, ans2) << "\n";

    /*
        free memory
    */
    for (int i = 1; i <= n; i++)
        adj[i].clear();

    adj.clear();
    dependencies.clear();
    aux_dependencies.clear();
    v.clear();

    fin.close();
    fout.close();
    return 0;
}
