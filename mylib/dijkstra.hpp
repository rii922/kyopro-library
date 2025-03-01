/**
 * @file dijkstra.hpp
 * @author rii922
 * @brief Dijkstra 法によって単一始点最短経路を求める。
 * @date 2025-03-02
 *
 * Verified with:
 * https://judge.yosupo.jp/submission/270571
 */

#include <bits/stdc++.h>
using namespace std;

/// EXPAND FROM HERE

/**
 * @brief Dijkstra 法によって単一始点最短経路を求めるクラス。
 *
 * @tparam T 辺の重みを表す整数型
 */
template<class T>
struct dijkstra {
	static const T INF;

	/**
	 * @brief `n` 頂点 0 辺のグラフで初期化する。
	 *
	 * @param n 頂点数
	 */
	dijkstra(int n) : _g(n) {}

	/**
	 * @brief `from` から `to` へ重さ `cost` の有向辺を追加する。
	 *
	 * @param from 始点
	 * @param to 終点
	 * @param cost 重み
	 */
	void add_edge(int from, int to, T cost) {
		_g[from].push_back(edge(to, cost));
	}

	/**
	 * @brief `from` から `to` へ重さ `cost` の無向辺を追加する。
	 *
	 * @param from 始点
	 * @param to 終点
	 * @param cost 重み
	 */
	void add_indirected_edge(int from, int to, T cost) {
		add_edge(from, to, cost);
		add_edge(to, from, cost);
	}

	/**
	 * @brief `s` を始点として各頂点への最短経路長を求める。
	 *
	 * O(V+E log E)
	 * @param s 始点
	 * @return 各頂点への最短経路長
	 */
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

	/**
	 * @brief `s` から `t` への最短経路長を求める。
	 *
	 * O(V+E log E)
	 * @param s 始点
	 * @param t 終点
	 * @return `s` から `t` への最短経路長
	 */
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

	/**
	 * @brief `t` への最短パスを 1 つ求め、通る頂点を順に並べた `vector` を返す。事前に `calc(s)` を呼び出しておく必要がある。
	 *
	 * @param t 終点
	 * @return `t` への最短パスで通る頂点を順に並べた `vector`
	 */
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