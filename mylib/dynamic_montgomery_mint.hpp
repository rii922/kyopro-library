/**
 * @file dynamic_montgomery_mint.cpp
 * @author rii922
 * @brief Montgomery 乗算によって自動で剰余を取る整数型。 `mod` が実行時に決まる場合に使用する。
 * @date 2025-02-24
 */

#include <bits/stdc++.h>
#include "custom_concepts.hpp"
using namespace std;

/// EXPAND FROM HERE

/**
 * @brief Montgomery 乗算によって自動で剰余を取る汎用クラス。 `mod` が実行時に決まる場合に使用する。
 *
 * @tparam _int `_uint` と同じ精度の符号付き整数型
 * @tparam _uint `mod*4` を表現可能な符号無し整数型
 * @tparam _long `_int` の倍の精度の符号付き整数型
 * @tparam _ulong `_uint` の倍の精度の符号無し整数型
 * @tparam id 型を区別するための ID 。同じ ID を持つインスタンスは同じ `mod` を持つ。
 */
template<gnu_signed_integral _int, gnu_unsigned_integral _uint, gnu_signed_integral _long, gnu_unsigned_integral _ulong, int id>
requires (sizeof(_int) == sizeof(_uint)) && (sizeof(_long) == sizeof(_ulong)) && (sizeof(_int)*2 == sizeof(_long))
struct dynamic_montgomery_mint {
	using mint = dynamic_montgomery_mint<_int, _uint, _long, _ulong, id>;
	static _uint mod;
	static _uint get_mod() {
		return mod;
	}

	/**
	 * @brief `mod` を設定する。インスタンス生成前に呼び出す必要がある。
	 *
	 * @param m `mod*4` が `_uint` に収まるような奇数
	 */
	static void set_mod(_uint m) {
		assert(m < _uint(1) << (_sz - 2));
		assert(m & 1);
		mod = m;
		_r2 = -_ulong(mod) % mod;
		_ninv = mod;
		while (mod * _ninv != 1) _ninv *= _uint(2) - mod * _ninv;
	}
	static mint primitive_root();
	dynamic_montgomery_mint() : _xr(0) {}
	template<gnu_signed_integral T> dynamic_montgomery_mint(T x) : _xr(_reduce(_ulong(x%_long(mod)+mod)*_r2)) {}
	template<gnu_unsigned_integral T> dynamic_montgomery_mint(T x) : _xr(_reduce(_ulong(x%_ulong(mod)+mod)*_r2)) {}
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
	 * @brief 繰り返し 2 乗法によって冪乗を計算する。負の冪乗では逆元の冪乗となり、 `mod` が素数である必要がある。
	 *
	 * @param t 指数
	 * @return 冪乗
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
	 * @brief 拡張ユークリッドの互除法によって逆元を求める。 `mod` が素数である必要がある。
	 *
	 * @return 逆元
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
	static _uint _r2, _ninv;
	constexpr static _uint _sz = sizeof(_uint) * 8;
	_uint _xr;
	_uint _reduce(const _ulong &a) const {
		return (a + _ulong(_uint(a) * _uint(-_ninv)) * mod) >> _sz;
	}
};
template<gnu_signed_integral _int, gnu_unsigned_integral _uint, gnu_signed_integral _long, gnu_unsigned_integral _ulong, int id>
requires (sizeof(_int) == sizeof(_uint)) && (sizeof(_long) == sizeof(_ulong)) && (sizeof(_int)*2 == sizeof(_long))
_uint dynamic_montgomery_mint<_int, _uint, _long, _ulong, id>::mod = 0;
template<gnu_signed_integral _int, gnu_unsigned_integral _uint, gnu_signed_integral _long, gnu_unsigned_integral _ulong, int id>
requires (sizeof(_int) == sizeof(_uint)) && (sizeof(_long) == sizeof(_ulong)) && (sizeof(_int)*2 == sizeof(_long))
_uint dynamic_montgomery_mint<_int, _uint, _long, _ulong, id>::_r2 = 0;
template<gnu_signed_integral _int, gnu_unsigned_integral _uint, gnu_signed_integral _long, gnu_unsigned_integral _ulong, int id>
requires (sizeof(_int) == sizeof(_uint)) && (sizeof(_long) == sizeof(_ulong)) && (sizeof(_int)*2 == sizeof(_long))
_uint dynamic_montgomery_mint<_int, _uint, _long, _ulong, id>::_ninv = 0;

/**
 * @brief Montgomery 乗算によって自動で剰余を取る 32 bit 整数。
 *
 * @tparam id 型を区別するための ID 。同じ ID を持つインスタンスは同じ `mod` を持つ。ライブラリ内で使用する場合は、ライブラリ名の SHA256 ハッシュの先頭 8 文字を使い、 `dynamic_mint<static_cast<int>(0xdeadbeef)>` のようにする。
 */
template<int id> using dynamic_mint = dynamic_montgomery_mint<int32_t, uint32_t, int64_t, uint64_t, id>;

/**
 * @brief Montgomery 乗算によって自動で剰余を取る 64 bit 整数。
 *
 * @tparam id 型を区別するための ID 。同じ ID を持つインスタンスは同じ `mod` を持つ。ライブラリ内で使用する場合は、ライブラリ名の SHA256 ハッシュの先頭 8 文字を使い、 `dynamic_mint64<static_cast<int>(0xdeadbeef)>` のようにする。
 */
template<int id> using dynamic_mint64 = dynamic_montgomery_mint<int64_t, uint64_t, __int128_t, __uint128_t, id>;

// using mint = dynamic_mint<-1>;
// using mint64 = dynamic_mint64<-1>;