#include <bits/stdc++.h>
using namespace std;

struct bellman_ford {
	bellman_ford(long long n) : _n(n) {}
	void add_edge(long long from, long long to, long long cost) {
		edges.push_back(edge(from, to, cost));
	}
	vector<long long> calc(long long s) {
		vector<long long> d(edges.size(), INF);
		d[s] = 0;
		long long cnt = 0;
		while (cnt < _n) {
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
		_has_negative_cycle = (cnt == _n);
		return d;
	}
	bool has_negative_cycle() {
		return _has_negative_cycle;
	}
private:
	struct edge {
		long long from, to, cost;
		edge(long long from, long long to, long long cost) : from(from), to(to), cost(cost) {}
	};
	long long INF = 1LL << 60;
	vector<edge> edges;
	long long _n;
	bool _has_negative_cycle;
};