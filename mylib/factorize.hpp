/**
 * @file factorize.hpp
 * @author rii922
 * @brief Pollard's rho 素因数分解法によって素因数分解を行う。
 * @date 2025-02-24
 *
 * Verified with:
 * https://judge.yosupo.jp/submission/269065
 */

#include <bits/stdc++.h>
#include "custom_concepts.hpp"
#include "is_prime.hpp"
using namespace std;

/// EXPAND FROM HERE

template<gnu_signed_integral T>
constexpr T _abs(T x) {
	return x > 0 ? x : -x;
}

template<gnu_integral T>
constexpr T _gcd(T a, T b) {
	return b == 0 ? a : _gcd(b, a%b);
}

constexpr int _eighth_root(unsigned long long x) {
	unsigned long long left = 0;
	unsigned long long right = 1 << 8;
	while (right - left > 1) {
		unsigned long long mid = (left+right)/2;
		if (mid*mid*mid*mid*mid*mid*mid*mid <= x) left = mid;
		else right = mid;
	}
	return left;
}

/**
 * @brief Floyd の循環検出法および Richard Brent の変形によって `n` の素因数の 1 つを求める。
 *
 * O(n^(1/4))
 * @param n 合成数
 * @return `n` の素因数の 1 つ
 */
constexpr unsigned long long find_prime_factor(unsigned long long n) {
	if (n % 2 == 0) return 2;
	int m = _eighth_root(n) + 1;
	for (int c = 1; c < n; c++) {
		__int128_t y = 0;
		__int128_t x, yc;
		__uint128_t q = 1;
		int r = 1;
		int k = 0;
		unsigned long long g = 1;
		while (g == 1) {
			x = y;
			while (k < r*3/4) {
				y = (y*y+c)%n;
				k++;
			}
			while (k < r && g == 1) {
				yc = y;
				int l = min(m, r-k);
				for (int i = 0; i < l; i++) {
					y = (y*y+c)%n;
					q = q*_abs(x-y)%n;
				}
				g = _gcd<__uint128_t>(q, n);
				k += m;
			}
			k = r;
			r *= 2;
		}
		if (g == n) {
			g = 1;
			y = yc;
			while (g == 1) {
				y = (y*y+c)%n;
				g = _gcd<__uint128_t>(_abs(x-y), n);
			}
		}
		if (g == n) continue;
		if (is_prime(g)) return g;
		else if (is_prime(n/g)) return n/g;
		else return find_prime_factor(g);
	}
	return -1;
}

/**
 * @brief Pollard's rho 素因数分解法によって素因数分解を行う。
 *
 * O(n^(1/4) polylog n)
 * @tparam T `n` の型
 * @param n 素因数分解を行う数
 * @return 素因数をキー、指数を値に持つ `map`
 */
template<class T>
map<T, int> factorize(T n) {
	map<T, int> res;
	while (!is_prime(n) && n > 1) {
		T p = find_prime_factor(n);
		while (n % p == 0) {
			n /= p;
			res[p]++;
		}
	}
	if (n > 1) res[n] = 1;
	return res;
}