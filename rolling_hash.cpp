#include <bits/stdc++.h>
#include "montgomery_mint.cpp"
using namespace std;

/// EXPAND FROM HERE

template<class T=int, int base_size=2>
struct rolling_hash {
	rolling_hash(int n) {
		_init_base();
		_init_data(n);
	}
	rolling_hash(int n, array<int, base_size> &base) : _base(base) {
		_init_data(n);
	}
	rolling_hash(vector<T> &v) {
		_init_base();
		_init_data(v);
	}
	rolling_hash(vector<T> &v, array<int, base_size> &base) : _base(base) {
		_init_data(v);
	}
	rolling_hash(string &s) {
		vector<int> v(s.size());
		for (int i = 0; i < s.size(); i++) v[i] = s[i];
		_init_base();
		_init_data(v);
	}
	rolling_hash(string &s, array<int, base_size> &base) : _base(base) {
		vector<int> v(s.size());
		for (int i = 0; i < s.size(); i++) v[i] = s[i];
		_init_data(v);
	}
	static array<int, base_size> generate_base() {
		array<int, base_size> base;
		mt19937 mt(time(nullptr));
		for (int i = 0; i < base_size; i++) base[i] = mt();
		return base;
	}
	void set(int p, T x) {
		for (int i = 0; i < base_size; i++) {
			_value998244353[i].set(p, static_mint<998244353>(_base[i]).pow(p)*x);
			_value1000000007[i].set(p, static_mint<1000000007>(_base[i]).pow(p)*x);
			_value1000000009[i].set(p, static_mint<1000000009>(_base[i]).pow(p)*x);
			_value1000000021[i].set(p, static_mint<1000000021>(_base[i]).pow(p)*x);
			_value1000000033[i].set(p, static_mint<1000000033>(_base[i]).pow(p)*x);
		}
	}
	array<tuple<static_mint<998244353>, static_mint<1000000007>, static_mint<1000000009>, static_mint<1000000021>, static_mint<1000000033>>, base_size> get(int l, int r) {
		array<tuple<static_mint<998244353>, static_mint<1000000007>, static_mint<1000000009>, static_mint<1000000021>, static_mint<1000000033>>, base_size> res;
		for (int i = 0; i < base_size; i++) {
			res[i] = {
				_value998244353[i].sum(l, r)*static_mint<998244353>(_base[i]).pow(-l),
				_value1000000007[i].sum(l, r)*static_mint<1000000007>(_base[i]).pow(-l),
				_value1000000009[i].sum(l, r)*static_mint<1000000009>(_base[i]).pow(-l),
				_value1000000021[i].sum(l, r)*static_mint<1000000021>(_base[i]).pow(-l),
				_value1000000033[i].sum(l, r)*static_mint<1000000033>(_base[i]).pow(-l),
			};
		}
		return res;
	}
private:
	array<fenwick_tree<static_mint<998244353>>, base_size> _value998244353;
	array<fenwick_tree<static_mint<1000000007>>, base_size> _value1000000007;
	array<fenwick_tree<static_mint<1000000009>>, base_size> _value1000000009;
	array<fenwick_tree<static_mint<1000000021>>, base_size> _value1000000021;
	array<fenwick_tree<static_mint<1000000033>>, base_size> _value1000000033;
	array<int, base_size> _base;
	int _n;
	void _init_base() {
		_base = generate_base();
	}
	template<int mod>
	void _init_value(int n, array<fenwick_tree<static_mint<mod>>, base_size> &value) {
		for (int i = 0; i < base_size; i++) value[i] = fenwick_tree<static_mint<mod>>(n);
	}
	template<int mod>
	void _init_value(vector<T> &v, array<fenwick_tree<static_mint<mod>>, base_size> &value) {
		_init_value(v.size(), value);
		for (int i = 0; i < base_size; i++) {
			static_mint<mod> p = 1;
			for (int j = 0; j < v.size(); j++) {
				value[i].add(j, p * v[j]);
				p *= _base[i];
			}
		}
	}
	void _init_data(int n) {
		_n = n;
		_init_value<998244353>(_n, _value998244353);
		_init_value<1000000007>(_n, _value1000000007);
		_init_value<1000000009>(_n, _value1000000009);
		_init_value<1000000021>(_n, _value1000000021);
		_init_value<1000000033>(_n, _value1000000033);
	}
	void _init_data(vector<T> &v) {
		_n = v.size();
		_init_value<998244353>(v, _value998244353);
		_init_value<1000000007>(v, _value1000000007);
		_init_value<1000000009>(v, _value1000000009);
		_init_value<1000000021>(v, _value1000000021);
		_init_value<1000000033>(v, _value1000000033);
	}
};