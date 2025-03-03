/**
 * @file persistent_vector.hpp
 * @author rii922
 * @brief 2 分木による完全永続配列
 * @date 2025-02-24
 */

#include <bits/stdc++.h>
using namespace std;

/// EXPAND FROM HERE

/**
 * @brief 2 分木による完全永続配列。計算量は `T` のコンストラクタやコピーが定数時間であると仮定したもの。
 *
 * @tparam T 配列に格納する値の型
 */
template<class T>
struct persistent_vector {
	/**
	 * @brief 永続配列をサイズ `n` で初期化し、バージョン 0 とする。要素はすべて `T` のデフォルトの値である。
	 *
	 * O(n)
	 * @param n サイズ
	 */
	persistent_vector(int n) {
		_init(n);
		_root.push_back(_generate(_height));
	}

	/**
	 * @brief 永続配列を `v` で初期化し、バージョン 0 とする。
	 *
	 * O(n)
	 * @param v 初期化に使う配列
	 */
	persistent_vector(vector<T> &v) {
		_init(v.size());
		_root.push_back(_generate(_height, v, 0, _n));
	}

	/**
	 * @brief 永続配列をサイズ `n` で初期化し、バージョン 0 とする。要素はすべて `x` となる。
	 *
	 * O(n)
	 * @param n サイズ
	 * @param x 初期値
	 */
	persistent_vector(int n, T x) {
		_init(n);
		_root.push_back(_generate(_height, x, 0, _n));
	}

	/**
	 * @brief 永続配列を `init` で初期化し、バージョン 0 とする。
	 *
	 * O(n)
	 * @param init 初期化に使う配列
	 */
	persistent_vector(initializer_list<T> init) {
		vector<T> v;
		for (const T x : init) v.push_back(x);
		_init(v.size());
		_root.push_back(_generate(_height, v, 0, _n));
	}

	/**
	 * @brief 参照するバージョンを変更する。
	 *
	 * O(1)
	 * @param version 新たに参照するバージョン番号
	 */
	void set_version(int version) {
		assert(version < _root.size());
		_version = version;
	}

	/**
	 * @brief 現在参照しているバージョン番号を取得する。
	 *
	 * O(1)
	 * @return 現在参照しているバージョン番号
	 */
	int version() {
		return _version;
	}

	/**
	 * @brief 最新のバージョン番号を取得する。
	 *
	 * O(1)
	 * @return 最新のバージョン番号
	 */
	int newest_version() {
		return _root.size() - 1;
	}

	/**
	 * @brief 指定したバージョンの配列のインデックス `p` に `x` を格納したものを最新バージョンとし、それを参照するようにする。
	 *
	 * O(log n)
	 * @param p インデックス
	 * @param x 値
	 * @param old_version 変更を加える配列のバージョン番号。指定しない場合、現在参照しているバージョンが使われる。
	 * @return 最新のバージョン番号
	 */
	int set(int p, T x, int old_version=-1) {
		assert(0 <= p && p < _sz);
		if (old_version == -1) old_version = _version;
		node *old_t = _root[old_version];
		node *new_t = new node();
		_root.push_back(new_t);
		vector<bool> v(_height-1);
		for (int i = 0; i < _height-1; i++) {
			v[i] = p & 1;
			p >>= 1;
		}
		for (int i = _height-2; i >= 0; i--) {
			if (v[i]) {
				new_t->left = old_t->left;
				new_t->right = new node();
				old_t = old_t->right;
				new_t = new_t->right;
			}
			else {
				new_t->right = old_t->right;
				new_t->left = new node();
				old_t = old_t->left;
				new_t = new_t->left;
			}
		}
		new_t->val = x;
		return _version = _root.size() - 1;
	}

	/**
	 * @brief 指定したバージョンの配列のインデックス `p` の値を取得する。
	 *
	 * O(log n)
	 * @param p インデックス
	 * @param version 値を取得するバージョン番号。指定しない場合、現在参照しているバージョンが使われる。
	 * @return 値
	 */
	T get(int p, int version=-1) {
		assert(0 <= p && p < _sz);
		if (version == -1) version = _version;
		vector<bool> v(_height-1);
		for (int i = 0; i < _height-1; i++) {
			v[i] = p & 1;
			p >>= 1;
		}
		node *t = _root[version];
		for (int i = _height-2; i >= 0; i--) t = v[i] ? t->right : t->left;
		return t->val;
	}

	/**
	 * @brief 指定したバージョンの配列を取得する。特定のバージョンの配列をイテレートする場合は、 `get` よりも高速。
	 *
	 * O(n)
	 * @param version 取得する配列のバージョン番号。指定しない場合、現在参照しているバージョンが使われる。
	 * @return 配列
	 */
	vector<T> get_all(int version=-1) {
		if (version == -1) version = _version;
		vector<T> res;
		_get_all(_root[version], res, 0, _n);
		return res;
	}
private:
	struct node {
		T val;
		node *left, *right;
		node (T val=T(), node *left=nullptr, node *right=nullptr) : val(val), left(left), right(right) {}
	};
	vector<node *> _root;
	int _sz, _n, _height;
	int _version = 0;
	void _init(int n) {
		_sz = n;
		_n = 1;
		_height = 1;
		while (_n < _sz) {
			_n *= 2;
			_height++;
		}
	}
	node *_generate(int height) {
		if (height == 0) return nullptr;
		node *t = new node();
		t->left = _generate(height-1);
		t->right = _generate(height-1);
		return t;
	}
	node *_generate(int height, vector<T> &v, int l, int r) {
		if (height == 0) return nullptr;
		node *t = new node();
		if (height == 1 && l < _sz) t->val = v[l];
		int m = (l + r) / 2;
		t->left = _generate(height-1, v, l, m);
		t->right = _generate(height-1, v, m, r);
		return t;
	}
	node *_generate(int height, T x, int l, int r) {
		if (height == 0) return nullptr;
		node *t = new node();
		if (height == 1 && l < _sz) t->val = x;
		int m = (l + r) / 2;
		t->left = _generate(height-1, x, l, m);
		t->right = _generate(height-1, x, m, r);
		return t;
	}
	void _get_all(node *t, vector<T> &res, int l, int r) {
		if (l >= _sz) return;
		if (t->left == nullptr) res.push_back(t->val);
		else {
			int m = (l + r) / 2;
			_get_all(t->left, res, l, m);
			_get_all(t->right, res, m, r);
		}
	}
};