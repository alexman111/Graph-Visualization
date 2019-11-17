#include <bits/stdc++.h>

struct edge {
    edge(int from, int to, long long cost);
    edge();
    int from, to;
    long long cost;
};

class MinCut {
private:
    int n;
    int INF = 2e9;
    std::vector< std::vector<int> > g;
    std::vector<int> v;
    std::vector<int> d;
    std::vector<int> vis;
public:
    MinCut(int n);

    void add(int u, int v, int c);

    int solve();
};


class DinicMaxFlow {
private:
    struct Edge {
        int to, rev, cap, flow;
    };

    int n, s, t;
    std::vector<std::vector <Edge> > g;
    std::vector<int> d, it;

    bool bfs();

    int dfs(int a, int flow);

public:
    DinicMaxFlow(int n, int s, int t);

    void add(int a, int b, int cap);
    std::pair <int, std::vector <edge> > get();
};

class Graph {

private:
    int size, edgesCount, procEdges;
    std::vector <std::vector <edge> > edges;
    std::vector <int> tin, tout;
    std::vector <std::vector <int > > up;
    int logSize;

public:
    Graph();

    Graph(int size_);

    void clearGraph();

    void resizeGraph(int size_);

    int lca(int a, int b);

    void addEdge(int from, int to, long long cost);

    std::vector <edge> findBridges();

    std::pair <std::vector <long long>, std::vector <int> > djikstra(int start);

    std::pair <long long, std::vector <edge> > kruskal();

    std::pair <int, std::vector <edge> > maxFlow(int s, int t);

    int minCut();
};
