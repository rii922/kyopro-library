#include <bits/stdc++.h>
using namespace std;

template<class T>
struct persistent_vector {
	persistent_vector(int n) {
		_init(n);
		_root.push_back(_generate(_height));
	}
	persistent_vector(vector<T> &v) {
		_init(v.size());
		_root.push_back(_generate(_height, v, 0, _n));
	}
	persistent_vector(int n, T x) {
		_init(n);
		_root.push_back(_generate(_height, x, 0, _n));
	}
	persistent_vector(initializer_list<T> init) {
		vector<T> v;
		for (const T x : init) v.push_back(x);
		_init(v.size());
		_root.push_back(_generate(_height, v, 0, _n));
	}
	void set_version(int version) {
		assert(version < _root.size());
		_version = version;
	}
	int version() {
		return _version;
	}
	int newest_version() {
		return _root.size() - 1;
	}
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