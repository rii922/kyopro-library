/**
 * @file segtree.hpp
 * @author rii922
 * @brief 1 点更新および区間の要素の総積の取得を高速に行うセグメント木
 * @date 2025-03-04
 *
 * Verified with:
 * https://judge.yosupo.jp/submission/271067
 * https://judge.yosupo.jp/submission/271081
 */

#include <bits/stdc++.h>
using namespace std;

/// EXPAND FROM HERE

/**
 * @brief 1 点更新および区間の要素の総積の取得を高速に行うセグメント木
 *
 * @tparam S モノイド
 * @tparam op 積
 * @tparam e 単位元
 */
template<class S, S (*op)(S, S), S (*e)()>
struct segtree {
	/**
	 * @brief セグメント木をサイズ `n` で初期化する。要素はすべて `e()` である。
	 *
	 * O(n)
	 * @param n サイズ
	 */
	segtree(int n) {
		_init(n);
	}

	/**
	 * @brief セグメント木を `v` で初期化する。
	 *
	 * O(n)
	 * @param v 初期化に使う配列
	 */
	segtree(vector<S> &v) {
		_init(v.size());
		for (int i = 0; i < _sz; i++) _node[_n+i-1] = v[i];
		for (int i = _n-2; i >= 0; i--) _node[i] = op(_node[i*2+1], _node[i*2+2]);
	}

	/**
	 * @brief セグメント木をサイズ `n` で初期化する。要素はすべて `x` となる。
	 *
	 * O(n)
	 * @param n サイズ
	 * @param x 初期値
	 */
	segtree(int n, S x) {
		_init(n);
		for (int i = 0; i < _sz; i++) _node[_n+i-1] = x;
		for (int i = _n-2; i >= 0; i--) _node[i] = op(_node[i*2+1], _node[i*2+2]);
	}

	/**
	 * @brief インデックス `p` に `x` を代入する。
	 *
	 * O(log n)
	 * @param p インデックス
	 * @param x 値
	 */
	void set(int p, S x) {
		assert(0 <= p && p < _sz);
		p += _n - 1;
		_node[p] = x;
		while (p > 0) {
			p = (p - 1) / 2;
			_node[p] = op(_node[p*2+1], _node[p*2+2]);
		}
	}

	/**
	 * @brief インデックス `p` の値を取得する。
	 *
	 * O(1)
	 * @param p インデックス
	 * @return インデックス `p` の値
	 */
	S get(int p) {
		assert(0 <= p && p < _sz);
		return _node[_n+p-1];
	}

	/**
	 * @brief 区間 `[l, r)` の要素の総積 `a[l]*a[l+1]*...*a[r-1]` を取得する。
	 *
	 * O(log n)
	 * @param l 左端
	 * @param r 右端
	 * @return 区間 `[l, r)` の要素の総積
	 */
	S prod(int l, int r) {
		assert(0 <= l && l <= r && r <= _sz);
		return _prod(l, r, 0, 0, _n);
	}

	/**
	 * @brief 全要素の総積 `a[0]*a[1]*...*a[n-1]` を取得する。
	 *
	 * O(1)
	 * @return 全要素の総積
	 */
	S all_prod() {
		return _node[0];
	}

	/**
	 * @brief (`r=l` または `f(a[l]*a[l+1]*...*a[r-1])=true`) かつ (`r=n` または `f(a[l]*a[l+1]*...*a[r])=false`) なる `r` を 1 つ返す。 `f(e())=true` である必要がある。
	 *
	 * `f` の計算量を定数時間として O(log n)
	 * @param l 左端
	 * @param f `S` から `bool` への関数で、 `f(e())=true`
	 * @return (`r=l` または `f(a[l]*a[l+1]*...*a[r-1])=true`) かつ (`r=n` または `f(a[l]*a[l+1]*...*a[r])=false`) なる `r`
	 */
	int max_right(int l, bool (*f)(S)) {
		assert(0 <= l && l <= _sz);
		S pr = e();
		return _max_right(l, f, 0, 0, _n, pr);
	}

	/**
	 * @brief (`l=r` または `f(a[l]*a[l+1]*...*a[r-1])=true`) かつ (`l=0` または `f(a[l-1]*a[l]*...*a[r-1])=false`) なる `l` を 1 つ返す。 `f(e())=true` である必要がある。
	 *
	 * `f` の計算量を定数時間として O(log n)
	 * @param r 右端
	 * @param f `S` から `bool` への関数で、 `f(e())=true`
	 * @return (`l=r` または `f(a[l]*a[l+1]*...*a[r-1])=true`) かつ (`l=0` または `f(a[l-1]*a[l]*...*a[r-1])=false`) なる `l`
	 */
	int min_left(int r, bool (*f)(S)) {
		assert(0 <= r && r <= _sz);
		S pr = e();
		return _min_left(r, f, 0, 0, _n, pr);
	}
private:
	int _sz, _n;
	vector<S> _node;
	void _init(int n) {
		_sz = n;
		_n = 1;
		while (_n < _sz) _n *= 2;
		_node.resize(_n*2-1, e());
	}
	S _prod(int l, int r, int p, int lp, int rp) {
		if (lp >= r || rp <= l) return e();
		if (lp >= l && rp <= r) return _node[p];
		int mp = (lp + rp) / 2;
		return op(_prod(l, r, p*2+1, lp, mp), _prod(l, r, p*2+2, mp, rp));
	}
	int _max_right(int l, bool (*f)(S), int p, int lp, int rp, S &pr) {
		if (lp >= _sz) return _sz;
		if (rp <= l) return l;
		if (lp >= l && rp <= _sz) {
			S npr = op(pr, _node[p]);
			if (f(npr)) {
				pr = npr;
				return rp;
			}
			if (rp - lp == 1) return lp;
		}
		int mp = (lp + rp) / 2;
		int m = _max_right(l, f, p*2+1, lp, mp, pr);
		if (m < mp) return m;
		return _max_right(l, f, p*2+2, mp, rp, pr);
	}
	int _min_left(int r, bool (*f)(S), int p, int lp, int rp, S &pr) {
		if (lp >= r) return r;
		if (rp <= 0) return 0;
		if (rp <= r) {
			S npr = op(pr, _node[p]);
			if (f(npr)) {
				pr = npr;
				return lp;
			}
			if (rp - lp == 1) return rp;
		}
		int mp = (lp + rp) / 2;
		int m = _min_left(r, f, p*2+2, mp, rp, pr);
		if (m > mp) return m;
		return _min_left(r, f, p*2+1, lp, mp, pr);
	}
};