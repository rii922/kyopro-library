#include <bits/stdc++.h>
using namespace std;

template<int id>
struct dynamic_mint {
	using mint = dynamic_mint<id>;
	static int mod;
	static void set_mod(int m) {
		mod = m;
	}
	long long x;
	dynamic_mint(long long x = 0) : x((x%mod+mod)%mod) {}
	mint operator+() const { return mint(x); }
	mint operator-() const { return mint(-x); }
	mint &operator+=(const mint a) {
		if ((x += a.x) >= mod) x -= mod;
		return *this;
	}
	mint &operator-=(const mint a) {
		if ((x += mod - a.x) >= mod) x -= mod;
		return *this;
	}
	mint &operator*=(const mint a) {
		(x *= a.x) %= mod;
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
	mint operator+(const mint a) const { return mint(*this) += a; }
	mint operator-(const mint a) const { return mint(*this) -= a; }
	mint operator*(const mint a) const { return mint(*this) *= a; }
	friend mint operator+(const long long a, const mint b) { return mint(a) + b; }
	friend mint operator-(const long long a, const mint b) { return mint(a) - b; }
	friend mint operator*(const long long a, const mint b) { return mint(a) * b; }
	mint pow(long long t) const {
		if (t < 0) return pow(-t).inv();
		if (t == 0) return 1;
		mint a = pow(t >> 1);
		a *= a;
		if (t & 1) a *= *this;
		return a;
	}
	mint inv() const { return pow(mod - 2); }
	mint &operator/=(const mint a) { return *this *= a.inv(); }
	mint operator/(const mint a) const { return mint(*this) /= a; }
	friend mint operator/(const long long a, const mint b) { return mint(a) / b; }
	bool operator==(const mint a) const { return x == a.x; }
	bool operator!=(const mint a) const { return x != a.x; }
	friend istream &operator>>(istream &is, mint &a) {
		long long x;
		is >> x;
		a = mint(x);
		return is;
	}
	friend ostream &operator<<(ostream &os, const mint &a) {
		os << a.x;
		return os;
	}
};
template<int id> int dynamic_mint<id>::mod = 998244353;
using mint = dynamic_mint<-1>;