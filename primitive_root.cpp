/**
 * @file primitive_root.cpp
 * @author rii922
 * @brief `mod` の原始根を求められるよう、 `mint` および `mint64` を拡張する。
 * @date 2025-02-19
 *
 * Verified with:
 * https://judge.yosupo.jp/submission/268596
 */

#include <bits/stdc++.h>
#include "montgomery_mint.cpp"
#include "dynamic_montgomery_mint.cpp"
#include "factorize.cpp"
using namespace std;

/**
 * @brief `mod` の原始根を 1 つ求める。乱択のため、返り値は呼び出しごとに変化しうる。
 *
 * @tparam _int `_uint` と同じ精度の符号付き整数型
 * @tparam _uint `mod*4` を表現可能な符号無し整数型
 * @tparam _long `_int` の倍の精度の符号付き整数型
 * @tparam _ulong `_uint` の倍の精度の符号無し整数型
 * @tparam mod `mod*4` が `_uint` に収まるような奇数
 * @return `mod` の原始根の 1 つ
 */
// template<class _int, class _uint, class _long, class _ulong, _uint mod> static_montgomery_mint<_int, _uint, _long, _ulong, mod> static_montgomery_mint<_int, _uint, _long, _ulong, mod>::primitive_root() {
// 	using mint = static_montgomery_mint<_int, _uint, _long, _ulong, mod>;
// 	mt19937 mt(time(nullptr));
//	vector<_int> primes;
//	for (auto &[x, _] : factorize(mod-1)) primes.push_back(x);
// 	while (true) {
// 		mint g = mt();
// 		if (g == 0) continue;
// 		bool ok = true;
// 		for (auto &x : primes) {
// 			if (g.pow((mod-1)/x) == 1) {
// 				ok = false;
// 				break;
// 			}
// 		}
// 		if (ok) return g;
// 	}
// }

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