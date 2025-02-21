/**
 * @file pow_mod.cpp
 * @author rii922
 * @brief 冪乗の剰余を求める。
 * @date 2025-02-21
 */

#include <bits/stdc++.h>
using namespace std;

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
template<integral _int, integral _long>
constexpr _int pow_mod(_long x, _long y, _int mod) {
	if (y == 0) return 1;
	_long res = pow_mod<_int, _long>(x, y/2, mod);
	res = res*res%mod;
	if (y % 2 == 1) res = res*x%mod;
	return res;
}