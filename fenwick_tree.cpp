#include <bits/stdc++.h>
using namespace std;

/// EXPAND FROM HERE

template<class T>
struct fenwick_tree {
	fenwick_tree() {}
	fenwick_tree(int n) : _v(n+1, T(0)) {}
	fenwick_tree(vector<T> &v) : _v(v.size()+1, T(0)) {
		for (int i = 0; i < _v.size(); i++) add(i, v[i]);
	}
	void add(int p, T x) {
		p++;
		while (p < _v.size()) {
			_v[p] += x;
			p += p & -p;
		}
	}
	void set(int p, T x) {
		add(p, x-get(p));
	}
	T get(int p) {
		return sum(p, p+1);
	}
	T sum(int r) {
		T res = 0;
		while (r > 0) {
			res += _v[r];
			r -= r & -r;
		}
		return res;
	}
	T sum(int l, int r) {
		return sum(r) - sum(l);
	}
private:
	vector<T> _v;
};