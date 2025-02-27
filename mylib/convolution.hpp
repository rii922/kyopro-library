/**
 * @file convolution.hpp
 * @author rii922
 * @brief 様々な mod に関する畳み込み
 * @date 2025-02-27
 *
 * Verified with:
 * https://judge.yosupo.jp/submission/269470
 * https://judge.yosupo.jp/submission/269476
 * https://judge.yosupo.jp/submission/269483
 * https://judge.yosupo.jp/submission/270107
 */

#include <bits/stdc++.h>
#include "custom_concepts.hpp"
#include "montgomery_mint.hpp"
#include "primitive_root.hpp"
using namespace std;

/// EXPAND FROM HERE

/**
 * @brief NTT-friendly 素数 mod に関する畳み込みを提供する汎用クラス。
 *
 * @tparam _int `_uint` と同じ精度の符号付き整数型
 * @tparam _uint `mod*4` を表現可能な符号無し整数型
 * @tparam _long `_int` の倍の精度の符号付き整数型
 * @tparam mint 自動で剰余を取る整数型
 */
template<gnu_signed_integral _int, gnu_unsigned_integral _uint, gnu_signed_integral _long, template<_uint mod> class static_mint, _uint mod>
requires requires {
	requires sizeof(_int) == sizeof(_uint);
	requires sizeof(_int)*2 == sizeof(_long);
	{static_mint<mod>::get_mod()} -> same_as<_uint>;
	{static_mint<mod>::primitive_root()} -> same_as<static_mint<mod>>;
}
struct ntt_friendly_convolution_base {
	using mint = static_mint<mod>;
	constexpr ntt_friendly_convolution_base() {}

	/**
	 * @brief 高速 FFT および NTT によって NTT-friendly 素数 mod に関する畳み込みを行う。
	 *
	 * O(N log N)
	 * @param a, b `mod = d*2^s + 1` のとき、 `a.size()+b.size()-1 <= 2^s` である必要がある
	 * @return `a` と `b` の畳み込みである、長さ `a.size()+b.size()-1` の `vector`
	 */
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

	/**
	 * @brief 高速 FFT および NTT によって NTT-friendly 素数 mod に関する畳み込みを行う。
	 *
	 * O(N log N)
	 * @param a, b `mod = d*2^s + 1` のとき、 `a.size()+b.size()-1 <= 2^s` である必要がある
	 * @return `a` と `b` の畳み込みである、長さ `a.size()+b.size()-1` の `vector`
	 */
	template<gnu_integral T>
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

	/**
	 * @brief 高速 FFT および NTT によって NTT-friendly 素数 mod に関する畳み込みを行う。 `mod = d*2^s + 1` のとき、 `a.size()+b.size()-1 <= 2^s` が満たされない場合に使う。
	 *
	 * @param a, b `vector`
	 * @return `a` と `b` の畳み込みである、長さ `a.size()+b.size()-1` の `vector`
	 */
	vector<mint> large(const vector<mint> &a, const vector<mint> &b) const {
		int n = a.size() + b.size() - 1;
		int n2 = 1 << __builtin_ctz(mod-1);
		vector<mint> root(n2+1);
		root[0] = 1;
		mint g = _pr.pow((mod-1)/n2);
		for (int i = 0; i < n2; i++) root[i+1] = root[i] * g;
		vector<vector<mint>> as, bs;
		for (int i = 0; i < a.size(); i += n2/2) {
			vector<mint> a2(n2, 0);
			int len = min<int>(a.size()-i, n2/2);
			for (int j = 0; j < len; j++) a2[j] = a[i+j];
			_ntt(a2, root);
			as.push_back(a2);
		}
		for (int i = 0; i < b.size(); i += n2/2) {
			vector<mint> b2(n2, 0);
			int len = min<int>(b.size()-i, n2/2);
			for (int j = 0; j < len; j++) b2[j] = b[i+j];
			_ntt(b2, root);
			bs.push_back(b2);
		}
		vector<vector<mint>> cs(as.size()+bs.size()-1, vector<mint>(n2, 0));
		for (int i = 0; i < as.size(); i++) {
			for (int j = 0; j < bs.size(); j++) {
				for (int k = 0; k < n2; k++) cs[i+j][k] += as[i][k] * bs[j][k];
			}
		}
		for (int i = 0; i < cs.size(); i++) _intt(cs[i], root);
		vector<mint> c(n);
		for (int i = 0; i < cs.size(); i++) {
			int offset = n2/2*i;
			int len = min<int>(c.size()-offset, n2);
			for (int j = 0; j < len; j++) c[j+offset] += cs[i][j];
		}
		mint ni = mint(n2).inv();
		for (int i = 0; i < n; i++) c[i] *= ni;
		return c;
	}

	/**
	 * @brief 高速 FFT および NTT によって NTT-friendly 素数 mod に関する畳み込みを行う。 `mod = d*2^s + 1` のとき、 `a.size()+b.size()-1 <= 2^s` が満たされない場合に使う。
	 *
	 * @param a, b `vector`
	 * @return `a` と `b` の畳み込みである、長さ `a.size()+b.size()-1` の `vector`
	 */
	template<gnu_integral T>
	vector<T> large(const vector<T> &a, const vector<T> &b) const {
		int n = a.size() + b.size() - 1;
		int n2 = 1 << __builtin_ctz(mod-1);
		vector<mint> root(n2+1);
		root[0] = 1;
		mint g = _pr.pow((mod-1)/n2);
		for (int i = 0; i < n2; i++) root[i+1] = root[i] * g;
		vector<vector<mint>> as, bs;
		for (int i = 0; i < a.size(); i += n2/2) {
			vector<mint> a2(n2, 0);
			int len = min<int>(a.size()-i, n2/2);
			for (int j = 0; j < len; j++) a2[j] = a[i+j];
			_ntt(a2, root);
			as.push_back(a2);
		}
		for (int i = 0; i < b.size(); i += n2/2) {
			vector<mint> b2(n2, 0);
			int len = min<int>(b.size()-i, n2/2);
			for (int j = 0; j < len; j++) b2[j] = b[i+j];
			_ntt(b2, root);
			bs.push_back(b2);
		}
		vector<vector<mint>> cs(as.size()+bs.size()-1, vector<mint>(n2, 0));
		for (int i = 0; i < as.size(); i++) {
			for (int j = 0; j < bs.size(); j++) {
				for (int k = 0; k < n2; k++) cs[i+j][k] += as[i][k] * bs[j][k];
			}
		}
		for (int i = 0; i < cs.size(); i++) _intt(cs[i], root);
		vector<mint> cm(n);
		for (int i = 0; i < cs.size(); i++) {
			int offset = n2/2*i;
			int len = min<int>(cm.size()-offset, n2);
			for (int j = 0; j < len; j++) cm[j+offset] += cs[i][j];
		}
		mint ni = mint(n2).inv();
		vector<T> c(n);
		for (int i = 0; i < n; i++) c[i] = (cm[i] * ni).val();
		return c;
	}
private:
	constexpr static mint _pr = mint::primitive_root();
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

/**
 * @brief NTT-friendly 32 bit 素数 mod に関する畳み込みを提供するクラス。
 *
 * @tparam mod `mod*4` が `uint32_t` に収まるような NTT-friendly 奇素数
 */
template<uint32_t mod> using ntt_friendly_convolution = ntt_friendly_convolution_base<int32_t, uint32_t, int64_t, static_mint, mod>;

/**
 * @brief NTT-friendly 64 bit 素数 mod に関する畳み込みを提供するクラス。
 *
 * @tparam mod `mod*4` が `uint64_t` に収まるような NTT-friendly 奇素数
 */
template<uint64_t mod> using ntt_friendly_convolution64 = ntt_friendly_convolution_base<int64_t, uint64_t, __int128_t, static_mint64, mod>;

/**
 * @brief NTT-friendly とは限らない mod に関する畳み込みを提供する汎用クラス。
 *
 * @tparam _int `_uint` と同じ精度の符号付き整数型
 * @tparam _uint `mod*4` を表現可能な符号無し整数型
 * @tparam _long `_int` の倍の精度の符号付き整数型
 * @tparam mint 自動で剰余を取る整数型
 */
template<gnu_signed_integral _int, gnu_unsigned_integral _uint, gnu_signed_integral _long, template<_uint mod> class ntt_friendly_convolution, template<_uint mod> class static_mint, _uint mod0, _uint mod1, _uint mod2, _uint mod>
requires requires {
	requires sizeof(_int) == sizeof(_uint);
	requires sizeof(_int)*2 == sizeof(_long);
	{static_mint<mod>::get_mod()} -> same_as<_uint>;
	{static_mint<mod>::primitive_root()} -> same_as<static_mint<mod>>;
}
struct arbitrary_convolution_base {
	using mint = static_mint<mod>;
	constexpr arbitrary_convolution_base() {}

	/**
	 * @brief 3 つの NTT-friendly 素数および Garner のアルゴリズムによって一般の mod に関する畳み込みを行う。
	 *
	 * O(N log N)
	 * @param a, b `mod = d*2^s + 1` のとき、 `a.size()+b.size()-1 <= 2^s` である必要がある
	 * @return `a` と `b` の畳み込みである、長さ `a.size()+b.size()-1` の `vector`
	 */
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

	/**
	 * @brief 3 つの NTT-friendly 素数および Garner のアルゴリズムによって一般の mod に関する畳み込みを行う。
	 *
	 * O(N log N)
	 * @param a, b `mod = d*2^s + 1` のとき、 `a.size()+b.size()-1 <= 2^s` である必要がある
	 * @return `a` と `b` の畳み込みである、長さ `a.size()+b.size()-1` の `vector`
	 */
	template<gnu_integral T>
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
	constexpr static ntt_friendly_convolution<mod0> _conv0 {};
	constexpr static ntt_friendly_convolution<mod1> _conv1 {};
	constexpr static ntt_friendly_convolution<mod2> _conv2 {};
	constexpr static _int _r01 = static_mint<mod1>(mod0).inv().val();
	constexpr static _int _r02 = static_mint<mod2>(mod0).inv().val();
	constexpr static _int _r12 = static_mint<mod2>(mod1).inv().val();
	constexpr static _int _r02r12 = (_long)_r02 * _r12 % mod2;
};

/**
 * @brief 32 bit mod に関する畳み込みを提供するクラス。
 *
 * @tparam mod `mod*4` が `uint32_t` に収まるような奇数
 */
template<int32_t mod> using arbitrary_convolution = arbitrary_convolution_base<int32_t, uint32_t, int64_t, ntt_friendly_convolution, static_mint, 167772161, 469762049, 754974721, mod>;

/**
 * @brief 64 bit mod に関する畳み込みを提供するクラス。
 *
 * @tparam mod `mod*4` が `uint64_t` に収まるような奇数
 */
template<int64_t mod> using arbitrary_convolution64 = arbitrary_convolution_base<int64_t, uint64_t, __int128_t, ntt_friendly_convolution64, static_mint64, 4604226931544555521ULL, 4605071356474687489ULL, 4610208274799656961ULL, mod>;

/**
 * @brief `2^64` を mod とした畳み込みを提供するクラス。
 *
 */
struct convolution_ull {
	constexpr convolution_ull() {}

	/**
	 * @brief 3 つの NTT-friendly 素数および Garner のアルゴリズムによって `2^64` に関する畳み込みを行う。
	 *
	 * O(N log N)
	 * @param a, b `mod = d*2^s + 1` のとき、 `a.size()+b.size()-1 <= 2^s` である必要がある
	 * @return `a` と `b` の畳み込みである、長さ `a.size()+b.size()-1` の `vector`
	 */
	template<class T>
	requires same_as<T, uint64_t> || same_as<T, unsigned long long>
	vector<T> operator()(const vector<T> &a, const vector<T> &b) const {
		vector<T> c0 = _conv0(a, b);
		vector<T> c1 = _conv1(a, b);
		vector<T> c2 = _conv2(a, b);
		vector<T> c(c0.size());
		constexpr T w = mod0 * mod1;
		for (int i = 0; i < c.size(); i++) {
			T t0 = c0[i];
			T t1 = (__uint128_t)(c1[i] - t0 + mod1) * _r01 % mod1;
			T t2 = ((__uint128_t)(c2[i] - t0 + mod2) * _r02r12 + (__uint128_t)(-t1 + mod2) * _r12) % mod2;
			c[i] = (__uint128_t)t0 + (__uint128_t)t1 * mod0 + (__uint128_t)t2 * w;
		}
		return c;
	}
private:
	constexpr static uint64_t mod0 = 4604226931544555521ULL;
	constexpr static uint64_t mod1 = 4605071356474687489ULL;
	constexpr static uint64_t mod2 = 4610208274799656961ULL;
	constexpr static ntt_friendly_convolution64<mod0> _conv0 {};
	constexpr static ntt_friendly_convolution64<mod1> _conv1 {};
	constexpr static ntt_friendly_convolution64<mod2> _conv2 {};
	constexpr static uint64_t _r01 = static_mint64<mod1>(mod0).inv().val();
	constexpr static uint64_t _r02 = static_mint64<mod2>(mod0).inv().val();
	constexpr static uint64_t _r12 = static_mint64<mod2>(mod1).inv().val();
	constexpr static uint64_t _r02r12 = (__int128_t)_r02 * _r12 % mod2;
};

// using convolution = ntt_friendly_convolution<998244353>;
// using convolution = arbitrary_convolution<1000000007>;