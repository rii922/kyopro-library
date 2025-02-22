/**
 * @file union_find.cpp
 * @author rii922
 * @brief 素集合データ構造
 * @date 2025-02-22
 *
 * Verified with:
 * https://judge.yosupo.jp/submission/268143
 */

#include <bits/stdc++.h>
using namespace std;

/// EXPAND FROM HERE

/**
 * @brief 素集合データ構造
 *
 */
struct union_find {
	/**
	 * @brief コンストラクタ
	 *
	 * @param n 頂点数
	 */
	union_find(int n) : _d(n, -1) {}

	/**
	 * @brief 頂点 `v` を含む連結成分の根を返す。
	 *
	 * O(α(n))
	 * @param v 頂点
	 * @return 頂点 `v` を含む連結成分の根
	 */
	int find(int v) {
		if (_d[v] < 0) return v;
		return _d[v] = find(_d[v]);
	}

	/**
	 * @brief 頂点 `u` と `v` を同じ連結成分に所属させる。
	 *
	 * O(α(n))
	 * @param u 頂点
	 * @param v 頂点
	 * @return `u` と `v` が新たに連結になったなら `true` 、既に連結だったなら `false`
	 */
	bool unite(int u, int v) {
		u = find(u);
		v = find(v);
		if (u == v) return false;
		if (_d[u] > _d[v]) swap(u, v);
		_d[u] += _d[v];
		_d[v] = u;
		return true;
	}

	/**
	 * @brief 頂点 `u` と `v` が同じ連結成分に属するかを判定する。
	 *
	 * O(α(n))
	 * @param u 頂点
	 * @param v 頂点
	 * @return 頂点 `u` と `v` が同じ連結成分に属するかどうか
	 */
	bool same(int u, int v) {
		return find(u) == find(v);
	}

	/**
	 * @brief 頂点 `v` を含む連結成分の頂点数を返す。
	 *
	 * O(α(n))
	 * @param v 頂点
	 * @return 頂点 `v` を含む連結成分の頂点数
	 */
	int size(int v) {
		return -_d[find(v)];
	}
private:
	vector<int> _d;
};