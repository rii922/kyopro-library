/**
 * @file montgomery_mint.hpp
 * @author rii922
 * @brief Montgomery 乗算によって自動で剰余を取る整数型。 `mod` が実行前に決まっている場合に使用する。
 * @date 2025-02-24
 */

#include <bits/stdc++.h>
#include "custom_concepts.hpp"
#include "is_prime.hpp"
using namespace std;

/// EXPAND FROM HERE

/**
 * @brief Montgomery 乗算によって自動で剰余を取る汎用クラス。 `mod` が実行前に決まっている場合に使用する。
 *
 * @tparam _int `_uint` と同じ精度の符号付き整数型
 * @tparam _uint `mod*4` を表現可能な符号無し整数型
 * @tparam _long `_int` の倍の精度の符号付き整数型
 * @tparam _ulong `_uint` の倍の精度の符号無し整数型
 * @tparam mod `mod*4` が `_uint` に収まるような奇数
 */
template<gnu_signed_integral _int, gnu_unsigned_integral _uint, gnu_signed_integral _long, gnu_unsigned_integral _ulong, _uint mod>
requires (sizeof(_int) == sizeof(_uint)) && (sizeof(_long) == sizeof(_ulong)) && (sizeof(_int)*2 == sizeof(_long)) && (mod < _uint(1) << (sizeof(_uint)*8 - 2)) && (mod % 2 == 1)
struct static_montgomery_mint {
	using mint = static_montgomery_mint<_int, _uint, _long, _ulong, mod>;
	constexpr static _uint get_mod() {
		return mod;
	}
	constexpr static mint primitive_root();
	static long long discrete_log(mint x, mint y);
	constexpr static_montgomery_mint() : _xr(0) {}
	template<gnu_signed_integral T> constexpr static_montgomery_mint(T x) : _xr(_reduce(_ulong(x%_long(mod)+mod)*_r2)) {}
	template<gnu_unsigned_integral T> constexpr static_montgomery_mint(T x) : _xr(_reduce(_ulong(x%_ulong(mod)+mod)*_r2)) {}
	constexpr _uint val() const {
		_uint x = _reduce(_xr);
		return x >= mod ? x - mod : x;
	}
	constexpr mint operator+() const { return mint(*this); }
	constexpr mint operator-() const { return mint(0) - mint(*this); }
	constexpr mint &operator+=(const mint &a) {
		if (_int(_xr += a._xr - mod * 2) < 0) _xr += mod * 2;
		return *this;
	}
	constexpr mint &operator-=(const mint &a) {
		if (_int(_xr -= a._xr) < 0) _xr += mod * 2;
		return *this;
	}
	constexpr mint &operator*=(const mint &a) {
		_xr = _reduce(_ulong(_xr) * a._xr);
		return *this;
	}
	constexpr mint &operator++() {
		*this += 1;
		return *this;
	}
	constexpr mint operator++(int) {
		mint temp = *this;
		++*this;
		return temp;
	}
	constexpr mint &operator--() {
		*this -= 1;
		return *this;
	}
	constexpr mint operator--(int) {
		mint temp = *this;
		--*this;
		return temp;
	}
	constexpr mint operator+(const mint &a) const { return mint(*this) += a; }
	constexpr mint operator-(const mint &a) const { return mint(*this) -= a; }
	constexpr mint operator*(const mint &a) const { return mint(*this) *= a; }
	friend mint operator+(const _long a, const mint b) { return mint(a) + b; }
	friend mint operator-(const _long a, const mint b) { return mint(a) - b; }
	friend mint operator*(const _long a, const mint b) { return mint(a) * b; }

	/**
	 * @brief 繰り返し 2 乗法によって冪乗を計算する。負の冪乗では逆元の冪乗となり、 `mod` が素数である必要がある。
	 *
	 * @param t 指数
	 * @return 冪乗
	 */
	constexpr mint pow(_long t) const {
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
	 * @brief 拡張ユークリッドの互除法によって逆元を求める。 `mod` が素数である必要がある。
	 *
	 * @return 逆元
	 */
	constexpr mint inv() const {
		assert(_is_prime_mod);
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
	constexpr mint &operator/=(const mint &a) {
		*this *= a.inv();
		return *this;
	}
	constexpr mint operator/(const mint &a) const { return mint(*this) /= a; }
	friend mint operator/(const _long a, const mint b) { return mint(a) / b; }
	constexpr bool operator==(const mint &a) const {
		return (_xr >= mod ? _xr - mod : _xr) == (a._xr >= mod ? a._xr - mod : a._xr);
	}
	constexpr bool operator!=(const mint &a) const {
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
	static constexpr _uint _r2 = -_ulong(mod) % mod;
	static constexpr _uint _sz = sizeof(_uint) * 8;
	static constexpr bool _is_prime_mod = is_prime(mod);
	static constexpr _uint _ninv = []() {
		_uint res = mod;
		while (mod * res != 1) res *= _uint(2) - mod * res;
		return res;
	}();
	_uint _xr;
	constexpr _uint _reduce(const _ulong &a) const {
		return (a + _ulong(_uint(a) * _uint(-_ninv)) * mod) >> _sz;
	}
};

/**
 * @brief Montgomery 乗算によって自動で剰余を取る 32 bit 整数。
 *
 * @tparam mod `mod*4` が `uint32_t` に収まるような奇数
 */
template<uint32_t mod> using static_mint = static_montgomery_mint<int32_t, uint32_t, int64_t, uint64_t, mod>;

/**
 * @brief Montgomery 乗算によって自動で剰余を取る 64 bit 整数。
 *
 * @tparam mod `mod*4` が `uint64_t` に収まるような奇数
 */
template<uint64_t mod> using static_mint64 = static_montgomery_mint<int64_t, uint64_t, __int128_t, __uint128_t, mod>;

// using mint = static_mint<1000000007>;
// using mint64 = static_mint64<1000000007>;
// using mint = static_mint<998244353>;
// using mint64 = static_mint64<998244353>;