#include <bits/stdc++.h>
using namespace std;

/// EXPAND FROM HERE

vector<int> suffix_array(vector<int> &v) {
	int n = v.size();
	vector<int> res(n);
	for (int i = 0; i < n; i++) res[i] = i;
	vector<int> rank = v, nrank(n);
	for (int i = 1; i < n; i *= 2) {
		auto comp = [&](int a, int b) {
			if (rank[a] != rank[b]) return rank[a] < rank[b];
			int x = a + i < n ? rank[a+i] : -1;
			int y = b + i < n ? rank[b+i] : -1;
			return x < y;
		};
		sort(res.begin(), res.end(), comp);
		nrank[res[0]] = 0;
		for (int i = 0; i+1 < n; i++) {
			nrank[res[i+1]] = nrank[res[i]] + comp(res[i], res[i+1]);
		}
		swap(rank, nrank);
	}
	return res;
}
vector<int> suffix_array(string &s) {
	vector<int> v(s.size());
	for (int i = 0; i < s.size(); i++) v[i] = s[i];
	return suffix_array(v);
}