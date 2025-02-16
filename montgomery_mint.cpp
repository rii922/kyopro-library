/**
 * @file montgomery_mint.cpp
 * @author rii922
 * @brief Montgomery 乗算によって自動で剰余を取る整数型。 @c mod が実行前に決まっている場合に使用する。
 * @date 2025-02-16
 */

#include <bits/stdc++.h>
using namespace std;

/**
 * @brief Montgomery 乗算によって自動で剰余を取る汎用クラス。 @c mod が実行前に決まっている場合に使用する。
 *
 * @tparam _int @c _uint と同じ精度の符号付き整数型
 * @tparam _uint @c mod*4 を表現可能な符号無し整数型
 * @tparam _long @c _int の倍の精度の符号付き整数型
 * @tparam _ulong @c _uint の倍の精度の符号無し整数型
 * @tparam mod @c mod*4 が @c _uint に収まるような奇数
 */
template<class _int, class _uint, class _long, class _ulong, _uint mod>
struct static_montgomery_mint {
	using mint = static_montgomery_mint<_int, _uint, _long, _ulong, mod>;
	static _uint get_mod() {
		return mod;
	}
	static_montgomery_mint() : _xr(0) {}
	static_montgomery_mint(_int x) : _xr(_reduce(_ulong(x%_long(mod)+mod)*_r2)) {}
	static_montgomery_mint(_uint x) : _xr(_reduce(_ulong(x%_long(mod)+mod)*_r2)) {}
	static_montgomery_mint(_long x) : _xr(_reduce(_ulong(x%_long(mod)+mod)*_r2)) {}
	static_montgomery_mint(_ulong x) : _xr(_reduce(_ulong(x%_ulong(mod)+mod)*_r2)) {}
	template<class T> static_montgomery_mint(T x) : _xr(_reduce(_ulong(x%_long(mod)+mod)*_r2)) {}
	_uint val() const {
		_uint x = _reduce(_xr);
		return x >= mod ? x - mod : x;
	}
	mint operator+() const { return mint(*this); }
	mint operator-() const { return mint(0) - mint(*this); }
	mint &operator+=(const mint &a) {
		if (_int(_xr += a._xr - mod * 2) < 0) _xr += mod * 2;
		return *this;
	}
	mint &operator-=(const mint &a) {
		if (_int(_xr -= a._xr) < 0) _xr += mod * 2;
		return *this;
	}
	mint &operator*=(const mint &a) {
		_xr = _reduce(_ulong(_xr) * a._xr);
		return *this;
	}
	mint &operator++() {
		*this += 1;
		return *this;
	}
	mint operator++(int) {
		mint temp = *this;
		++*this;
		return temp;
	}
	mint &operator--() {
		*this -= 1;
		return *this;
	}
	mint operator--(int) {
		mint temp = *this;
		--*this;
		return temp;
	}
	mint operator+(const mint &a) const { return mint(*this) += a; }
	mint operator-(const mint &a) const { return mint(*this) -= a; }
	mint operator*(const mint &a) const { return mint(*this) *= a; }
	friend mint operator+(const _long a, const mint b) { return mint(a) + b; }
	friend mint operator-(const _long a, const mint b) { return mint(a) - b; }
	friend mint operator*(const _long a, const mint b) { return mint(a) * b; }

	/**
	 * @brief 繰り返し 2 乗法によって冪乗を計算する。負の冪乗では逆元の冪乗となり、 @c mod が素数である必要がある。
	 *
	 * @param t 指数
	 * @return mint 冪乗
	 */
	mint pow(_long t) const {
		if (t < 0) return pow(-t).inv();
		mint res = 1;
		mint mul = *this;
		while (t) {
			if (t & 1) res *= mul;
			mul *= mul;
			t >>= 1;
		}
		return res;
	}

	/**
	 * @brief 拡張ユークリッドの互除法によって逆元を求める。 @c mod が素数である必要がある。
	 *
	 * @return mint 逆元
	 */
	mint inv() const {
		_int x = val();
		_int y = mod;
		_int u = 1;
		_int v = 0;
		while (y) {
			_int t = x / y;
			x -= y * t;
			u -= v * t;
			swap(x, y);
			swap(u, v);
		}
		return mint(u);
	}
	mint &operator/=(const mint &a) {
		*this *= a.inv();
		return *this;
	}
	mint operator/(const mint &a) const { return mint(*this) /= a; }
	friend mint operator/(const _long a, const mint b) { return mint(a) / b; }
	bool operator==(const mint &a) const {
		return (_xr >= mod ? _xr - mod : _xr) == (a._xr >= mod ? a._xr - mod : a._xr);
	}
	bool operator!=(const mint &a) const {
		return (_xr >= mod ? _xr - mod : _xr) != (a._xr >= mod ? a._xr - mod : a._xr);
	}
	friend istream &operator>>(istream &is, mint &a) {
		_int x;
		is >> x;
		a = mint(x);
		return is;
	}
	friend ostream &operator<<(ostream &os, const mint &a) {
		_int x = a.val();
		os << x;
		return os;
	}
private:
	static _uint _r2, _ninv, _sz;
	_uint _xr;
	_uint _reduce(const _ulong &a) const {
		return (a + _ulong(_uint(a) * _uint(-_ninv)) * mod) >> _sz;
	}
};
template<class _int, class _uint, class _long, class _ulong, _uint mod> _uint static_montgomery_mint<_int, _uint, _long, _ulong, mod>::_r2 = -_ulong(mod) % mod;
template<class _int, class _uint, class _long, class _ulong, _uint mod> _uint static_montgomery_mint<_int, _uint, _long, _ulong, mod>::_ninv = []() {
	_uint res = mod;
	while (mod * res != 1) res *= _uint(2) - mod * res;
	return res;
}();
template<class _int, class _uint, class _long, class _ulong, _uint mod> _uint static_montgomery_mint<_int, _uint, _long, _ulong, mod>::_sz = sizeof(_uint) * 8;

/**
 * @brief Montgomery 乗算によって自動で剰余を取る 32 bit 整数。
 *
 * @tparam mod 奇数
 */
template<uint32_t mod> using static_mint = static_montgomery_mint<int32_t, uint32_t, int64_t, uint64_t, mod>;

/**
 * @brief Montgomery 乗算によって自動で剰余を取る 64 bit 整数。
 *
 * @tparam mod 奇数
 */
template<uint64_t mod> using static_mint64 = static_montgomery_mint<int64_t, uint64_t, __int128_t, __uint128_t, mod>;

// using mint = static_mint<1000000007>;
// using mint64 = static_mint64<1000000007>;
using mint = static_mint<998244353>;
using mint64 = static_mint64<998244353>;