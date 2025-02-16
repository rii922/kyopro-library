/**
 * @file primitive_root.cpp
 * @author rii922
 * @brief @c mod の原始根を 1 つ求める。
 * @date 2025-02-16
 *
 * Verified with:
 * https://judge.yosupo.jp/submission/268067
 */

#include <bits/stdc++.h>
#include "montgomery_mint.cpp"
#include "dynamic_montgomery_mint.cpp"
#include "factorize.cpp"
using namespace std;

/**
 * @brief @c mod の原始根を 1 つ求める。乱択のため、返り値は呼び出しごとに変化しうる。
 *
 * @tparam _int @c _uint と同じ精度の符号付き整数型
 * @tparam _uint @c mod*4 を表現可能な符号無し整数型
 * @tparam _long @c _int の倍の精度の符号付き整数型
 * @tparam _ulong @c _uint の倍の精度の符号無し整数型
 * @tparam mod @c mod*4 が @c _uint に収まるような奇数
 * @return static_montgomery_mint<_int, _uint, _long, _ulong, mod> @c mod の原始根の 1 つ
 */
// template<class _int, class _uint, class _long, class _ulong, _uint mod> static_montgomery_mint<_int, _uint, _long, _ulong, mod> static_montgomery_mint<_int, _uint, _long, _ulong, mod>::primitive_root() {
// 	using mint = static_montgomery_mint<_int, _uint, _long, _ulong, mod>;
// 	mt19937 mt(time(nullptr));
// 	while (true) {
// 		mint g = mt();
// 		if (g == 0) continue;
// 		bool ok = true;
// 		for (auto &[x, _] : factorize(mod-1)) {
// 			if (g.pow((mod-1)/x) == 1) {
// 				ok = false;
// 				break;
// 			}
// 		}
// 		if (ok) return g;
// 	}
// }

/**
 * @brief @c mod の原始根を 1 つ求める。乱択のため、返り値は呼び出しごとに変化しうる。
 *
 * @tparam _int @c _uint と同じ精度の符号付き整数型
 * @tparam _uint @c mod*4 を表現可能な符号無し整数型
 * @tparam _long @c _int の倍の精度の符号付き整数型
 * @tparam _ulong @c _uint の倍の精度の符号無し整数型
 * @tparam id 型を区別するための ID 。同じ ID を持つインスタンスは同じ @c mod を持つ。
 * @return dynamic_montgomery_mint<_int, _uint, _long, _ulong, id> @c mod の原始根の 1 つ
 */
template<class _int, class _uint, class _long, class _ulong, int id> dynamic_montgomery_mint<_int, _uint, _long, _ulong, id> dynamic_montgomery_mint<_int, _uint, _long, _ulong, id>::primitive_root() {
	using mint = dynamic_montgomery_mint<_int, _uint, _long, _ulong, id>;
	mt19937 mt(time(nullptr));
	while (true) {
		mint g = mt();
		if (g == 0) continue;
		bool ok = true;
		for (auto &[x, _] : factorize(mod-1)) {
			if (g.pow((mod-1)/x) == 1) {
				ok = false;
				break;
			}
		}
		if (ok) return g;
	}
}