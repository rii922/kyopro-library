#include <bits/stdc++.h>
using namespace std;

struct fraction {
	long long p, q;
	fraction(long long p = 0, long long q = 1) : p(p), q(q) {}
	bool operator==(const fraction &a) {
		return p * a.q == q * a.p;
	}
	bool operator<(const fraction &a) {
		return p * a.q < q * a.p;
	}
	bool operator<=(const fraction &a) {
		return p * a.q <= q * a.p;
	}
};