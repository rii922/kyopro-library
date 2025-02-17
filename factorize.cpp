/**
 * @file factorize.cpp
 * @author rii922
 * @brief Pollard's rho 素因数分解法によって素因数分解を行う。
 * @date 2025-02-16
 *
 * Verified with:
 * https://judge.yosupo.jp/submission/268062
 */

#include <bits/stdc++.h>
#include "dynamic_montgomery_mint.cpp"
#include "is_prime.cpp"
using namespace std;

/**
 * @brief Floyd の循環検出法および Richard Brent の変形によって @c n の素因数の 1 つを求める。
 *
 * O(n^(1/4))
 * @param n 合成数
 * @return long long @c n の素因数の 1 つ
 */
long long find_prime_factor(long long n) {
	using mint64 = dynamic_mint64<static_cast<int>(0x3bdc9608)>;
	if (n % 2 == 0) return 2;
	mint64::set_mod(n);
	long long m = round(pow(n, 0.125)) + 1;
	for (long long c = 1; c < n; c++) {
		mint64 y = 0;
		mint64 x, yc;
		mint64 q = 1;
		long long r = 1;
		long long k = 0;
		long long g = 1;
		while (g == 1) {
			x = y;
			while (k < r*3/4) {
				y = y*y+c;
				k++;
			}
			while (k < r && g == 1) {
				yc = y;
				long long l = min(m, r-k);
				for (long long i = 0; i < l; i++) {
					y = y*y+c;
					q *= x-y;
				}
				g = gcd(q.val(), n);
				k += m;
			}
			k = r;
			r *= 2;
		}
		if (g == n) {
			g = 1;
			y = yc;
			while (g == 1) {
				y = y*y+c;
				g = gcd((x-y).val(), n);
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
 * @tparam T @c n の型
 * @param n 素因数分解を行う数
 * @return map<T, int> 素因数をキー、指数を値に持つ @c map
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