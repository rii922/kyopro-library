#include <bits/stdc++.h>
using namespace std;

/// EXPAND FROM HERE

vector<pair<int, int>> convex_hull(vector<pair<int, int>> &v) {
	if (v.size() == 0) return {};
	const int INF = 2001001001;
	pair<int, int> p = {INF, INF};
	for (int i = 0; i < v.size(); i++) {
		if (p.first > v[i].first) p.first = v[i].first;
	}
	for (int i = 0; i < v.size(); i++) {
		if (p.first == v[i].first && p.second > v[i].second) p.second = v[i].second;
	}
	sort(v.begin(), v.end(), [&p](pair<int, int> a, pair<int, int> b) {
		if (a == p) return false;
		if (b == p) return true;
		long long xa = (long long)a.first - p.first;
		long long ya = (long long)a.second - p.second;
		long long xb = (long long)b.first - p.first;
		long long yb = (long long)b.second - p.second;
		if (xa == 0 && xb == 0) return ya < yb;
		if (xa == 0) return false;
		if (xb == 0) return true;
		if (ya * xb == yb * xa) return xa < xb;
		return ya * xb < yb * xa;
	});
	vector<pair<int, int>> res;
	res.push_back(p);
	for (int i = 0; i < v.size(); i++) {
		while (true) {
			int n = res.size();
			if (n < 2) break;
			if (n == 2 && res[0] == p && res[1] != p && v[i] == p) break;
			long long x0 = (long long)res[n-1].first - res[n-2].first;
			long long y0 = (long long)res[n-1].second - res[n-2].second;
			long long x1 = (long long)v[i].first - res[n-1].first;
			long long y1 = (long long)v[i].second - res[n-1].second;
			if (x0 * y1 > x1 * y0) break;
			else res.pop_back();
		}
		if (v[i] == p) break;
		res.push_back(v[i]);
	}
	return res;
}