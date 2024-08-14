#include <bits/stdc++.h>
using namespace std;

template<int mod>
struct static_convolution {
	using mint = static_mint<mod>;
	static_convolution(int n) {
		_n = n;
		_n2 = 1;
		while (_n2 < _n) _n2 *= 2;
		assert((mod - 1) % _n2 == 0);
		_init_primes();
		_root.resize(_n2+1);
		mint g = _primitive_root().pow((mod-1)/_n2);
		_root[0] = 1;
		for (int i = 0; i < _n2; i++) _root[i+1] = _root[i] * g;
	}
	vector<mint> operator()(vector<mint> &a, vector<mint> &b) {
		int n = a.size() + b.size() - 1;
		assert(n <= _n);
		vector<mint> a2(_n2, 0), b2(_n2, 0);
		for (int i = 0; i < a.size(); i++) a2[i] = a[i];
		for (int i = 0; i < b.size(); i++) b2[i] = b[i];
		_ntt(a2);
		_ntt(b2);
		for (int i = 0; i < _n2; i++) a2[i] *= b2[i];
		_ntt_inv(a2);
		vector<mint> c(n);
		for (int i = 0; i < n; i++) c[i] = a2[i];
		return c;
	}
private:
	int _n, _n2;
	const mint INV2 = mint(2).inv();
	vector<int> _primes;
	vector<mint> _root;
	void _init_primes() {
		int x = mod - 1;
		for (int i = 2; (long long)i * i <= x; i++) {
			if (x % i == 0) {
				_primes.push_back(i);
				while (x % i == 0) x /= i;
			}
		}
		if (x > 1) _primes.push_back(x);
	}
	mint _primitive_root() {
		mt19937 mt(time(nullptr));
		while (true) {
			mint g = mt();
			bool ok = true;
			for (auto &x : _primes) {
				if (g.pow((mod-1)/x) == 1) {
					ok = false;
					break;
				}
			}
			if (ok) return g;
		}
	}
	void _ntt(vector<mint> &v) {
		int n = v.size();
		if (n == 1) return;
		vector<mint> even(n/2), odd(n/2);
		for (int i = 0; i < n/2; i++) {
			even[i] = v[i*2];
			odd[i] = v[i*2+1];
		}
		_ntt(even);
		_ntt(odd);
		for (int i = 0; i < n/2; i++) {
			odd[i] *= _root[_n2/n*i];
			v[i] = even[i] + odd[i];
			v[n/2+i] = even[i] - odd[i];
		}
	}
	void _ntt_inv(vector<mint> &v) {
		int n = v.size();
		if (n == 1) return;
		vector<mint> even(n/2), odd(n/2);
		for (int i = 0; i < n/2; i++) {
			even[i] = v[i*2];
			odd[i] = v[i*2+1];
		}
		_ntt_inv(even);
		_ntt_inv(odd);
		for (int i = 0; i < n/2; i++) {
			odd[i] *= _root[_n2-_n2/n*i];
			v[i] = even[i] + odd[i];
			v[n/2+i] = even[i] - odd[i];
		}
		for (int i = 0; i < n; i++) v[i] *= INV2;
	}
};
using convolution = static_convolution<998244353>;