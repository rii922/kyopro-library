#include <bits/stdc++.h>
using namespace std;

template<int id>
struct dynamic_mint64 {
	using mint64 = dynamic_mint64<id>;
	using i64 = int64_t;
	using ui64 = uint64_t;
	using ui128 = __uint128_t;
	static ui64 mod;
	static void set_mod(ui64 m) {
		assert(m < ui64(1) << 62);
		assert(m & 1);
		mod = m;
		_r2 = -ui128(mod) % mod;
		_ninv = mod;
		while (mod * _ninv != 1) _ninv *= ui64(2) - mod * _ninv;
	}
	dynamic_mint64() : _xr(0) {}
	dynamic_mint64(i64 x) : _xr(_reduce(ui128(x%i64(mod)+mod)*_r2)) {}
	ui64 val() const {
		ui64 x = _reduce(_xr);
		return x >= mod ? x - mod : x;
	}
	mint64 operator+() const { return mint64(*this); }
	mint64 operator-() const { return mint64(0) - mint64(*this); }
	mint64 &operator+=(const mint64 &a) {
		if (((_xr += a._xr - mod * 2) >> 63) & 1) _xr += mod * 2;
		return *this;
	}
	mint64 &operator-=(const mint64 &a) {
		if (((_xr -= a._xr) >> 63) & 1) _xr += mod * 2;
		return *this;
	}
	mint64 &operator*=(const mint64 &a) {
		_xr = _reduce(ui128(_xr) * a._xr);
		return *this;
	}
	mint64 &operator++() {
		*this += 1;
		return *this;
	}
	mint64 operator++(int) {
		mint64 temp = *this;
		++*this;
		return temp;
	}
	mint64 &operator--() {
		*this -= 1;
		return *this;
	}
	mint64 operator--(int) {
		mint64 temp = *this;
		--*this;
		return temp;
	}
	mint64 operator+(const mint64 &a) const { return mint64(*this) += a; }
	mint64 operator-(const mint64 &a) const { return mint64(*this) -= a; }
	mint64 operator*(const mint64 &a) const { return mint64(*this) *= a; }
	friend mint64 operator+(const i64 a, const mint64 b) { return mint64(a) + b; }
	friend mint64 operator-(const i64 a, const mint64 b) { return mint64(a) - b; }
	friend mint64 operator*(const i64 a, const mint64 b) { return mint64(a) * b; }
	mint64 pow(i64 t) const {
		if (t < 0) return pow(-t).inv();
		mint64 res = 1;
		mint64 mul = *this;
		while (t) {
			if (t & 1) res *= mul;
			mul *= mul;
			t >>= 1;
		}
		return res;
	}
	mint64 inv() const {
		i64 x = val();
		i64 y = mod;
		i64 u = 1;
		i64 v = 0;
		while (y) {
			i64 t = x / y;
			x -= y * t;
			u -= v * t;
			swap(x, y);
			swap(u, v);
		}
		return mint64(u);
	}
	mint64 &operator/=(const mint64 &a) {
		*this *= a.inv();
		return *this;
	}
	mint64 operator/(const mint64 &a) const { return mint64(*this) /= a; }
	friend mint64 operator/(const i64 a, const mint64 b) { return mint64(a) / b; }
	bool operator==(const mint64 &a) const {
		return (_xr >= mod ? _xr - mod : _xr) == (a._xr >= mod ? a._xr - mod : a._xr);
	}
	bool operator!=(const mint64 &a) const {
		return (_xr >= mod ? _xr - mod : _xr) != (a._xr >= mod ? a._xr - mod : a._xr);
	}
	friend istream &operator>>(istream &is, mint64 &a) {
		i64 x;
		is >> x;
		a = mint64(x);
		return is;
	}
	friend ostream &operator<<(ostream &os, const mint64 &a) {
		i64 x = a.val();
		os << x;
		return os;
	}
private:
	static ui64 _r2, _ninv;
	ui64 _xr;
	ui64 _reduce(const ui128 &a) const {
		return (a + ui128(ui64(a) * ui64(-_ninv)) * mod) >> 64;
	}
};
template<int id> dynamic_mint64<id>::ui64 dynamic_mint64<id>::mod = 998244353ULL;
template<int id> dynamic_mint64<id>::ui64 dynamic_mint64<id>::_r2 = 299560064ULL;
template<int id> dynamic_mint64<id>::ui64 dynamic_mint64<id>::_ninv = 996491785301655553ULL;
using mint64 = dynamic_mint64<-1>;