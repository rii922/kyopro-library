#include <bits/stdc++.h>
#include "montgomery_mint.cpp"
using namespace std;

/// EXPAND FROM HERE

template<class _int, class _uint, class _long, template<_uint mod> class static_mint, _uint mod>
struct ntt_friendly_convolution_base {
	using mint = static_mint<mod>;
	ntt_friendly_convolution_base() {}
	vector<mint> operator()(const vector<mint> &a, const vector<mint> &b) const {
		int n = a.size() + b.size() - 1;
		int n2 = 1;
		while (n2 < n) n2 *= 2;
		assert((mod - 1) % n2 == 0);
		vector<mint> root(n2+1), a2(n2, 0), b2(n2, 0);
		root[0] = 1;
		mint g = _pr.pow((mod-1)/n2);
		for (int i = 0; i < n2; i++) root[i+1] = root[i] * g;
		for (int i = 0; i < a.size(); i++) a2[i] = a[i];
		for (int i = 0; i < b.size(); i++) b2[i] = b[i];
		_ntt(a2, root);
		_ntt(b2, root);
		for (int i = 0; i < n2; i++) a2[i] *= b2[i];
		_intt(a2, root);
		mint ni = mint(n2).inv();
		vector<mint> c(n);
		for (int i = 0; i < n; i++) c[i] = a2[i] * ni;
		return c;
	}
	template<class T>
	vector<T> operator()(const vector<T> &a, const vector<T> &b) const {
		int n = a.size() + b.size() - 1;
		int n2 = 1;
		while (n2 < n) n2 *= 2;
		assert((mod - 1) % n2 == 0);
		vector<mint> root(n2+1), a2(n2, 0), b2(n2, 0);
		root[0] = 1;
		mint g = _pr.pow((mod-1)/n2);
		for (int i = 0; i < n2; i++) root[i+1] = root[i] * g;
		for (int i = 0; i < a.size(); i++) a2[i] = a[i];
		for (int i = 0; i < b.size(); i++) b2[i] = b[i];
		_ntt(a2, root);
		_ntt(b2, root);
		for (int i = 0; i < n2; i++) a2[i] *= b2[i];
		_intt(a2, root);
		mint ni = mint(n2).inv();
		vector<T> c(n);
		for (int i = 0; i < n; i++) c[i] = (a2[i] * ni).val();
		return c;
	}
private:
	static const mint _pr;
	void _ntt(vector<mint> &v, vector<mint> &root) const {
		if (v.size() <= 1) return;
		if (v.size() == 2) {
			mint v0 = v[0];
			v[0] = v0 + v[1];
			v[1] = v0 - v[1];
			return;
		}
		int e = __builtin_ctz(v.size());
		int d = e;
		int n2 = 1 << e;
		if (e & 1) {
			int b = n2 >> 1;
			for (int i = 0; i < b; i++) {
				mint s = v[i];
				mint t = v[i+b];
				v[i] = s + t;
				v[i+b] = (s - t) * root[n2-i];
			}
			d--;
		}
		int b = 1 << (d - 2);
		while (d >= 2) {
			for (int i = 0; i < n2; i += (b << 2)) {
				for (int j = 0; j < b; j++) {
					int p0 = i + j;
					int p1 = p0 + b;
					int p2 = p1 + b;
					int p3 = p2 + b;
					mint t0 = v[p0];
					mint t1 = v[p1];
					mint t2 = v[p2];
					mint t3 = v[p3];
					mint t0p2 = t0 + t2;
					mint t1p3 = t1 + t3;
					mint t0m2 = (t0 - t2) * root[n2-(j<<(e-d))];
					mint t1m3 = (t1 - t3) * root[n2-((j+b)<<(e-d))];
					v[p0] = t0p2 + t1p3;
					v[p1] = (t0p2 - t1p3) * root[n2-(j<<(e-d+1))];
					v[p2] = t0m2 + t1m3;
					v[p3] = (t0m2 - t1m3) * root[n2-(j<<(e-d+1))];
				}
			}
			d -= 2;
			b >>= 2;
		}
	}
	void _intt(vector<mint> &v, vector<mint> &root) const {
		if (v.size() <= 1) return;
		if (v.size() == 2) {
			mint v0 = v[0];
			v[0] = v0 + v[1];
			v[1] = v0 - v[1];
			return;
		}
		int e = __builtin_ctz(v.size());
		int d = 2;
		int n2 = 1 << e;
		int b = 1;
		while (d <= e) {
			for (int i = 0; i < n2; i += (b << 2)) {
				for (int j = 0; j < b; j++) {
					int p0 = i + j;
					int p1 = p0 + b;
					int p2 = p1 + b;
					int p3 = p2 + b;
					mint t0 = v[p0];
					mint t1 = v[p1] * root[j<<(e-d+1)];
					mint t2 = v[p2];
					mint t3 = v[p3] * root[j<<(e-d+1)];
					mint t0p1 = t0 + t1;
					mint t2p3 = (t2 + t3) * root[j<<(e-d)];
					mint t0m1 = t0 - t1;
					mint t2m3 = (t2 - t3) * root[(j+b)<<(e-d)];
					v[p0] = t0p1 + t2p3;
					v[p1] = t0m1 + t2m3;
					v[p2] = t0p1 - t2p3;
					v[p3] = t0m1 - t2m3;
				}
			}
			d += 2;
			b <<= 2;
		}
		if (e & 1) {
			b = n2 >> 1;
			for (int i = 0; i < b; i++) {
				mint s = v[i];
				mint t = v[i+b] * root[i];
				v[i] = s + t;
				v[i+b] = s - t;
			}
		}
	}
};
template<class _int, class _uint, class _long, template<_uint mod> class static_mint, _uint mod> const static_mint<mod> ntt_friendly_convolution_base<_int, _uint, _long, static_mint, mod>::_pr = []() {
	vector<_int> primes;
	_int x = mod - 1;
	for (_int i = 2; (_long)i * i <= x; i++) {
		if (x % i == 0) {
			primes.push_back(i);
			while (x % i == 0) x /= i;
		}
	}
	if (x > 1) primes.push_back(x);
	mt19937 mt(time(nullptr));
	while (true) {
		mint g = mt();
		if (g == 0) continue;
		bool ok = true;
		for (auto &y : primes) {
			if (g.pow((mod-1)/y) == 1) {
				ok = false;
				break;
			}
		}
		if (ok) return g;
	}
}();
template<uint32_t mod> using ntt_friendly_convolution = ntt_friendly_convolution_base<int32_t, uint32_t, int64_t, static_mint, mod>;
template<uint64_t mod> using ntt_friendly_convolution64 = ntt_friendly_convolution_base<int64_t, uint64_t, __int128_t, static_mint64, mod>;
template<class _int, class _uint, class _long, template<_uint mod> class ntt_friendly_convolution, template<_uint mod> class static_mint, _uint mod0, _uint mod1, _uint mod2, _uint mod>
struct arbitrary_convolution_base {
	using mint = static_mint<mod>;
	arbitrary_convolution_base() {}
	vector<mint> operator()(const vector<mint> &a, const vector<mint> &b) const {
		vector<_int> ia(a.size()), ib(b.size());
		for (int i = 0; i < a.size(); i++) ia[i] = a[i].val();
		for (int i = 0; i < b.size(); i++) ib[i] = b[i].val();
		vector<_int> ic0 = _conv0(ia, ib);
		vector<_int> ic1 = _conv1(ia, ib);
		vector<_int> ic2 = _conv2(ia, ib);
		vector<mint> c(ic0.size());
		_int w = (_long)mod0 * mod1 % mod;
		for (int i = 0; i < c.size(); i++) {
			_int t0 = ic0[i];
			_int t1 = (_long)(ic1[i] - t0 + mod1) * _r01 % mod1;
			_int t2 = ((_long)(ic2[i] - t0 + mod2) * _r02r12 + (_long)(-t1 + mod2) * _r12) % mod2;
			c[i] = (_long)t0 + (_long)t1 * mod0 + (_long)t2 * w;
		}
		return c;
	}
	template<class T>
	vector<T> operator()(const vector<T> &a, const vector<T> &b) const {
		vector<T> c0 = _conv0(a, b);
		vector<T> c1 = _conv1(a, b);
		vector<T> c2 = _conv2(a, b);
		vector<T> c(c0.size());
		_int w = (_long)mod0 * mod1 % mod;
		for (int i = 0; i < c.size(); i++) {
			_int t0 = c0[i];
			_int t1 = (_long)(c1[i] - t0 + mod1) * _r01 % mod1;
			_int t2 = ((_long)(c2[i] - t0 + mod2) * _r02r12 + (_long)(-t1 + mod2) * _r12) % mod2;
			c[i] = ((_long)t0 + (_long)t1 * mod0 + (_long)t2 * w) % mod;
		}
		return c;
	}
private:
	const ntt_friendly_convolution<mod0> _conv0;
	const ntt_friendly_convolution<mod1> _conv1;
	const ntt_friendly_convolution<mod2> _conv2;
	const _int _r01 = static_mint<mod1>(mod0).inv().val();
	const _int _r02 = static_mint<mod2>(mod0).inv().val();
	const _int _r12 = static_mint<mod2>(mod1).inv().val();
	const _int _r02r12 = (_long)_r02 * _r12 % mod2;
};
template<int32_t mod> using arbitrary_convolution = arbitrary_convolution_base<int32_t, uint32_t, int64_t, ntt_friendly_convolution, static_mint, 167772161, 469762049, 754974721, mod>;
template<int64_t mod> using arbitrary_convolution64 = arbitrary_convolution_base<int64_t, uint64_t, __int128_t, ntt_friendly_convolution64, static_mint64, 4604226931544555521ULL, 4605071356474687489ULL, 4610208274799656961ULL, mod>;
struct convolution_ull {
	convolution_ull() {}
	vector<uint64_t> operator()(const vector<uint64_t> &a, const vector<uint64_t> &b) const {
		vector<uint64_t> c0 = _conv0(a, b);
		vector<uint64_t> c1 = _conv1(a, b);
		vector<uint64_t> c2 = _conv2(a, b);
		vector<uint64_t> c(c0.size());
		uint64_t w = 4604226931544555521ULL * 4605071356474687489ULL;
		for (int i = 0; i < c.size(); i++) {
			uint64_t t0 = c0[i];
			uint64_t t1 = (__uint128_t)(c1[i] - t0 + 4605071356474687489ULL) * _r01 % 4605071356474687489ULL;
			uint64_t t2 = ((__uint128_t)(c2[i] - t0 + 4610208274799656961ULL) * _r02r12 + (__uint128_t)(-t1 + 4610208274799656961ULL) * _r12) % 4610208274799656961ULL;
			c[i] = (__uint128_t)t0 + (__uint128_t)t1 * 4604226931544555521ULL + (__uint128_t)t2 * w;
		}
		return c;
	}
private:
	const ntt_friendly_convolution64<4604226931544555521ULL> _conv0;
	const ntt_friendly_convolution64<4605071356474687489ULL> _conv1;
	const ntt_friendly_convolution64<4610208274799656961ULL> _conv2;
	const uint64_t _r01 = static_mint64<4605071356474687489ULL>(4604226931544555521ULL).inv().val();
	const uint64_t _r02 = static_mint64<4610208274799656961ULL>(4604226931544555521ULL).inv().val();
	const uint64_t _r12 = static_mint64<4610208274799656961ULL>(4605071356474687489ULL).inv().val();
	const uint64_t _r02r12 = (__int128_t)_r02 * _r12 % 4610208274799656961ULL;
};
using convolution = ntt_friendly_convolution<998244353>;
using convolution = arbitrary_convolution<1000000007>;