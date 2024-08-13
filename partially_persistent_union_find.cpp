#include <bits/stdc++.h>
using namespace std;

struct partially_persistent_union_find {
	partially_persistent_union_find(int n) : _parent(n, -1), _rank(n, 0), _time(n, INF), _size(n, {{0, 1}}) {}
	int version() {
		return _version;
	}
	int find(int v, int version=-1) {
		if (version == -1) version = _version;
		assert(version <= _version);
		if (_time[v] > version) return v;
		return find(_parent[v], version);
	}
	bool unite(int u, int v) {
		_version++;
		u = find(u);
		v = find(v);
		if (u == v) return false;
		if (_rank[u] < _rank[v]) swap(u, v);
		_size[u].push_back({_version, size(u, _version) + size(v, _version)});
		_parent[v] = u;
		if (_rank[u] == _rank[v]) _rank[u]++;
		_time[v] = _version;
		return true;
	}
	bool same(int u, int v, int version=-1) {
		if (version == -1) version = _version;
		assert(version <= _version);
		return find(u, version) == find(v, version);
	}
	int size(int v, int version=-1) {
		if (version == -1) version = _version;
		assert(version <= _version);
		v = find(v, version);
		int left = 0;
		int right = _size[v].size();
		while (right - left > 1) {
			int mid = (left + right) / 2;
			if (_size[v][mid].first <= version) left = mid;
			else right = mid;
		}
		return _size[v][left].second;
	}
private:
	const int INF = 1001001001;
	vector<int> _parent, _rank, _time;
	vector<vector<pair<int, int>>> _size;
	int _version = 0;
};