/**
 * @file or_convolution.hpp
 * @author rii922
 * @brief Zeta 変換、 Moebius 変換にょって OR convolution を行う。
 * @date 2025-03-03
 */

#include <bits/stdc++.h>
using namespace std;

/// EXPAND FROM HERE

template<class T>
void _or_zeta(vector<T> &v) {
	int e = __builtin_ctz(v.size());
	for (int i = 0; i < e; i++) {
		for (int j = 0; j < v.size(); j++) {
			if (!((j >> i) & 1)) v[j|(1<<i)] += v[j];
		}
	}
}

template<class T>
void _or_moebius(vector<T> &v) {
	int e = __builtin_ctz(v.size());
	for (int i = 0; i < e; i++) {
		for (int j = 0; j < v.size(); j++) {
			if (!((j >> i) & 1)) v[j|(1<<i)] -= v[j];
		}
	}
}

/**
 * @brief Zeta 変換、 Moebius 変換にょって OR convolution を行う。
 *
 * O(N log N)
 * @tparam T 環
 * @param a, b `vector`
 * @return `c[k]=\sum_{i|j=k} a[i]*b[j]` なる `c` 。 `c.size()` は `a.size()` と `b.size()` 以上の最小の 2 冪である。
 */
template<class T>
vector<T> or_convolution(const vector<T> &a, const vector<T> &b) {
	int n = max(a.size(), b.size());
	int n2 = 1;
	while (n2 < n) n2 *= 2;
	vector<T> a2(n2, 0), b2(n2, 0);
	for (int i = 0; i < a.size(); i++) a2[i] = a[i];
	for (int i = 0; i < b.size(); i++) b2[i] = b[i];
	_or_zeta(a2);
	_or_zeta(b2);
	for (int i = 0; i < n2; i++) a2[i] *= b2[i];
	_or_moebius(a2);
	return a2;
}