/**
 * @file fps.hpp
 * @author rii922
 * @brief 形式的冪級数を扱うクラス
 * @date 2025-02-27
 *
 * Verified with:
 * https://judge.yosupo.jp/submission/270157
 * https://judge.yosupo.jp/submission/270158
 * https://judge.yosupo.jp/submission/270159
 * https://judge.yosupo.jp/submission/270160
 */

#include <bits/stdc++.h>
#include "../custom_concepts.hpp"
#include "../mint/montgomery_mint.hpp"
#include "../convolution/convolution.hpp"
using namespace std;

/// EXPAND FROM HERE

/**
 * @brief 形式的冪級数を扱う汎用クラス。
 *
 * @tparam _uint `mod*4` を表現可能な符号無し整数型
 * @tparam convolution `operator()` で `mod` を法とした畳み込みを行うクラス
 * @tparam static_mint 自動で剰余を取る整数型
 * @tparam mod 法
 */
template<gnu_unsigned_integral _uint, template<_uint mod> class convolution, template<_uint mod> class static_mint, uint32_t mod>
struct fps_base {
	using fps = fps_base<_uint, convolution, static_mint, mod>;
	using mint = static_mint<mod>;
	fps_base(int n) : _v(n, 0) {}
	fps_base(int n, mint &x) : _v(n, x) {}
	fps_base(int n, mint &&x) : _v(n, x) {}
	fps_base(const vector<mint> &v) : _v(v) {}
	fps_base(initializer_list<mint> init) : _v(init) {}

	/**
	 * @brief 項の数を返す。係数が 0 の項も数える。
	 *
	 * @return 項の数
	 */
	int size() const {
		return _v.size();
	}

	/**
	 * @brief 項が 1 つもないかどうかを返す。
	 *
	 * @return 項が 1 つもないかどうか
	 */
	bool empty() const {
		return _v.empty();
	}

	/**
	 * @brief `x^p` の係数を取得する。
	 *
	 * @param p 指数
	 * @return `x^p` の係数
	 */
	const mint &operator[](int p) const { return _v[p]; }

	/**
	 * @brief `x^p` の係数を取得する。
	 *
	 * @param p 指数
	 * @return `x^p` の係数
	 */
	mint &operator[](int p) { return _v[p]; }
	fps operator+() const {
		return fps(*this);
	}
	fps operator-() const {
		fps res(_v.size());
		for (int i = 0; i < _v.size(); i++) res[i] = -_v[i];
		return res;
	}
	fps &operator+=(const fps &a) {
		if (_v.size() < a.size()) _v.resize(a.size());
		for (int i = 0; i < _v.size(); i++) _v[i] += a[i];
		return *this;
	}
	fps &operator+=(const mint &a) {
		if (_v.empty()) _v.resize(1);
		_v[0] += a;
		return *this;
	}
	fps &operator-=(const fps &a) {
		if (_v.size() < a.size()) _v.resize(a.size());
		for (int i = 0; i < a.size(); i++) _v[i] -= a[i];
		return *this;
	}
	fps &operator-=(const mint &a) {
		if (_v.empty()) _v.resize(1);
		_v[0] -= a;
		return *this;
	}
	fps &operator*=(const fps &a) {
		_v = _conv(_v, a._v);
		return *this;
	}
	fps &operator*=(const mint &a) {
		for (int i = 0; i < _v.size(); i++) _v[i] *= a;
		return *this;
	}
	fps &operator<<=(int sz) {
		if (sz < 0) return *this >>= -sz;
		if (_v.size() <= sz) return *this = {};
		_v.erase(_v.begin(), _v.begin()+sz);
		return *this;
	}
	fps &operator>>=(int sz) {
		if (sz < 0) return *this <<= -sz;
		_v.insert(_v.begin(), sz, 0);
		return *this;
	}
	fps &operator+=(const long long a) { return *this += mint(a); }
	fps &operator-=(const long long a) { return *this -= mint(a); }
	fps &operator*=(const long long a) { return *this *= mint(a); }
	fps operator+(const fps &a) const { return fps(*this) += a; }
	fps operator+(const mint &a) const { return fps(*this) += a; }
	fps operator+(const long long a) const { return fps(*this) += mint(a); }
	fps operator-(const fps &a) const { return fps(*this) -= a; }
	fps operator-(const mint &a) const { return fps(*this) -= a; }
	fps operator-(const long long a) const { return fps(*this) -= mint(a); }
	fps operator*(const fps &a) const { return fps(*this) *= a; }
	fps operator*(const mint &a) const { return fps(*this) *= a; }
	fps operator*(const long long a) const { return fps(*this) *= mint(a); }
	friend fps operator+(const mint a, const fps b) { return b + a; }
	friend fps operator-(const mint a, const fps b) { return -b + a; }
	friend fps operator*(const mint a, const fps b) { return b * a; }
	fps operator<<(int sz) const { return fps(*this) <<= sz; }
	fps operator>>(int sz) const { return fps(*this) >>= sz; }

	/**
	 * @brief 先頭 `sz` 項を返す。
	 *
	 * @param sz 項数
	 * @return 先頭 `sz` 項
	 */
	fps pre(int sz) const {
		fps res(sz, 0);
		int m = min((int)_v.size(), sz);
		for (int i = 0; i < m; i++) res[i] = _v[i];
		return res;
	}

	/**
	 * @brief `fg = 1` となる逆数 `g` をダブリングによって求める。定数項が 0 であってはいけない。
	 *
	 * O(N log N)
	 * @param deg 求める項数。指定しない場合、 `size()` が使われる。
	 * @return 逆数
	 */
	fps inv(int deg=-1) const {
		assert(!_v.empty() && _v[0] != 0);
		if (deg == -1) deg = _v.size();
		fps res = {_v[0].inv()};
		for (int i = 1; i < deg; i <<= 1) {
			res = (res + res - res * res * pre(i<<1)).pre(i<<1);
		}
		res = res.pre(deg);
		return res;
	}
	fps &operator/=(const fps &a) {
		*this *= a.inv(max((int)_v.size(), a.size()));
		return *this;
	}
	fps &operator/=(const mint &a) {
		mint ai = a.inv();
		for (int i = 0; i < _v.size(); i++) _v[i] *= ai;
		return *this;
	}
	fps operator/(const fps &a) const { return fps(*this) /= a; }
	fps operator/(const mint &a) const { return fps(*this) /= a; }
	fps operator/(const long long a) const { return fps(*this) /= mint(a); }
	friend fps operator/(const mint a, const fps b) { return b.inv() * a; }

	/**
	 * @brief 形式微分された形式的冪級数を求める。
	 *
	 * O(N)
	 * @return 形式微分された形式的冪級数
	 */
	fps diff() const {
		fps res(_v.size()-1);
		for (int i = 1; i < _v.size(); i++) res[i-1] = _v[i] * i;
		return res;
	}

	/**
	 * @brief 形式積分された形式的冪級数を求める。定数項は 0 となる。
	 *
	 * O(N)
	 * @return 形式積分された形式的冪級数
	 */
	fps integral() const {
		fps res(_v.size()+1);
		res[0] = 0;
		for (int i = 0; i < _v.size(); i++) res[i+1] = _v[i] / (i + 1);
		return res;
	}

	/**
	 * @brief 対数 `log f` を返す。定数項が 1 である必要がある。
	 *
	 * O(N log N)
	 * @param deg 求める項数。指定しない場合、 `size()` が使われる。
	 * @return 対数
	 */
	fps log(int deg=-1) const {
		assert(!_v.empty() && _v[0] == 1);
		if (deg == -1) deg = _v.size();
		return (diff() / pre(deg)).pre(deg-1).integral();
	}

	/**
	 * @brief 指数 `exp f` を Newton 法によって求める。定数項が 0 である必要がある。
	 *
	 * O(N log N)
	 * @param deg 求める項数。指定しない場合、 `size()` が使われる。
	 * @return 指数
	 */
	fps exp(int deg=-1) const {
		assert(!_v.empty() && _v[0] == 0);
		if (deg == -1) deg = _v.size();
		fps res = {1};
		for (int i = 1; i < deg; i <<= 1) {
			res = (res * (pre(i<<1) + 1 - res.log(i<<1))).pre(i<<1);
		}
		res = res.pre(deg);
		return res;
	}

	/**
	 * @brief 冪乗 `f^t` を求める。
	 *
	 * O(N log N)
	 * @param t 指数
	 * @param deg 求める項数。指定しない場合、 `size()` が使われる。
	 * @return 冪乗
	 */
	fps pow(long long t, int deg=-1) const {
		if (deg == -1) deg = _v.size();
		if (t == 0) {
			fps res(deg, 0);
			if (deg > 0) res[0] = 1;
			return res;
		}
		if (t < 0) return pow(-t, deg).inv();
		long long z = 0;
		for (int i = 0; i < _v.size(); i++) {
			if (_v[i] != 0) {
				fps res = (((*this << i) / _v[i]).log(deg) * t).exp(deg);
				res *= _v[i].pow(t);
				res = (res >> (i * t)).pre(deg);
				return res;
			}
			z += t;
			if (z >= deg) return fps(deg, 0);
		}
		return fps(deg, 0);
	}
	friend istream &operator>>(istream &is, fps &f) {
		for (int i = 0; i < f.size(); i++) is >> f[i];
		return is;
	}
	friend ostream &operator<<(ostream &os, const fps &f) {
		for (int i = 0; i < f.size(); i++) {
			if (i) os << ' ';
			os << f[i];
		}
		return os;
	}
private:
	constexpr static convolution<mod> _conv {};
	vector<mint> _v;
};

/**
 * @brief NTT-friendly 32 bit 素数 mod による形式的冪級数を扱うクラス
 *
 * @tparam mod `mod*4` が `uint32_t` に収まるような NTT-friendly 奇素数
 */
template<uint32_t mod> using ntt_friendly_fps = fps_base<uint32_t, ntt_friendly_convolution, static_mint, mod>;

/**
 * @brief 32 bit mod による形式的冪級数を扱うクラス
 *
 * @tparam mod `mod*4` が `uint32_t` に収まるような奇数
 */
template<uint32_t mod> using arbitrary_fps = fps_base<uint32_t, arbitrary_convolution, static_mint, mod>;

/**
 * @brief NTT-friendly 64 bit 素数 mod による形式的冪級数を扱うクラス
 *
 * @tparam mod `mod*4` が `uint64_t` に収まるような NTT-friendly 奇素数
 */
template<uint64_t mod> using ntt_friendly_fps64 = fps_base<uint64_t, ntt_friendly_convolution64, static_mint64, mod>;

/**
 * @brief 64 bit mod による形式的冪級数を扱うクラス
 *
 * @tparam mod `mod*4` が `uint64_t` に収まるような奇数
 */
template<uint64_t mod> using arbitrary_fps64 = fps_base<uint64_t, arbitrary_convolution64, static_mint64, mod>;

// using fps = ntt_friendly_fps<998244353>;
// using fps = arbitrary_fps<1000000007>;