#include <bits/stdc++.h>
using namespace std;

/// EXPAND FROM HERE

long long INF = 1LL << 60;
template<class T>
int lis(vector<T> &a) {
	vector<long long> dp(a.size(), INF);
	for (int i = 0; i < a.size(); i++) {
		auto it = lower_bound(dp.begin(), dp.end(), a[i]);
		*it = a[i];
	}
	return lower_bound(dp.begin(), dp.end(), INF) - dp.begin();
}