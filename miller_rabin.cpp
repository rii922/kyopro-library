#include <bits/stdc++.h>
#include "dynamic_montgomery_mint.cpp"
using namespace std;

bool miller_rabin(long long x) {
	using _mint64 = dynamic_mint64<static_cast<int>(0xc56a4ffc)>;
	if (x == 2) return true;
	if (x < 2 || x % 2 == 0) return false;
	_mint64::set_mod(x);
	long long d = x - 1;
	long long s = 0;
	while (d % 2 == 0) {
		d /= 2;
		s++;
	}
	const vector<long long> bases = (x < 1LL << 32) ? vector<long long>{2, 7, 61} : vector<long long>{2, 325, 9375, 28178, 450775, 9780504, 1795265022};
	for (auto &b : bases) {
		_mint64 a = b;
		if (a == 0) continue;
		_mint64 t = a.pow(d);
		if (t == 1) continue;
		bool e = true;
		for (int i = 0; i < s; i++) {
			if (t == -1) {
				e = false;
				break;
			}
			t *= t;
		}
		if (e) return false;
	}
	return true;
}