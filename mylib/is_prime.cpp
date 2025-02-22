/**
 * @file is_prime.cpp
 * @author rii922
 * @brief Miller-Rabin 素数判定法による素数判定を行う。
 * @date 2025-02-21
 *
 * Verified with:
 * https://judge.yosupo.jp/submission/269057
 */

#include <bits/stdc++.h>
#include "pow_mod.cpp"
using namespace std;

/// EXPAND FROM HERE

/**
 * @brief Miller-Rabin 素数判定法による素数判定を行う。
 *
 * O(log n)
 * @param n 素数判定する数
 * @return `n` が素数かどうか
 */
constexpr bool is_prime(unsigned long long n) {
	if (n == 2) return true;
	if (n < 2 || n % 2 == 0) return false;
	long long d = n-1;
	long long s = 0;
	while (d % 2 == 0) {
		d /= 2;
		s++;
	}
	const vector<unsigned long long> bases = (n < 1LL << 32) ? vector<unsigned long long>{2, 7, 61} : vector<unsigned long long>{2, 325, 9375, 28178, 450775, 9780504, 1795265022};
	for (auto &b : bases) {
		unsigned long long a = b%n;
		if (a == 0) continue;
		__uint128_t t = pow_mod<uint64_t, __uint128_t>(a, d, n);
		if (t == 1) continue;
		bool e = true;
		for (int i = 0; i < s; i++) {
			if (t == n-1) {
				e = false;
				break;
			}
			t = t*t%n;
		}
		if (e) return false;
	}
	return true;
}