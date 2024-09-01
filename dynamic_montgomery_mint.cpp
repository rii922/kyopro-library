#include <bits/stdc++.h>
using namespace std;

template<class _int, class _uint, class _ulong, int id>
struct dynamic_montgomery_mint {
	using mint = dynamic_montgomery_mint<_int, _uint, _ulong, id>;
	static _uint mod;
	static void set_mod(_uint m) {
		assert(m < _uint(1) << (_sz - 2));
		assert(m & 1);
		mod = m;
		_r2 = -_ulong(mod) % mod;
		_ninv = mod;
		while (mod * _ninv != 1) _ninv *= _uint(2) - mod * _ninv;
	}
	dynamic_montgomery_mint() : _xr(0) {}
	dynamic_montgomery_mint(_int x) : _xr(_reduce(_ulong(x%_int(mod)+mod)*_r2)) {}
	_uint val() const {
		_uint x = _reduce(_xr);
		return x >= mod ? x - mod : x;
	}
	mint operator+() const { return mint(*this); }
	mint operator-() const { return mint(0) - mint(*this); }
	mint &operator+=(const mint &a) {
		if (((_xr += a._xr - mod * 2) >> (_sz - 1)) & 1) _xr += mod * 2;
		return *this;
	}
	mint &operator-=(const mint &a) {
		if (((_xr -= a._xr) >> (_sz - 1)) & 1) _xr += mod * 2;
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
	friend mint operator+(const _int a, const mint b) { return mint(a) + b; }
	friend mint operator-(const _int a, const mint b) { return mint(a) - b; }
	friend mint operator*(const _int a, const mint b) { return mint(a) * b; }
	mint pow(_int t) const {
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
	friend mint operator/(const _int a, const mint b) { return mint(a) / b; }
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
template<class _int, class _uint, class _ulong, int id> _uint dynamic_montgomery_mint<_int, _uint, _ulong, id>::mod = 0;
template<class _int, class _uint, class _ulong, int id> _uint dynamic_montgomery_mint<_int, _uint, _ulong, id>::_r2 = 0;
template<class _int, class _uint, class _ulong, int id> _uint dynamic_montgomery_mint<_int, _uint, _ulong, id>::_ninv = 0;
template<class _int, class _uint, class _ulong, int id> _uint dynamic_montgomery_mint<_int, _uint, _ulong, id>::_sz = sizeof(_uint) * 8;
template<int id> using dynamic_mint = dynamic_montgomery_mint<int32_t, uint32_t, uint64_t, id>;
template<int id> using dynamic_mint64 = dynamic_montgomery_mint<int64_t, uint64_t, __uint128_t, id>;
using mint = dynamic_mint<-1>;
using mint64 = dynamic_mint64<-1>;