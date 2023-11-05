#include <bits/stdc++.h>
using namespace std;

template <typename T>
vector<T> compress(vector<T> &X) {
	vector<T> vals = X;
	sort(vals.begin(), vals.end());
	vals.erase(unique(vals.begin(), vals.end()), vals.end());
	for (int i = 0; i < X.size(); i++) {
		X[i] = lower_bound(vals.begin(), vals.end(), X[i]) - vals.begin();
	}
	return vals;
}