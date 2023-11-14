#include <bits/stdc++.h>
using namespace std;

template <class T>
void dfs_comb(vector<T> &arr, int idx, int r, vector<T> &pattern, vector<vector<T>> &result) {
	if (r == 0) {
		result.push_back(pattern);
		return;
	}
	if ((idx + r) > arr.size()) {
		return;
	}
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