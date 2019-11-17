#include <bits/stdc++.h>
#include "graphAlgorithm.h"

edge::edge(int from, int to, long long cost) :
    from(from),
    to(to),
    cost(cost)
    {}

 edge::edge(){}

MinCut::MinCut(int n)
    : n(n), g(n + 1), v(n + 1), d(n + 1), vis(n + 1) {
        for(int i = 1; i <= n; i++) g[i].resize(n + 1);
    }
void MinCut::add(int u, int v, int c) {
    g[u][v] += c;
    g[v][u] += c;
}
int MinCut::solve() {

    int res = INF;
    for (int i = 1; i <= n; i++) v[i] = i, vis[i] = 0;

    while (n > 1) {
        int p = 2, prev = 1;
        for (int i = 2; i <= n; ++i) {
            d[v[i]] = g[v[1]][v[i]];
            if (d[v[i]] > d[v[p]])
            p = i;
        }

        vis[v[1]] = n;
        for (int i = 2; i <= n; ++i) {
            if (i == n) {
                res = std::min(res, d[v[p]]);
                for (int j = 1; j <= n; ++j) {
                    g[v[prev]][v[j]] += g[v[p]][v[j]];
                    g[v[j]][v[prev]] = g[v[prev]][v[j]];
                }
                v[p] = v[n--];
                break;
            }

            vis[v[p]] = n;
            prev = p;
            p = -1;
            for (int j = 2; j <= n; ++j) {
                if (vis[v[j]] != n) {
                    d[v[j]] += g[v[prev]][v[j]];
                    if (p == -1 || d[v[p]] < d[v[j]])
                    p = j;
                }
            }
        }
    }
    return res;
}

bool DinicMaxFlow::bfs() {

    fill(d.begin(), d.end(), -1);
    std::queue<int> q;
    d[s] = 0;

    for (q.push(s); !q.empty(); q.pop()) {
        int a = q.front();
        for (int i = 0; i < g[a].size(); i++) {
            const Edge& e = g[a][i];
            int b = e.to;
            if (d[b] < 0 && e.flow < e.cap) {
            d[b] = d[a] + 1;
            q.push(b);
            }
        }
    }

    return d[t] >= 0;
}
int DinicMaxFlow::dfs(int a, int flow) {

    if (!flow || a == t) return flow;

    for (int& i = it[a]; i < g[a].size(); i++) {
        Edge& e = g[a][i];
        Edge& r = g[e.to][e.rev];

        int b = e.to;
        if (d[b] != d[a] + 1) continue;

        int delta = dfs(b, std::min(flow, e.cap - e.flow));
        if (delta > 0) {
            e.flow += delta;
            r.flow -= delta;
            return delta;
        }
    }
    return 0;
}

DinicMaxFlow::DinicMaxFlow(int n, int s, int t)
: n(n), s(s), t(t), g(n), d(n), it(n) {}

void DinicMaxFlow::add(int a, int b, int cap) {
    Edge e1 = {b, int(g[b].size()), cap, 0};
    Edge e2 = {a, int(g[a].size()), 0, 0};
    g[a].push_back(e1);
    g[b].push_back(e2);
}

std::pair <int, std::vector <edge> > DinicMaxFlow::get() {
    int flow = 0;
    while (bfs()) {
        fill(it.begin(), it.end(), 0);
        while (int delta = dfs(s, 1000000007))
        flow += delta;
    }

    std::vector <edge> v;
    for (int i = 0; i < g.size(); ++i) {
        for (int j = 0; j < g[i].size(); ++j){
            v.push_back({i, g[i][j].to, g[i][j].flow});
        }
    }

    return {flow, v};
}

Graph::Graph(){}

Graph::Graph(int size_) : size(size_) {
    edgesCount = 0;
    procEdges = 0;
    logSize = 1;
    edges.resize(size);

    up.resize(size + 1);
    while((1 << logSize) <= size) ++logSize;
    for (int i = 0; i < up.size(); ++i) up[i].resize(logSize + 1);

    tin.resize(size + 1);
    tout.resize(size + 1);
}

void Graph::clearGraph(){
    size = 0;
    edgesCount = 0;
    procEdges = 0;

    tin.clear();
    tout.clear();
    edges.clear();
}

void Graph::resizeGraph(int size_){
    size = size_;
    edges.resize(size_);
    edgesCount = 0, procEdges = 0;
    logSize = 1;
    edges.resize(size);

    up.resize(size + 1);
    while((1 << logSize) <= size) ++logSize;
    for (int i = 0; i < up.size(); ++i) up[i].resize(logSize + 1);

    tin.resize(size + 1);
    tout.resize(size + 1);
}

int Graph::lca(int a, int b) {
    if (edgesCount > procEdges) {
        int timer = 0;
        std::function<void(int, int)> dfs = [&](int v, int p){
            tin[v] = ++timer;
            up[v][0] = p;
            for (int i = 1; i <= logSize; ++i) {
                up[v][i] = up[up[v][i - 1]][i - 1];
            }

            for (auto e : edges[v]) if (e.to != p) {
                dfs(e.to, v);
            }
            tout[v] = timer;
        };
        dfs(0, 0);
        procEdges = edgesCount;
    }

    std::function<bool(int, int)> upper = [&](int a, int b){
        return (tin[a] <= tin[b] && tout[b] <= tout[a]);
    };

    std::function<int(int, int)> calcLca = [&](int a, int b){
        if (upper(a, b)) return a;
        if (upper(b, a)) return b;
        for (int i = logSize; i >= 0; --i) if (!upper(up[a][i], b)) a = up[a][i];
        return up[a][0];
    };
    return calcLca(a, b);
}

void Graph::addEdge(int from, int to, long long cost) {
    edgesCount++;
    edges[from - 1].push_back({from - 1, to - 1, cost});
}

std::vector <edge> Graph::findBridges() {
    std::vector <edge> answer;
    int timer = 0;
    std::vector <bool> used(size, 0);
    std::vector <int> tin(size, 0), fup(size, 0);

    std::function<void(int, int)> dfs = [&](int v, int p){
        used[v] = 1;
        tin[v] = fup[v] = timer++;
        for (auto e : edges[v]) if (e.to != p) {
            int to = e.to;
            if (used[to]) fup[v] = std::min(fup[v], tin[to]);
            else {
                dfs(to, v);
                fup[v] = std::min(fup[v], fup[to]);
                if (fup[to] > tin[v]) answer.push_back(e);
            }
        }
    };

    for (int i = 0; i < size; ++i) if (!used[i]) dfs(i, -1);

    return answer;
}

std::pair <std::vector <long long>, std::vector <int> > Graph::djikstra(int start) {
    std::vector <long long> d(size, INT64_MAX);
    std::vector <int> p(size, -1);
    std::set <std::pair <long long, int> > G;

    d[start] = 0;
    G.insert(std::make_pair(d[start], start));

    while(!G.empty()) {
        int v = G.begin() -> second;
        G.erase(G.begin());

        for (auto e : edges[v]) {
            if (d[v] != INT64_MAX && d[v] + e.cost < d[e.to]) {
                G.erase(std::make_pair(d[e.to], e.to));
                d[e.to] = d[v] + e.cost;
                p[e.to] = v;
                G.insert(std::make_pair(d[e.to], e.to));
            }
        }
    }

    return {d, p};
}

std::pair <long long, std::vector <edge> > Graph::kruskal() {
    std::vector <edge> g, res;
    std::vector <int> p(size);
    long long cost = 0;

    std::function<int(int)> dsuGet = [&](int v){
        if (p[v] != v) p[v] = dsuGet(p[v]);
        return p[v];
    };

    for (int i = 0; i < size; ++i) {
        p[i] = i;
        for (auto e : edges[i]) g.push_back(e);
    }
    sort(g.begin(), g.end(), [](edge a, edge b) {return (a.cost < b.cost);});

    for (auto e : g) {
        if (dsuGet(e.to) != dsuGet(e.from)) {
            cost += e.cost;
            res.push_back(e);
            p[dsuGet(e.to)] = dsuGet(e.from);
        }
    }
    return std::make_pair(cost, res);
}

std::pair <int, std::vector <edge> > Graph::maxFlow(int s, int t) {
    DinicMaxFlow dFlow(size, s, t);
    for (int i = 0; i < size; ++i)
        for (auto e : edges[i]) dFlow.add(e.from, e.to, e.cost);

    return dFlow.get();
}

int Graph::minCut() {
    MinCut minCutGr(size);
    for (int i = 0; i < size; ++i)
        for (auto e : edges[i]) minCutGr.add(e.from + 1, e.to + 1, e.cost);

    return minCutGr.solve();
}
