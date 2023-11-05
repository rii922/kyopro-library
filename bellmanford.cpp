#include <bits/stdc++.h>
using namespace std;

long long INF = 1LL << 60;
struct Edge {
	long long from, to, cost;
	Edge(long long from, long long to, long long cost) : from(from), to(to), cost(cost) {}
};
struct Graph {
	vector<Edge> edges;
	long long v;
	vector<long long> d;
	bool is_endless;
	Graph(long long n) : v(n) {
		d.assign(n, INF);
	}
	void add_edge(long long from, long long to, long long cost) {
		edges.push_back(Edge(from, to, cost));
	}
	void bellman_ford(long long s) {
		d[s] = 0;
		long long cnt = 0;
		while (cnt < v) {
			bool flag = true;
			for (auto &e : edges) {
				if (d[e.from] != INF && d[e.to] < d[e.from]+e.cost) {
					d[e.to] = d[e.from]+e.cost;
					flag = false;
				}
			}
			if (flag) break;
			cnt++;
		}
		is_endless = (cnt == v);
	}
};