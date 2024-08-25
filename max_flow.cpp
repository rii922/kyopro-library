#include <bits/stdc++.h>
using namespace std;

template<class T>
struct max_flow {
	max_flow(int n) : _graph(n), _level(n), _iter(n), _index(0) {}
	int add_edge(int from, int to, T cap) {
		int pos = _graph[from].size();
		int rev_pos = _graph[to].size();
		_graph[from].push_back(edge(to, rev_pos, cap, _index));
		_graph[to].push_back(edge(from, pos, 0, -1));
		return _index++;
	}
	T flow(int s, int t) {
		T res = 0;
		while (true) {
			fill(_iter.begin(), _iter.end(), 0);
			_bfs(s);
			if (_level[t] == -1) break;
			while (true) {
				T f = _dfs(s, t, INF);
				if (f == 0) break;
				res += f;
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
	vector<bool> min_cut() {
		vector<bool> res(_graph.size());
		for (int i = 0; i < _graph.size(); i++) res[i] = _level[i] >= 0;
		return res;
	}
private:
	struct edge {
		int to, rev_pos, index;
		T cap;
		edge(int to, int rev_pos, T cap, int index) : to(to), rev_pos(rev_pos), cap(cap), index(index) {}
	};
	static const T INF;
	vector<vector<edge>> _graph;
	vector<int> _level, _iter;
	int _index;
	void _bfs(int s) {
		fill(_level.begin(), _level.end(), -1);
		_level[s] = 0;
		queue<int> q;
		q.push(s);
		while (!q.empty()) {
			int v = q.front();
			q.pop();
			for (auto &e : _graph[v]) {
				if (_level[e.to] == -1 && e.cap > 0) {
					_level[e.to] = _level[v] + 1;
					q.push(e.to);
				}
			}
		}
	}
	T _dfs(int v, int t, T f) {
		if (v == t) return f;
		for (int &i = _iter[v]; i < _graph[v].size(); i++) {
			edge &e = _graph[v][i];
			if (_level[v] < _level[e.to] && e.cap > 0) {
				T d = _dfs(e.to, t, min(f, e.cap));
				if (d > 0) {
					e.cap -= d;
					_graph[e.to][e.rev_pos].cap += d;
					return d;
				}
			}
		}
		return 0;
	}
};
template<> const int max_flow<int>::INF = 1001001001;
template<> const long long max_flow<long long>::INF = 1LL << 60;