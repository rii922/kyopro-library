/**
 * @file avl_tree.cpp
 * @author rii922
 * @brief `n` 番目に小さい要素を高速に求められるコンテナ。同じ要素の重複は取り除かない。
 * @date 2025-02-17
 *
 * Verified with:
 * https://judge.yosupo.jp/submission/267681
 * https://atcoder.jp/contests/arc033/submissions/62727378
 */

#include <bits/stdc++.h>
using namespace std;

/**
 * @brief `n` 番目に小さい要素を高速に求められるコンテナ。同じ要素の重複は取り除かない。
 *
 * @tparam T 大小比較可能なクラス
 */
template<class T>
struct avl_tree {
	avl_tree() : _root(nullptr) {}
	avl_tree(avl_tree<T> &avl) : _root(nullptr) {
		for (const T x : avl.vals()) insert(x);
	}
	avl_tree(initializer_list<T> init) : _root(nullptr) {
		for (const T x : init) insert(x);
	}

	/**
	 * @brief 要素を挿入する。既に同じ要素があっても挿入が行われる。
	 *
	 * O(log N)
	 * @param val 挿入する要素
	 */
	void insert(T val) {
		_root = _insert(_root, _index(_root, val), val);
	}

	/**
	 * @brief `n` 番目に小さい要素を削除する。
	 *
	 * O(log N)
	 * @param n 0-indexed
	 */
	void erase_index(int n) {
		_root = _erase(_root, n);
	}

	/**
	 * @brief `val` 以上の最小の要素を 1 個削除する。そのような要素が存在しない場合、削除は行われない。
	 *
	 * O(log N)
	 * @param val 削除する要素
	 */
	void erase(T val) {
		_root = _erase(_root, _index(_root, val));
	}

	/**
	 * @brief `n` 番目に小さい要素を取得する。
	 *
	 * O(log N)
	 * @param n 0-indexed
	 * @return `n` 番目に小さい要素
	 */
	T operator[](int n) {
		return _nth(_root, n)->val;
	}

	/**
	 * @brief `val` 以上の最小の要素が何番目に小さいかを取得する。そのような要素が複数存在する場合、最小のものを返す。そのような要素が存在しない場合、 `size()` を返す。
	 *
	 * O(log N)
	 * @param val 検索する要素
	 * @return `val` 以上の最小の要素のインデックス (0-indexed)
	 */
	int index(T val) {
		return _index(_root, val);
	}

	/**
	 * @brief 要素数を取得する。
	 *
	 * O(1)
	 * @return 要素数
	 */
	int size() {
		return _size(_root);
	}

	/**
	 * @brief `val` 以上の最小の要素が何番目に小さいかを取得する。そのような要素が複数存在する場合、最小のものを返す。そのような要素が存在しない場合、 `size()` を返す。
	 *
	 * O(log N)
	 * @param val 検索する要素
	 * @return `val` 以上の最小の要素のインデックス (0-indexed)
	 */
	int lower_bound_index(T val) {
		return _lower_bound_index(_root, val);
	}

	/**
	 * @brief `val` 以上の最小の要素を取得する。
	 *
	 * O(log N)
	 * @param val 検索する要素
	 * @return `val` 以上の最小の要素
	 */
	T lower_bound(T val) {
		return _nth(_root, _lower_bound_index(_root, val))->val;
	}

	/**
	 * @brief `val` より大きい最小の要素が何番目に小さいかを取得する。そのような要素が複数存在する場合、最小のものを返す。そのような要素が存在しない場合、 `size()` を返す。
	 *
	 * O(log N)
	 * @param val 検索する要素
	 * @return `val` より大きい最小の要素のインデックス (0-indexed)
	 */
	int upper_bound_index(T val) {
		return _upper_bound_index(_root, val);
	}

	/**
	 * @brief `val` より大きい最小の要素を取得する。
	 *
	 * O(log N)
	 * @param val 検索する要素
	 * @return `val` より大きい最小の要素
	 */
	T upper_bound(T val) {
		return _nth(_root, _upper_bound_index(_root, val))->val;
	}

	/**
	 * @brief `val` が何個格納されているかを取得する。
	 *
	 * O(log N)
	 * @param val 検索する要素
	 * @return `val` が格納されている個数
	 */
	int count(T val) {
		return _count(_root, val);
	}

	/**
	 * @brief すべての要素を昇順に格納した `vector` を取得する。 `avl_tree` のすべての要素をイテレートする場合、 `[]` 演算子よりも高速。
	 *
	 * O(N)
	 * @return すべての要素を昇順に格納した `vector`
	 */
	vector<T> vals() {
		vector<T> res;
		_vals(_root, res);
		return res;
	}
private:
	struct node {
		T val;
		node *left, *right;
		int height, size;
		node(T val, node *left=nullptr, node *right=nullptr) : val(val), left(left), right(right), height(1), size(1) {}
	};
	node *_root;
	int _height(node *t) {
		return t == nullptr ? 0 : t->height;
	}
	int _size(node *t) {
		return t == nullptr ? 0 : t->size;
	}
	node *_update(node *t) {
		t->height = max(_height(t->left), _height(t->right)) + 1;
		t->size = _size(t->left) + _size(t->right) + 1;
		return t;
	}
	node *_rotate(node *t, bool direction) {
		node *s = direction ? t->left : t->right;
		if (direction) {
			t->left = s->right;
			s->right = t;
		}
		else {
			t->right = s->left;
			s->left = t;
		}
		t = _update(t);
		s = _update(s);
		return s;
	}
	node *_flatten(node *t) {
		if (t == nullptr) return t;
		if (_height(t->left) - _height(t->right) == 2) {
			if (_height(t->left->right) > _height(t->left->left)) {
				t->left = _rotate(t->left, false);
			}
			t = _rotate(t, true);
		}
		else if (_height(t->right) - _height(t->left) == 2) {
			if (_height(t->right->left) > _height(t->right->right)) {
				t->right = _rotate(t->right, true);
			}
			t = _rotate(t, false);
		}
		return t;
	}
	node *_insert(node *t, int n, T val) {
		if (t == nullptr) return new node(val);
		int sz = _size(t->left);
		if (n > sz) t->right = _insert(t->right, n-sz-1, val);
		else t->left = _insert(t->left, n, val);
		t = _update(t);
		return _flatten(t);
	}
	node *_erase(node *t) {
		if (t == nullptr) return t;
		if (t->left == nullptr && t->right == nullptr) {
			delete t;
			return nullptr;
		}
		else if (t->left == nullptr || t->right == nullptr) {
			node *s = t->left == nullptr ? t->right : t->left;
			delete t;
			return s;
		}
		T val = _nth(t->right, 0)->val;
		node *right = _erase(t->right, 0);
		node *s = new node(val, t->left, right);
		delete t;
		s = _update(s);
		return _flatten(s);
	}
	node *_erase(node *t, int n) {
		if (t == nullptr) return t;
		int sz = _size(t->left);
		if (n < sz) {
			t->left = _erase(t->left, n);
			t = _update(t);
		}
		else if (n > sz) {
			t->right = _erase(t->right, n-sz-1);
			t = _update(t);
		}
		else t = _erase(t);
		return _flatten(t);
	}
	node *_nth(node *t, int n) {
		if (t == nullptr) return t;
		int sz = _size(t->left);
		if (n < sz) return _nth(t->left, n);
		else if (n > sz) return _nth(t->right, n-sz-1);
		else return t;
	}
	int _index(node *t, T val) {
		if (t == nullptr) return 0;
		if (val < t->val) return _index(t->left, val);
		else if (val > t->val) return _size(t->left) + _index(t->right, val) + 1;
		else return _size(t->left);
	}
	int _lower_bound_index(node *t, T val) {
		if (t == nullptr) return 0;
		if (val <= t->val) return _lower_bound_index(t->left, val);
		else return _size(t->left) + _lower_bound_index(t->right, val) + 1;
	}
	int _upper_bound_index(node *t, T val) {
		if (t == nullptr) return 0;
		if (val < t->val) return _upper_bound_index(t->left, val);
		else return _size(t->left) + _upper_bound_index(t->right, val) + 1;
	}
	int _count(node *t, T val) {
		return _upper_bound_index(t, val) - _lower_bound_index(t, val);
	}
	void _vals(node *t, vector<T> &res) {
		if (t == nullptr) return;
		_vals(t->left, res);
		res.push_back(t->val);
		_vals(t->right, res);
	}
};