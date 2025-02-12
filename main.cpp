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
template<class T> using min_priority_queue = priority_queue<T, vector<T>, greater<T>>;
template<class T> bool chmax(T &a, const T &b) { if (a<b) { a=b; return 1; } return 0; }
template<class T> bool chmin(T &a, const T &b) { if (b<a) { a=b; return 1; } return 0; }
template<class T> bool chmaxeq(T &a, const T &b) { if (a<=b) { a=b; return 1; } return 0; }
template<class T> bool chmineq(T &a, const T &b) { if (b<=a) { a=b; return 1; } return 0; }
bool yes(bool a=true) { cout << (a?"yes":"no") << endl; return a; }
bool no(bool a=true) { cout << (a?"no":"yes") << endl; return a; }
bool Yes(bool a=true) { cout << (a?"Yes":"No") << endl; return a; }
bool No(bool a=true) { cout << (a?"No":"Yes") << endl; return a; }
bool YES(bool a=true) { cout << (a?"YES":"NO") << endl; return a; }
bool NO(bool a=true) { cout << (a?"NO":"YES") << endl; return a; }
template<class T1, class T2> istream &operator>>(istream &is, pair<T1, T2> &p);
template<class T1, class T2> ostream &operator<<(ostream &os, const pair<T1, T2> &p);
template<class T> istream &operator>>(istream &is, vector<T> &v);
template<class T> ostream &operator<<(ostream &os, const vector<T> &v);
template<class T1, class T2>
istream &operator>>(istream &is, pair<T1, T2> &p) {
	return is >> p.first >> p.second;
}
template<class T1, class T2>
ostream &operator<<(ostream &os, const pair<T1, T2> &p) {
	return os << p.first << ' ' << p.second;
}
template<class T>
istream &operator>>(istream &is, vector<T> &v) {
	int sz = v.size();
	for (int i = 0; i < sz; i++) is >> v[i];
	return is;
}
template<class T>
ostream &operator<<(ostream &os, const vector<T> &v) {
	int sz = v.size();
	for (int i = 0; i < sz; i++) {
		os << v[i];
		if (i != sz-1) os << ' ';
	}
	return os;
}
void _main();
int main() { cin.tie(0); ios::sync_with_stdio(0); cout << fixed << setprecision(16); _main(); return 0; }

void _main() {
	
}