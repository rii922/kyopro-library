/**
 * @file primitive_root.cpp
 * @author rii922
 * @brief `mod` の原始根を求められるよう、 `mint` および `mint64` を拡張する。
 * @date 2025-02-21
 */

#include <bits/stdc++.h>
#include "montgomery_mint.cpp"
#include "dynamic_montgomery_mint.cpp"
#include "factorize.cpp"
using namespace std;

/**
 * @brief `mod` の最小の原始根を求める。
 *
 * @tparam _int `_uint` と同じ精度の符号付き整数型
 * @tparam _uint `mod*4` を表現可能な符号無し整数型
 * @tparam _long `_int` の倍の精度の符号付き整数型
 * @tparam _ulong `_uint` の倍の精度の符号無し整数型
 * @tparam mod `mod*4` が `_uint` に収まるような奇数
 * @return `mod` の原始根の 1 つ
 */
template<signed_integral _int, unsigned_integral _uint, signed_integral _long, unsigned_integral _ulong, _uint mod>
requires (sizeof(_int) == sizeof(_uint)) && (sizeof(_long) == sizeof(_ulong)) && (sizeof(_int)*2 == sizeof(_long)) && (mod < _uint(1) << (sizeof(_uint)*8 - 2)) && (mod % 2 == 1)
constexpr static_montgomery_mint<_int, _uint, _long, _ulong, mod> static_montgomery_mint<_int, _uint, _long, _ulong, mod>::primitive_root() {
	using mint = static_montgomery_mint<_int, _uint, _long, _ulong, mod>;
	unsigned long long x = mod - 1;
	vector<_int> primes;
	while (!is_prime(x) && x > 1) {
		_int p = find_prime_factor(x);
		primes.push_back(p);
		while (x % p == 0) x /= p;
	}
	if (x > 1) primes.push_back(x);
	mint g = 1;
	while (true) {
		bool ok = true;
		for (auto &y : primes) {
			if (g.pow((mod-1)/y) == 1) {
				ok = false;
				break;
			}
		}
		if (ok) return g;
		g++;
	}
}

/**
 * @brief `mod` の原始根を 1 つ求める。乱択のため、返り値は呼び出しごとに変化しうる。
 *
 * @tparam _int `_uint` と同じ精度の符号付き整数型
 * @tparam _uint `mod*4` を表現可能な符号無し整数型
 * @tparam _long `_int` の倍の精度の符号付き整数型
 * @tparam _ulong `_uint` の倍の精度の符号無し整数型
 * @tparam id 型を区別するための ID 。同じ ID を持つインスタンスは同じ `mod` を持つ。
 * @return `mod` の原始根の 1 つ
 */
template<class _int, class _uint, class _long, class _ulong, int id> dynamic_montgomery_mint<_int, _uint, _long, _ulong, id> dynamic_montgomery_mint<_int, _uint, _long, _ulong, id>::primitive_root() {
	using mint = dynamic_montgomery_mint<_int, _uint, _long, _ulong, id>;
	mt19937 mt(time(nullptr));
	vector<_int> primes;
	for (auto &[x, _] : factorize(mod-1)) primes.push_back(x);
	while (true) {
		mint g = mt();
		if (g == 0) continue;
		bool ok = true;
		for (auto &x : primes) {
			if (g.pow((mod-1)/x) == 1) {
				ok = false;
				break;
			}
		}
		if (ok) return g;
	}
}