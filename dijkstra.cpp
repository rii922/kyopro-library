#include <bits/stdc++.h>
using namespace std;

long long INF = 1LL << 60;
using P = pair<long long, long long>;
struct Edge {
	long long to, cost;
	Edge(long long to, long long cost) : to(to), cost(cost) {}
};
struct Graph {
	vector<vector<Edge>> g;
	vector<long long> d;
	vector<long long> prev;
	Graph(long long n) {
		g.resize(n);
		d.assign(n, INF);
		prev.assign(n, -1);
	}
	void add_edge(long long from, long long to, long long cost) {
		g[from].push_back(Edge(to, cost));
	}
	void add_indirected_edge(long long from, long long to, long long cost) {
		add_edge(from, to, cost);
		add_edge(to, from, cost);
	}
	void dijkstra(long long s) {
		d[s] = 0;
		priority_queue<P, vector<P>, greater<P>> q;
		q.push({0, s});
		while (!q.empty()) {
			P p = q.top();
			q.pop();
			long long v = p.second;
			if (d[v] < p.first) continue;
			for (auto &e : g[v]) {
				if (d[e.to] > d[v]+e.cost) {
					d[e.to] = d[v]+e.cost;
					prev[e.to] = v;
					q.push({d[e.to], e.to});
				}
			}
		}
	}
	vector<long long> get_path(long long t) {
		vector<long long> path;
		for (long long cur = t; cur != -1; cur = prev[cur]) {
			path.push_back(cur);
		}
		reverse(path.begin(), path.end());
		return path;
	}
};