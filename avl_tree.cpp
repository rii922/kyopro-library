#include <bits/stdc++.h>
using namespace std;

template<class T>
struct avl_tree {
	avl_tree() : _root(nullptr) {}
	avl_tree(avl_tree<T> &avl) : _root(nullptr) {
		for (const T x : avl.vals()) insert(x);
	}
	avl_tree(initializer_list<T> init) : _root(nullptr) {
		for (const T x : init) insert(x);
	}
	void insert(T val) {
		_root = _insert(_root, _index(_root, val), val);
	}
	void erase(T val) {
		_root = _erase(_root, _index(_root, val));
	}
	T operator[](int n) {
		return _nth(_root, n)->val;
	}
	int index(T val) {
		return _index(_root, val);
	}
	int size() {
		return _size(_root);
	}
	int lower_bound_index(T val) {
		return _lower_bound_index(_root, val);
	}
	T lower_bound(T val) {
		return _nth(_root, _lower_bound_index(_root, val))->val;
	}
	int upper_bound_index(T val) {
		return _upper_bound_index(_root, val);
	}
	T upper_bound(T val) {
		return _nth(_root, _upper_bound_index(_root, val))->val;
	}
	int count(T val) {
		return _count(_root, val);
	}
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
	int _height(node *t) { return t == nullptr ? 0 : t->height; }
	int _size(node *t) { return t == nullptr ? 0 : t->size; }
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