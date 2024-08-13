#include <bits/stdc++.h>
using namespace std;

template<class T>
void dfs_comb(vector<T> &arr, int idx, int r, vector<T> &pattern, vector<vector<T>> &result) {
	if (r == 0) {
		result.push_back(pattern);
		return;
	}
	if (idx+r > arr.size()) return;
	pattern.push_back(arr[idx]);
	dfs_comb(arr, idx+1, r-1, pattern, result);
	pattern.pop_back();
	dfs_comb(arr, idx+1, r, pattern, result);
}
template <class T>
vector<vector<T>> enum_comb(vector<T> &arr, int r) {
	vector<T> pattern;
	vector<vector<T>> result;
	dfs_comb(arr, 0, r, pattern, result);
	return result;
}
vector<vector<int>> enum_multi_choose(int n, int r) {
	vector<int> arr(n+r-1);
	for (int i = 0; i < n+r-1; i++) arr[i] = i;
	vector<vector<int>> res;
	for (auto &x : enum_comb(arr, n-1)) {
		vector<int> y;
		y.push_back(-1);
		for (auto &z : x) y.push_back(z);
		y.push_back(n+r-1);
		vector<int> multi_choose(n);
		for (int i = 0; i < n; i++) multi_choose[i] = y[i+1]-y[i]-1;
		res.push_back(multi_choose);
	}
	return res;
}
vector<vector<int>> enum_multi_choose_positive(int n, int r) {
	vector<vector<int>> res;
	for (auto &x : enum_multi_choose(r-n, n)) {
		for (auto &y : x) y++;
		res.push_back(x);
	}
	return res;
}