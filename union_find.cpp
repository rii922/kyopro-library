#include <bits/stdc++.h>
using namespace std;

struct union_find {
	union_find(int n) : _d(n, -1) {}
	int find(int v) {
		if (_d[v] < 0) return v;
		return _d[v] = find(_d[v]);
	}
	bool unite(int u, int v) {
		u = find(u);
		v = find(v);
		if (u == v) return false;
		if (_d[u] > _d[v]) swap(u, v);
		_d[u] += _d[v];
		_d[v] = u;
		return true;
	}
	bool same(int u, int v) {
		return find(u) == find(v);
	}
	int size(int v) {
		return -_d[find(v)];
	}
private:
	vector<int> _d;
};