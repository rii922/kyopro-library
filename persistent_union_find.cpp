#include <bits/stdc++.h>
using namespace std;

struct persistent_union_find {
	persistent_union_find(int n) : _d(n, -1) {
		_d_version.push_back(_d.newest_version());
	}
	void set_version(int version) {
		assert(version < _d_version.size());
		_version = version;
	}
	int version() {
		return _version;
	}
	int newest_version() {
		return _d_version.size() - 1;
	}
	int find(int v, int version=-1) {
		if (version == -1) version = _version;
		int d = _d.get(v, _d_version[version]);
		if (d < 0) return v;
		return find(d, version);
	}
	bool unite(int u, int v, int old_version=-1) {
		if (old_version == -1) old_version = _version;
		u = find(u, old_version);
		v = find(v, old_version);
		if (u == v) {
			_d_version.push_back(_d.newest_version());
			_version = _d_version.size() - 1;
			return false;
		}
		int du = _d.get(u, _d_version[old_version]);
		int dv = _d.get(v, _d_version[old_version]);
		if (du > dv) swap(u, v);
		_d.set(u, du+dv, _d_version[old_version]);
		_d.set(v, u);
		_d_version.push_back(_d.newest_version());
		_version = _d_version.size() - 1;
		return true;
	}
	bool same(int u, int v, int version=-1) {
		if (version == -1) version = _version;
		return find(u, version) == find(v, version);
	}
	int size(int v, int version=-1) {
		if (version == -1) version = _version;
		return -_d.get(find(v, version), _d_version[version]);
	}
private:
	persistent_vector<int> _d;
	vector<int> _d_version;
	int _version = 0;
};