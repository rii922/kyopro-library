/**
 * @file pow_mod.hpp
 * @author rii922
 * @brief 冪乗の剰余を求める。
 * @date 2025-02-24
 */

#include <bits/stdc++.h>
#include "../custom_concepts.hpp"
using namespace std;

/// EXPAND FROM HERE

/**
 * @brief 冪乗の剰余を求める。
 *
 * O(log y)
 * @tparam _int `mod` の型
 * @tparam _long `_int` の倍の精度の整数型
 * @param x 底
 * @param y 指数
 * @param mod 法
 * @return 冪乗の剰余
 */
template<gnu_integral _int, gnu_integral _long>
constexpr _int pow_mod(_long x, _long y, _int mod) {
	if (y == 0) return 1 % mod;
	_long res = pow_mod<_int, _long>(x, y/2, mod);
	res = res*res%mod;
	if (y % 2 == 1) res = res*x%mod;
	return res;
}