#include <bits/stdc++.h>
using namespace std;

template<class T>
struct dijkstra {
	static const T INF;
	dijkstra(int n) : _g(n) {}
	void add_edge(int from, int to, T cost) {
		_g[from].push_back(edge(to, cost));
	}
	void add_indirected_edge(int from, int to, T cost) {
		add_edge(from, to, cost);
		add_edge(to, from, cost);
	}
	vector<T> calc(int s) {
		vector<T> d(_g.size(), INF);
		_prev = vector<int>(_g.size(), -1);
		d[s] = 0;
		priority_queue<pair<T, int>, vector<pair<T, int>>, greater<pair<T, int>>> q;
		q.push({0, s});
		while (!q.empty()) {
			pair<T, int> p = q.top();
			q.pop();
			int v = p.second;
			if (d[v] < p.first) continue;
			for (auto &e : _g[v]) {
				if (d[e.to] > d[v]+e.cost) {
					d[e.to] = d[v]+e.cost;
					_prev[e.to] = v;
					q.push({d[e.to], e.to});
				}
			}
		}
		return d;
	}
	T calc(int s, int t) {
		vector<T> d(_g.size(), INF);
		_prev = vector<int>(_g.size(), -1);
		d[s] = 0;
		priority_queue<pair<T, int>, vector<pair<T, int>>, greater<pair<T, int>>> q;
		q.push({0, s});
		while (!q.empty()) {
			pair<T, int> p = q.top();
			q.pop();
			int v = p.second;
			if (v == t) return d[v];
			if (d[v] < p.first) continue;
			for (auto &e : _g[v]) {
				if (d[e.to] > d[v]+e.cost) {
					d[e.to] = d[v]+e.cost;
					_prev[e.to] = v;
					q.push({d[e.to], e.to});
				}
			}
		}
		return INF;
	}
	vector<int> get_path(int t) {
		vector<int> path;
		for (int cur = t; cur != -1; cur = _prev[cur]) {
			path.push_back(cur);
		}
		reverse(path.begin(), path.end());
		return path;
	}
private:
	struct edge {
		int to;
		T cost;
		edge(int to, T cost) : to(to), cost(cost) {}
	};
	vector<vector<edge>> _g;
	vector<int> _prev;
};
template<> const int dijkstra<int>::INF = 1001001001;
template<> const long long dijkstra<long long>::INF = 1LL << 60;