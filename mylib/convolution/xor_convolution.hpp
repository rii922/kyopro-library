/**
 * @file xor_convolution.hpp
 * @author rii922
 * @brief Hadamard 変換にょって XOR convolution を行う。
 * @date 2025-03-04
 *
 * Verified with:
 * https://judge.yosupo.jp/submission/270895
 */

#include <bits/stdc++.h>
using namespace std;

/// EXPAND FROM HERE

template<class T>
void _hadamard(vector<T> &v) {
	int e = __builtin_ctz(v.size());
	int d = 1;
	int n2 = 1 << e;
	int b = 1;
	while (d <= e) {
		for (int i = 0; i < n2; i += (b << 1)) {
			for (int j = 0; j < b; j++) {
				int p0 = i + j;
				int p1 = p0 + b;
				T tp = v[p0] + v[p1];
				T tm = v[p0] - v[p1];
				v[p0] = tp;
				v[p1] = tm;
			}
		}
		d++;
		b <<= 1;
	}
}

/**
 * @brief Hadamard 変換にょって XOR convolution を行う。
 *
 * O(N log N)
 * @tparam T 環
 * @param a, b `vector`
 * @return `c[k]=\sum_{i XOR j=k} a[i]*b[j]` なる `c` 。 `c.size()` は `a.size()` と `b.size()` 以上の最小の 2 冪である。
 */
template<class T>
vector<T> xor_convolution(const vector<T> &a, const vector<T> &b) {
	int n = max(a.size(), b.size());
	int n2 = 1;
	while (n2 < n) n2 *= 2;
	vector<T> a2(n2, 0), b2(n2, 0);
	for (int i = 0; i < a.size(); i++) a2[i] = a[i];
	for (int i = 0; i < b.size(); i++) b2[i] = b[i];
	_hadamard(a2);
	_hadamard(b2);
	for (int i = 0; i < n2; i++) a2[i] *= b2[i];
	_hadamard(a2);
	for (int i = 0; i < n2; i++) a2[i] /= n2;
	return a2;
}