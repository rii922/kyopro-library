/**
 * @file discrete_log.hpp
 * @author rii922
 * @brief 離散対数を解けるよう、 `mint` を拡張する。 `discrete_log(x, y, m)` で呼ぶこともできる。
 * @date 2025-03-01
 *
 * Verified with:
 * https://judge.yosupo.jp/submission/270450
 */

#include <bits/stdc++.h>
#include "../mint/montgomery_mint.hpp"
#include "../mint/dynamic_montgomery_mint.hpp"
#include "pow_mod.hpp"
using namespace std;

/// EXPAND FROM HERE

/**
 * @brief `x^n=y (mod m)` となる最小の `n` を返す。存在しない場合は -1 を返す。
 *
 * O(m^(1/2))
 * @param x 底
 * @param y 真数
 * @param m 法
 * @return `x^n=y (mod m)` となる最小の `n` または -1
 */
long long discrete_log(int x, int y, unsigned int m) {
	long long s = round(sqrt(m)) + 1;
	unordered_map<unsigned int, long long> ma;
	long long z = y;
	for (int i = 1; i <= s; i++) {
		z = z*x%m;
		if (!ma.count(z)) ma[z] = i;
	}
	long long g = pow_mod<unsigned int, unsigned long long>(x, s, m);
	long long w = 1 % m;
	for (int i = 0; i <= s; i++) {
		if (ma.count(w*g%m)) {
			long long ww = w;
			for (int j = 0; j < s; j++) {
				if (ww == y) return s*i+j;
				ww = ww*x%m;
			}
		}
		w = w*g%m;
	}
	return -1;
}

/**
 * @brief `mint` の上で `x^n=y` となる最小の `n` を返す。存在しない場合は -1 を返す。
 *
 * O(mod^(1/2))
 * @tparam _int `_uint` と同じ精度の符号付き整数型
 * @tparam _uint `mod*4` を表現可能な符号無し整数型
 * @tparam _long `_int` の倍の精度の符号付き整数型
 * @tparam _ulong `_uint` の倍の精度の符号無し整数型
 * @tparam mod `mod*4` が `_uint` に収まるような奇数
 * @param x 底
 * @param y 真数
 * @return `mint` の上で `x^n=y` となる最小の `n` または -1
 */
template<gnu_signed_integral _int, gnu_unsigned_integral _uint, gnu_signed_integral _long, gnu_unsigned_integral _ulong, _uint mod>
requires (sizeof(_int) == sizeof(_uint)) && (sizeof(_long) == sizeof(_ulong)) && (sizeof(_int)*2 == sizeof(_long)) && (mod < _uint(1) << (sizeof(_uint)*8 - 2)) && (mod % 2 == 1)
long long static_montgomery_mint<_int, _uint, _long, _ulong, mod>::discrete_log(mint x, mint y) {
	return discrete_log(x.val(), y.val(), mod);
}

/**
 * @brief `mint` の上で `x^n=y` となる最小の `n` を返す。存在しない場合は -1 を返す。
 *
 * O(mod^(1/2))
 * @tparam _int `_uint` と同じ精度の符号付き整数型
 * @tparam _uint `mod*4` を表現可能な符号無し整数型
 * @tparam _long `_int` の倍の精度の符号付き整数型
 * @tparam _ulong `_uint` の倍の精度の符号無し整数型
 * @tparam id 型を区別するための ID 。同じ ID を持つインスタンスは同じ `mod` を持つ。
 * @param x 底
 * @param y 真数
 * @return `mint` の上で `x^n=y` となる最小の `n` または -1
 */
template<gnu_signed_integral _int, gnu_unsigned_integral _uint, gnu_signed_integral _long, gnu_unsigned_integral _ulong, int id>
requires (sizeof(_int) == sizeof(_uint)) && (sizeof(_long) == sizeof(_ulong)) && (sizeof(_int)*2 == sizeof(_long))
long long dynamic_montgomery_mint<_int, _uint, _long, _ulong, id>::discrete_log(mint x, mint y) {
	return discrete_log(x.val(), y.val(), mod);
}