/**
 * @file gcd_convolution.hpp
 * @author rii922
 * @brief Zeta 変換、 Moebius 変換にょって GCD convolution を行う。
 * @date 2025-03-04
 *
 * Verified with:
 * https://judge.yosupo.jp/submission/271025
 */

#include <bits/stdc++.h>
using namespace std;

/// EXPAND FROM HERE

template<class T>
void _gcd_zeta(vector<T> &v) {
	int n = v.size() - 1;
	vector<bool> sieve(n+1, true);
	for (long long i = 2; i <= n; i++) {
		if (sieve[i]) {
			for (long long j = n/i; j >= 1; j--) v[j] += v[i*j];
			for (long long j = i*i; j <= n; j += i) sieve[j] = false;
		}
	}
}

template<class T>
void _gcd_moebius(vector<T> &v) {
	int n = v.size() - 1;
	vector<bool> sieve(n+1, true);
	for (long long i = 2; i <= n; i++) {
		if (sieve[i]) {
			for (long long j = 1; j <= n/i; j++) v[j] -= v[i*j];
			for (long long j = i*i; j <= n; j += i) sieve[j] = false;
		}
	}
}

/**
 * @brief Zeta 変換、 Moebius 変換にょって GCD convolution を行う。 `a[0]` および `b[0]` は計算結果に影響しない。
 *
 * O(N log log N)
 * @tparam T 環
 * @param a, b `vector`
 * @return `c[k]=\sum_{gcd(i, j)=k} a[i]*b[j]` なる `c` 。 `c.size()` は `max(a.size(), b.size())` である。 `c[0]` は未定義。
 */
template<class T>
vector<T> gcd_convolution(const vector<T> &a, const vector<T> &b) {
	int n = max(a.size(), b.size()) - 1;
	vector<T> an(n+1, 0), bn(n+1, 0);
	for (int i = 1; i < a.size(); i++) an[i] = a[i];
	for (int i = 1; i < b.size(); i++) bn[i] = b[i];
	_gcd_zeta(an);
	_gcd_zeta(bn);
	for (int i = 1; i <= n; i++) an[i] *= bn[i];
	_gcd_moebius(an);
	return an;
}