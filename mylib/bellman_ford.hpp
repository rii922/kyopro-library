/**
 * @file bellman_ford.hpp
 * @author rii922
 * @brief Bellman-Ford 法によって単一始点最短経路を求める。
 * @date 2025-03-02
 */

#include <bits/stdc++.h>
using namespace std;

/// EXPAND FROM HERE

/**
 * @brief Bellman-Ford 法によって単一始点最短経路を求めるクラス。
 *
 * @tparam T 辺の重みを表す整数型
 */
template<class T>
struct bellman_ford {
	static const T INF;

	/**
	 * @brief `n` 頂点 0 辺のグラフで初期化する。
	 *
	 * @param n 頂点数
	 */
	bellman_ford(int n) : _n(n) {}

	/**
	 * @brief `from` から `to` へ重さ `cost` の有向辺を追加する。
	 *
	 * @param from 始点
	 * @param to 終点
	 * @param cost 重み
	 */
	void add_edge(int from, int to, T cost) {
		_edges.push_back(edge(from, to, cost));
	}

	/**
	 * @brief `s` を始点として各頂点への最短経路を求める。負閉路があるかどうかも検出する。
	 *
	 * O(VE)
	 * @param s 始点
	 * @return 各頂点への最短経路。負閉路がある場合は未定義。
	 */
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

	/**
	 * @brief 負閉路があるかどうかを返す。事前に `calc(s)` を呼び出しておく必要がある。
	 *
	 * @return 負閉路があるかどうか
	 */
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