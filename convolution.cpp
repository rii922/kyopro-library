#include <bits/stdc++.h>
using namespace std;

template<int mod>
struct ntt_friendly_convolution {
	using mint = static_mint<mod>;
	ntt_friendly_convolution() {}
	vector<mint> operator()(const vector<mint> &a, const vector<mint> &b) const {
		int n = a.size() + b.size() - 1;
		int n2 = 1;
		while (n2 < n) n2 *= 2;
		assert((mod - 1) % n2 == 0);
		vector<mint> root(n2+1), a2(n2, 0), b2(n2, 0);
		root[0] = 1;
		mint g = PR.pow((mod-1)/n2);
		for (int i = 0; i < n2; i++) root[i+1] = root[i] * g;
		for (int i = 0; i < a.size(); i++) a2[i] = a[i];
		for (int i = 0; i < b.size(); i++) b2[i] = b[i];
		_ntt(a2, root);
		_ntt(b2, root);
		for (int i = 0; i < n2; i++) a2[i] *= b2[i];
		_intt(a2, root);
		mint ni = mint(n2).inv();
		vector<mint> c(n);
		for (int i = 0; i < n; i++) c[i] = a2[i] * ni;
		return c;
	}
	vector<int> operator()(const vector<int> &a, const vector<int> &b) const {
		int n = a.size() + b.size() - 1;
		int n2 = 1;
		while (n2 < n) n2 *= 2;
		assert((mod - 1) % n2 == 0);
		vector<mint> root(n2+1), a2(n2, 0), b2(n2, 0);
		root[0] = 1;
		mint g = PR.pow((mod-1)/n2);
		for (int i = 0; i < n2; i++) root[i+1] = root[i] * g;
		for (int i = 0; i < a.size(); i++) a2[i] = a[i];
		for (int i = 0; i < b.size(); i++) b2[i] = b[i];
		_ntt(a2, root);
		_ntt(b2, root);
		for (int i = 0; i < n2; i++) a2[i] *= b2[i];
		_intt(a2, root);
		mint ni = mint(n2).inv();
		vector<int> c(n);
		for (int i = 0; i < n; i++) c[i] = (a2[i] * ni).val();
		return c;
	}
private:
	static const mint PR;
	void _ntt(vector<mint> &v, vector<mint> &root) const {
		if (v.size() <= 1) return;
		if (v.size() == 2) {
			mint v0 = v[0];
			v[0] = v0 + v[1];
			v[1] = v0 - v[1];
			return;
		}
		int e = __builtin_ctz(v.size());
		int d = e;
		int n2 = 1 << e;
		if (e & 1) {
			int b = n2 >> 1;
			for (int i = 0; i < b; i++) {
				mint s = v[i];
				mint t = v[i+b];
				v[i] = s + t;
				v[i+b] = (s - t) * root[n2-i];
			}
			d--;
		}
		int b = 1 << (d - 2);
		while (d >= 2) {
			for (int i = 0; i < n2; i += (b << 2)) {
				for (int j = 0; j < b; j++) {
					int p0 = i + j;
					int p1 = p0 + b;
					int p2 = p1 + b;
					int p3 = p2 + b;
					mint t0 = v[p0];
					mint t1 = v[p1];
					mint t2 = v[p2];
					mint t3 = v[p3];
					mint t0p2 = t0 + t2;
					mint t1p3 = t1 + t3;
					mint t0m2 = (t0 - t2) * root[n2-(j<<(e-d))];
					mint t1m3 = (t1 - t3) * root[n2-((j+b)<<(e-d))];
					v[p0] = t0p2 + t1p3;
					v[p1] = (t0p2 - t1p3) * root[n2-(j<<(e-d+1))];
					v[p2] = t0m2 + t1m3;
					v[p3] = (t0m2 - t1m3) * root[n2-(j<<(e-d+1))];
				}
			}
			d -= 2;
			b >>= 2;
		}
	}
	void _intt(vector<mint> &v, vector<mint> &root) const {
		if (v.size() <= 1) return;
		if (v.size() == 2) {
			mint v0 = v[0];
			v[0] = v0 + v[1];
			v[1] = v0 - v[1];
			return;
		}
		int e = __builtin_ctz(v.size());
		int d = 2;
		int n2 = 1 << e;
		int b = 1;
		while (d <= e) {
			for (int i = 0; i < n2; i += (b << 2)) {
				for (int j = 0; j < b; j++) {
					int p0 = i + j;
					int p1 = p0 + b;
					int p2 = p1 + b;
					int p3 = p2 + b;
					mint t0 = v[p0];
					mint t1 = v[p1] * root[j<<(e-d+1)];
					mint t2 = v[p2];
					mint t3 = v[p3] * root[j<<(e-d+1)];
					mint t0p1 = t0 + t1;
					mint t2p3 = (t2 + t3) * root[j<<(e-d)];
					mint t0m1 = t0 - t1;
					mint t2m3 = (t2 - t3) * root[(j+b)<<(e-d)];
					v[p0] = t0p1 + t2p3;
					v[p1] = t0m1 + t2m3;
					v[p2] = t0p1 - t2p3;
					v[p3] = t0m1 - t2m3;
				}
			}
			d += 2;
			b <<= 2;
		}
		if (e & 1) {
			b = n2 >> 1;
			for (int i = 0; i < b; i++) {
				mint s = v[i];
				mint t = v[i+b] * root[i];
				v[i] = s + t;
				v[i+b] = s - t;
			}
		}
	}
};
template<int mod> const static_mint<mod> ntt_friendly_convolution<mod>::PR = []() {
	using mint = ntt_friendly_convolution<mod>::mint;
	vector<int> primes;
	int x = mod - 1;
	for (int i = 2; (long long)i * i <= x; i++) {
		if (x % i == 0) {
			primes.push_back(i);
			while (x % i == 0) x /= i;
		}
	}
	if (x > 1) primes.push_back(x);
	mt19937 mt(time(nullptr));
	while (true) {
		mint g = mt();
		bool ok = true;
		for (auto &x : primes) {
			if (g.pow((mod-1)/x) == 1) {
				ok = false;
				break;
			}
		}
		if (ok) return g;
	}
}();
template<int mod>
struct arbitrary_convolution {
	using mint = static_mint<mod>;
	arbitrary_convolution() {}
	vector<mint> operator()(const vector<mint> &a, const vector<mint> &b) const {
		vector<int> ia(a.size()), ib(b.size());
		for (int i = 0; i < a.size(); i++) ia[i] = a[i].val();
		for (int i = 0; i < b.size(); i++) ib[i] = b[i].val();
		vector<int> ic0 = _conv0(ia, ib);
		vector<int> ic1 = _conv1(ia, ib);
		vector<int> ic2 = _conv2(ia, ib);
		vector<mint> c(ic0.size());
		for (int i = 0; i < c.size(); i++) {
			int t0 = ic0[i];
			int t1 = (long long)(ic1[i] - t0 + 469762049) * _r01 % 469762049;
			int t2 = ((long long)(ic2[i] - t0 + 754974721) * _r02r12 + (long long)(-t1 + 754974721) * _r12) % 754974721;
			c[i] = (long long)t0 + (long long)t1 * 167772161 + (long long)t2 * (167772161LL * 469762049LL % mod);
		}
		return c;
	}
	vector<int> operator()(const vector<int> &a, const vector<int> &b) const {
		vector<int> c0 = _conv0(a, b);
		vector<int> c1 = _conv1(a, b);
		vector<int> c2 = _conv2(a, b);
		vector<int> c(c0.size());
		int w = 167772161LL * 469762049LL % mod;
		for (int i = 0; i < c.size(); i++) {
			int t0 = c0[i];
			int t1 = (long long)(c1[i] - t0 + 469762049) * _r01 % 469762049;
			int t2 = ((long long)(c2[i] - t0 + 754974721) * _r02r12 + (long long)(-t1 + 754974721) * _r12) % 754974721;
			c[i] = ((long long)t0 + (long long)t1 * 167772161 + (long long)t2 * w) % mod;
		}
		return c;
	}
private:
	const ntt_friendly_convolution<167772161> _conv0;
	const ntt_friendly_convolution<469762049> _conv1;
	const ntt_friendly_convolution<754974721> _conv2;
	const int _r01 = static_mint<469762049>(167772161).inv().val();
	const int _r02 = static_mint<754974721>(167772161).inv().val();
	const int _r12 = static_mint<754974721>(469762049).inv().val();
	const int _r02r12 = (long long)_r02 * _r12 % 754974721;
};