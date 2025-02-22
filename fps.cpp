#include <bits/stdc++.h>
#include "montgomery_mint.cpp"
#include "convolution.cpp"
using namespace std;

/// EXPAND FROM HERE

template<class convolution, int mod>
struct fps_base {
	using fps = fps_base<convolution, mod>;
	using mint = static_mint<mod>;
	fps_base(int n) : _v(n, 0) {}
	fps_base(int n, mint &x) : _v(n, x) {}
	fps_base(int n, mint &&x) : _v(n, x) {}
	fps_base(const vector<mint> &v) : _v(v) {}
	fps_base(initializer_list<mint> init) : _v(init) {}
	int size() const {
		return _v.size();
	}
	bool empty() const {
		return _v.empty();
	}
	const mint &operator[](int p) const { return _v[p]; }
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
		if (sz < 0) return *this >>= sz;
		if (_v.size() <= sz) return *this = {};
		_v.erase(_v.begin(), _v.begin()+sz);
		return *this;
	}
	fps &operator>>=(int sz) {
		if (sz < 0) return *this <<= sz;
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
	fps pre(int sz) const {
		fps res(sz, 0);
		int m = min((int)_v.size(), sz);
		for (int i = 0; i < m; i++) res[i] = _v[i];
		return res;
	}
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
	fps diff() const {
		fps res(_v.size()-1);
		for (int i = 1; i < _v.size(); i++) res[i-1] = _v[i] * i;
		return res;
	}
	fps integral() const {
		fps res(_v.size()+1);
		res[0] = 0;
		for (int i = 0; i < _v.size(); i++) res[i+1] = _v[i] / (i + 1);
		return res;
	}
	fps log(int deg=-1) const {
		assert(!_v.empty() && _v[0] == 1);
		if (deg == -1) deg = _v.size();
		return (diff() / pre(deg)).pre(deg-1).integral();
	}
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
private:
	static const convolution _conv;
	vector<mint> _v;
};
template<class convolution, int mod> const convolution fps_base<convolution, mod>::_conv = convolution();
template<int mod> using ntt_friendly_fps = fps_base<ntt_friendly_convolution<mod>, mod>;
template<int mod> using arbitrary_fps = fps_base<arbitrary_convolution<mod>, mod>;
using fps = ntt_friendly_fps<998244353>;
using fps = arbitrary_fps<1000000007>;