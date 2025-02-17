/**
 * @file is_prime.cpp
 * @author rii922
 * @brief Miller-Rabin 素数判定法による素数判定を行う。
 * @date 2025-02-17
 *
 * Verified with:
 * https://judge.yosupo.jp/submission/268061
 */

#include <bits/stdc++.h>
#include "dynamic_montgomery_mint.cpp"
using namespace std;

/**
 * @brief Miller-Rabin 素数判定法による素数判定を行う。
 *
 * O(log n)
 * @param n 素数判定する数
 * @return `n` が素数かどうか
 */
bool is_prime(long long n) {
	using mint64 = dynamic_mint64<static_cast<int>(0x90d3801e)>;
	if (n == 2) return true;
	if (n < 2 || n % 2 == 0) return false;
	mint64::set_mod(n);
	long long d = n - 1;
	long long s = 0;
	while (d % 2 == 0) {
		d /= 2;
		s++;
	}
	const vector<long long> bases = (n < 1LL << 32) ? vector<long long>{2, 7, 61} : vector<long long>{2, 325, 9375, 28178, 450775, 9780504, 1795265022};
	for (auto &b : bases) {
		mint64 a = b;
		if (a == 0) continue;
		mint64 t = a.pow(d);
		if (t == 1) continue;
		bool e = true;
		for (int i = 0; i < s; i++) {
			if (t == -1) {
				e = false;
				break;
			}
			t *= t;
		}
		if (e) return false;
	}
	return true;
}