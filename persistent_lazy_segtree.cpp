#include <bits/stdc++.h>
using namespace std;

/// EXPAND FROM HERE

template<class S, S (*op)(S, S), S (*e)(), class F, S (*mapping)(F, S), F (*composition)(F, F)>
struct persistent_lazy_segtree {
	persistent_lazy_segtree(int n) {
		_init(n);
		_root.push_back(_generate(_height));
	}
	persistent_lazy_segtree(vector<S> &v) {
		_init(v.size());
		_root.push_back(_generate(_height, v, 0, _n));
	}
	persistent_lazy_segtree(int n, S x) {
		_init(n);
		_root.push_back(_generate(_height, x, 0, _n));
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
	int set(int p, S x, int old_version=-1) {
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
		vector<node *> pass(_height-1);
		for (int i = _height-2; i >= 0; i--) {
			pass[i] = new_t;
			_eval(old_t);
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
		for (int i = 0; i < _height-1; i++) {
			pass[i]->val = op(pass[i]->left->val, pass[i]->right->val);
		}
		return _version = _root.size() - 1;
	}
	S get(int p, int version=-1) {
		assert(0 <= p && p < _sz);
		if (version == -1) version = _version;
		vector<bool> v(_height-1);
		for (int i = 0; i < _height-1; i++) {
			v[i] = p & 1;
			p >>= 1;
		}
		node *t = _root[version];
		for (int i = _height-2; i >= 0; i--) {
			_eval(t);
			t = v[i] ? t->right : t->left;
		}
		return t->val;
	}
	S prod(int l, int r, int version=-1) {
		assert(0 <= l && l <= r && r <= _sz);
		if (version == -1) version = _version;
		return _prod(_root[version], l, r, 0, _n);
	}
	S all_prod(int version=-1) {
		if (version == -1) version = _version;
		return _root[version]->val;
	}
	int apply(int l, int r, F f, int version=-1) {
		assert(0 <= l && l <= r && r <= _sz);
		if (version == -1) version = _version;
		_root.push_back(_apply(_root[version], l, r, f, 0, _n));
		return _version = _root.size() - 1;
	}
	int max_right(int l, bool (*f)(S), int version=-1) {
		assert(0 <= l && l <= _sz);
		if (version == -1) version = _version;
		S pr = e();
		return _max_right(_root[version], l, f, 0, _n, pr);
	}
	int min_left(int r, bool (*f)(S), int version=-1) {
		assert(0 <= r && r <= _sz);
		if (version == -1) version = _version;
		S pr = e();
		return _min_left(_root[version], r, f, 0, _n, pr);
	}
private:
	struct node {
		S val;
		optional<F> lazy;
		node *left, *right;
		node(S val=e(), optional<F> lazy=nullopt, node *left=nullptr, node *right=nullptr) : val(val), lazy(lazy), left(left), right(right) {}
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
	node *_generate(int height, vector<S> &v, int l, int r) {
		if (height == 0) return nullptr;
		node *t = new node();
		if (height == 1 && l < _sz) t->val = v[l];
		int m = (l + r) / 2;
		t->left = _generate(height-1, v, l, m);
		t->right = _generate(height-1, v, m, r);
		if (t->left != nullptr) t->val = op(t->left->val, t->right->val);
		return t;
	}
	node *_generate(int height, S x, int l, int r) {
		if (height == 0) return nullptr;
		node *t = new node();
		if (height == 1 && l < _sz) t->val = x;
		int m = (l + r) / 2;
		t->left = _generate(height-1, x, l, m);
		t->right = _generate(height-1, x, m, r);
		if (t->left != nullptr) t->val = op(t->left->val, t->right->val);
		return t;
	}
	void _eval(node *t) {
		if (t->lazy) {
			t->left = new node(mapping(t->lazy.value(), t->left->val), t->left->lazy ? composition(t->lazy.value(), t->left->lazy.value()) : t->lazy, t->left->left, t->left->right);
			t->right = new node(mapping(t->lazy.value(), t->right->val), t->right->lazy ? composition(t->lazy.value(), t->right->lazy.value()) : t->lazy, t->right->left, t->right->right);
			t->lazy = nullopt;
		}
	}
	S _prod(node *t, int l, int r, int lt, int rt) {
		if (lt >= r || rt <= l) return e();
		if (lt >= l && rt <= r) return t->val;
		_eval(t);
		int mt = (lt + rt) / 2;
		return op(_prod(t->left, l, r, lt, mt), _prod(t->right, l, r, mt, rt));
	}
	node *_apply(node *t, int l, int r, F f, int lt, int rt) {
		if (lt >= r || rt <= l) return t;
		if (lt >= l && rt <= r) return new node(mapping(f, t->val), t->lazy ? composition(f, t->lazy.value()) : f, t->left, t->right);
		_eval(t);
		int mt = (lt + rt) / 2;
		node *left = _apply(t->left, l, r, f, lt, mt);
		node *right = _apply(t->right, l, r, f, mt, rt);
		return new node(op(left->val, right->val), nullopt, left, right);
	}
	int _max_right(node *t, int l, bool (*f)(S), int lt, int rt, S &pr) {
		if (lt >= _sz) return _sz;
		if (rt <= l) return l;
		if (lt >= l && rt <= _sz) {
			S npr = op(pr, t->val);
			if (f(npr)) {
				pr = npr;
				return rt;
			}
			if (rt - lt == 1) return lt;
		}
		_eval(t);
		int mt = (lt + rt) / 2;
		int m = _max_right(t->left, l, f, lt, mt, pr);
		if (m < mt) return m;
		return _max_right(t->right, l, f, mt, rt, pr);
	}
	int _min_left(node *t, int r, bool (*f)(S), int lt, int rt, S &pr) {
		if (lt >= r) return r;
		if (rt <= 0) return 0;
		if (rt <= r) {
			S npr = op(pr, t->val);
			if (f(npr)) {
				pr = npr;
				return lt;
			}
			if (rt - lt == 1) return rt;
		}
		_eval(t);
		int mt = (lt + rt) / 2;
		int m = _min_left(t->right, r, f, mt, rt, pr);
		if (m > mt) return m;
		return _min_left(t->left, r, f, lt, mt, pr);
	}
};