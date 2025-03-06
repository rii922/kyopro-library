/**
 * @file count_primes.hpp
 * @author rii922
 * @brief Meissel-Lehmer アルゴリズムによって `n` 以下の素数の数を数える。
 * @date 2025-03-07
 *
 * Verified with:
 * https://judge.yosupo.jp/submission/271572
 */

#include <bits/stdc++.h>
using namespace std;

/// EXPAND FROM HERE

/**
 * @brief Meissel-Lehmer アルゴリズムによって `n` 以下の素数の数を数える。
 *
 * O(n^(2/3) polylog n)
 * @param n 上限
 * @return `n` 以下の素数の数
 */
long long count_primes(long long n) {
	constexpr long long N = 100000;
	constexpr long long A = 100;
	constexpr long long P = 10000000;
	vector<bool> sieve(P, true);
	vector<int> primes, count(P, 0);
	for (long long i = 2; i < P; i++) {
		count[i] = count[i-1];
		if (sieve[i]) {
			for (long long j = i*i; j < P; j += i) sieve[j] = false;
			primes.push_back(i);
			count[i]++;
		}
	}
	vector<vector<long long>> dp(N, vector<long long>(A));
	for (long long i = 0; i < N; i++) dp[i][0] = i;
	for (long long i = 0; i < N; i++) {
		for (long long j = 1; j < A; j++) dp[i][j] = dp[i][j-1] - dp[i/primes[j-1]][j-1];
	}
	function<long long(long long, long long)> phi = [&](long long m, long long a) {
		if (a == 0) return m;
		if (m < N && a < A) return dp[m][a];
		if (m <= primes[a-1]*primes[a-1] && m < P) return count[m] - a + 1;
		return phi(m, a-1) - phi(m/primes[a-1], a-1);
	};
	function<long long(long long)> _count_primes = [&](long long m) {
		if (m < P) return (long long)count[m];
		long long s = sqrt(m+0.5);
		long long y = cbrt(m+0.5);
		long long a = count[y];
		long long res = phi(m, a) + a - 1;
		for (long long i = a; primes[i] <= s; i++) {
			res += _count_primes(primes[i]) - _count_primes(m/primes[i]) - 1;
		}
		return res;
	};
	return _count_primes(n);
}