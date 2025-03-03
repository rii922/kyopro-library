/**
 * @file persistent_union_find.hpp
 * @author rii922
 * @brief 完全永続配列を利用した、完全永続素集合データ構造
 * @date 2025-02-24
 *
 * Verified with:
 * https://judge.yosupo.jp/submission/268213
 */

#include <bits/stdc++.h>
#include "persistent_vector.hpp"
using namespace std;

/// EXPAND FROM HERE

/**
 * @brief 完全永続配列を利用した、完全永続素集合データ構造
 *
 */
struct persistent_union_find {
	/**
	 * @brief `n` 頂点 0 辺のグラフで初期化し、バージョン 0 とする。
	 *
	 * O(n)
	 * @param n 頂点数
	 */
	persistent_union_find(int n) : _d(n, -1) {
		_d_version.push_back(_d.newest_version());
	}

	/**
	 * @brief 参照するバージョンを変更する。
	 *
	 * O(1)
	 * @param version 新たに参照するバージョン番号
	 */
	void set_version(int version) {
		assert(version < _d_version.size());
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
		return _d_version.size() - 1;
	}

	/**
	 * @brief 指定したバージョンでの、頂点 `v` を含む連結成分の根を返す。
	 *
	 * O((log n)^2)
	 * @param v 頂点
	 * @param version バージョン番号。指定しない場合、現在参照しているバージョン番号が使われる。
	 * @return 指定したバージョンでの、頂点 `v` を含む連結成分の根
	 */
	int find(int v, int version=-1) {
		if (version == -1) version = _version;
		int d = _d.get(v, _d_version[version]);
		if (d < 0) return v;
		return find(d, version);
	}

	/**
	 * @brief 指定したバージョンで頂点 `u` と `v` を同じ連結成分に所属させ、それを最新のバージョンとし、さらにそれを参照するようにする。データ構造に変化がなくてもバージョン番号は加算される。
	 *
	 * O((log n)^2)
	 * @param u 頂点
	 * @param v 頂点
	 * @param old_version 変更を加えるバージョン番号。指定しない場合、現在参照しているバージョン番号が使われる。
	 * @return `u` と `v` が新たに連結になったなら `true` 、既に連結だったなら `false`
	 */
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

	/**
	 * @brief 指定したバージョンで頂点 `u` と `v` が同じ連結成分に属するかを判定する。
	 *
	 * O((log n)^2)
	 * @param u 頂点
	 * @param v 頂点
	 * @param version バージョン番号。指定しない場合、現在参照しているバージョン番号が使われる。
	 * @return 指定したバージョンで頂点 `u` と `v` が同じ連結成分に属するかどうか
	 */
	bool same(int u, int v, int version=-1) {
		if (version == -1) version = _version;
		return find(u, version) == find(v, version);
	}

	/**
	 * @brief 指定したバージョンで頂点 `v` を含む連結成分の頂点数を返す。
	 *
	 * O((log n)^2)
	 * @param v 頂点
	 * @param version バージョン番号。指定しない場合、現在参照しているバージョン番号が使われる。
	 * @return 指定したバージョンで頂点 `v` を含む連結成分の頂点数
	 */
	int size(int v, int version=-1) {
		if (version == -1) version = _version;
		return -_d.get(find(v, version), _d_version[version]);
	}
private:
	persistent_vector<int> _d;
	vector<int> _d_version;
	int _version = 0;
};