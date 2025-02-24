#include <bits/stdc++.h>
using namespace std;

/// EXPAND FROM HERE

template<class T>
struct bellman_ford {
	static const T INF;
	bellman_ford(int n) : _n(n) {}
	void add_edge(int from, int to, T cost) {
		_edges.push_back(edge(from, to, cost));
	}
	vector<T> calc(int s) {
		vector<T> d(_edges.size(), INF);
		d[s] = 0;
		int cnt = 0;
		while (cnt < _n) {
			bool flag = true;
			for (auto &e : _edges) {
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
		int from, to;
		T cost;
		edge(int from, int to, T cost) : from(from), to(to), cost(cost) {}
	};
	vector<edge> _edges;
	int _n;
	bool _has_negative_cycle;
};
template<> const int bellman_ford<int>::INF = 1001001001;
template<> const long long bellman_ford<long long>::INF = 1LL << 60;