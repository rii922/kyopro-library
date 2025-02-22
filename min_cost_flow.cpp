#include <bits/stdc++.h>
using namespace std;

/// EXPAND FROM HERE

template<class T>
struct min_cost_flow {
	min_cost_flow(int n) : _graph(n), _h(n), _prevv(n), _preve(n), _index(0) {}
	int add_edge(int from, int to, T cap, T cost) {
		int pos = _graph[from].size();
		int rev_pos = _graph[to].size();
		_graph[from].push_back(edge(to, rev_pos, cap, cost, _index));
		_graph[to].push_back(edge(from, pos, 0, -cost, -1));
		return _index++;
	}
	T flow(int s, int t, T f) {
		fill(_h.begin(), _h.end(), 0);
		fill(_prevv.begin(), _prevv.end(), -1);
		fill(_preve.begin(), _preve.end(), -1);
		T res = 0;
		priority_queue<pair<T, int>, vector<pair<T, int>>, greater<pair<T, int>>> q;
		vector<T> dist(_graph.size());
		while (f > 0) {
			fill(dist.begin(), dist.end(), INF);
			q.push({0, s});
			dist[s] = 0;
			while (!q.empty()) {
				pair<T, int> p = q.top();
				q.pop();
				if (p.first > dist[p.second]) continue;
				for (int i = 0; i < _graph[p.second].size(); i++) {
					edge &e = _graph[p.second][i];
					T d = dist[p.second] + e.cost + _h[p.second] - _h[e.to];
					if (e.cap > 0 && dist[e.to] > d) {
						dist[e.to] = d;
						_prevv[e.to] = p.second;
						_preve[e.to] = i;
						q.push({d, e.to});
					}
				}
			}
			if (dist[t] == INF) return -1;
			for (int i = 0; i < _graph.size(); i++) _h[i] += dist[i];
			T d = f;
			for (int v = t; v != s; v = _prevv[v]) d = min(d, _graph[_prevv[v]][_preve[v]].cap);
			f -= d;
			res += d * _h[t];
			for (int v = t; v != s; v = _prevv[v]) {
				edge &e = _graph[_prevv[v]][_preve[v]];
				e.cap -= d;
				_graph[e.to][e.rev_pos].cap += d;
			}
		}
		return res;
	}
	vector<T> edges() {
		vector<T> res(_index);
		for (int i = 0; i < _graph.size(); i++) {
			for (auto &e : _graph[i]) {
				if (e.index >= 0) res[e.index] = _graph[e.to][e.rev_pos].cap;
			}
		}
		return res;
	}
private:
	struct edge {
		int to, rev_pos, index;
		T cap, cost;
		edge(int to, int rev_pos, T cap, T cost, int index) : to(to), rev_pos(rev_pos), cap(cap), cost(cost), index(index) {}
	};
	static const T INF;
	vector<vector<edge>> _graph;
	vector<T> _h;
	vector<int> _prevv, _preve;
	int _index;
};
template<> const int min_cost_flow<int>::INF = 1001001001;
template<> const long long min_cost_flow<long long>::INF = 1LL << 60;