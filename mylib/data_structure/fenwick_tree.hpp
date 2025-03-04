/**
 * @file fenwick_tree.hpp
 * @author rii922
 * @brief 1 点更新および区間和の取得を高速に行う Fenwick 木
 * @date 2025-03-04
 *
 * Verified with:
 * https://judge.yosupo.jp/submission/271070
 */

#include <bits/stdc++.h>
using namespace std;

/// EXPAND FROM HERE

/**
 * @brief 1 点更新および区間和の取得を高速に行う Fenwick 木
 *
 * @tparam T 群
 */
template<class T>
struct fenwick_tree {
	/**
	 * @brief Fenwick 木をサイズ `n` で初期化する。要素はすべて 0 である。
	 *
	 * O(n)
	 * @param n サイズ
	 */
	fenwick_tree(int n) : _v(n+1, T(0)) {}

	/**
	 * @brief Fenwick 木を `v` で初期化する。
	 *
	 * O(n)
	 * @param v 初期化に使う配列
	 */
	fenwick_tree(vector<T> &v) : _v(v.size()+1, T(0)) {
		for (int i = 0; i < _v.size(); i++) add(i, v[i]);
	}

	/**
	 * @brief インデックス `p` に `x` を加算する。
	 *
	 * O(log n)
	 * @param p インデックス
	 * @param x 値
	 */
	void add(int p, T x) {
		p++;
		while (p < _v.size()) {
			_v[p] += x;
			p += p & -p;
		}
	}

	/**
	 * @brief インデックス `p` に `x` を代入する。
	 *
	 * O(log n)
	 * @param p インデックス
	 * @param x 値
	 */
	void set(int p, T x) {
		add(p, x-get(p));
	}

	/**
	 * @brief インデックス `p` の値を取得する。
	 *
	 * O(log n)
	 * @param p インデックス
	 * @return インデックス `p` の値
	 */
	T get(int p) {
		return sum(p, p+1);
	}

	/**
	 * @brief 区間 `[0, r)` の要素の総和 `a[0]+a[1]+...+a[r-1]` を取得する。
	 *
	 * O(log n)
	 * @param r 右端
	 * @return 区間 `[0, r)` の要素の総和
	 */
	T sum(int r) {
		T res = 0;
		while (r > 0) {
			res += _v[r];
			r -= r & -r;
		}
		return res;
	}

	/**
	 * @brief 区間 `[l, r)` の要素の総和 `a[l]+a[l+1]+...+a[r-1]` を取得する。
	 *
	 * O(log n)
	 * @param l 左端
	 * @param r 右端
	 * @return 区間 `[l, r)` の要素の総和
	 */
	T sum(int l, int r) {
		return sum(r) - sum(l);
	}
private:
	vector<T> _v;
};