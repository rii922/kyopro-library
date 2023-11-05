#include <bits/stdc++.h>
#define rep(i, n) for(int i=0, i##_len=(n); i<i##_len; ++i)
#define reps(i, n) for(int i=1, i##_len=(n); i<=i##_len; ++i)
#define rrep(i, n) for(int i=((int)(n)-1); i>=0; --i)
#define rreps(i, n) for(int i=((int)(n)); i>0; --i)
#define all(v) (v).begin(), (v).end()
using namespace std;
using ll = long long;
using ull = unsigned long long;
using vi = vector<int>;
using vvi = vector<vector<int>>;
using vvvi = vector<vector<vector<int>>>;
using vl = vector<ll>;
using vvl = vector<vector<ll>>;
using vvvl = vector<vector<vector<ll>>>;
using vs = vector<string>;
using pi = pair<int, int>;
using pl = pair<ll, ll>;
template<class T>bool chmax(T &a, const T &b) { if (a<b) { a=b; return 1; } return 0; }
template<class T>bool chmin(T &a, const T &b) { if (b<a) { a=b; return 1; } return 0; }
template<class T>bool chmaxeq(T &a, const T &b) { if (a<=b) { a=b; return 1; } return 0; }
template<class T>bool chmineq(T &a, const T &b) { if (b<=a) { a=b; return 1; } return 0; }
bool yes(bool a) { cout << (a?"yes":"no") << endl; return a; }
bool Yes(bool a) { cout << (a?"Yes":"No") << endl; return a; }
bool YES(bool a) { cout << (a?"YES":"NO") << endl; return a; }
void _main();
int main() { cin.tie(0); ios::sync_with_stdio(0); cout << fixed << setprecision(16); _main(); return 0; }

const int mod = 998244353;
struct mint {
	long long x;
	mint(long long x = 0) : x((x%mod+mod)%mod) {}
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
		// x += 1;
		// if (x == mod) x = 0;
		*this += 1;
		return *this;
	}
	mint operator++(int) {
		mint temp = *this;
		++*this;
		return temp;
	}
	mint &operator--() {
		// x -= 1;
		// if (x < 0) x += mod;
		*this -= 1;
		return *this;
	}
	mint operator--(int) {
		mint temp = *this;
		--*this;
		return temp;
	}
	mint operator+(const mint a) { return mint(*this) += a; }
	mint operator-(const mint a) { return mint(*this) -= a; }
	mint operator*(const mint a) { return mint(*this) *= a; }
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
	mint operator/(const mint a) { return mint(*this) /= a; }
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

void _main() {
	
}