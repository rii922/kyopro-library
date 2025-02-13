#include <bits/stdc++.h>
#include "montgomery_mint.cpp"
using namespace std;

struct perm_comb {
	vector<mint> fact, ifact;
	perm_comb(int n) : fact(n+1), ifact(n+1) {
		fact[0] = 1;
		for (int i = 0; i < n; i++) fact[i+1] = fact[i] * (i+1);
		ifact[n] = fact[n].inv();
		for (int i = n-1; i >= 0; i--) ifact[i] = ifact[i+1] * (i+1);
	}
	mint perm(int n, int r) {
		if (r < 0 || r > n) return 0;
		return fact[n] * ifact[n-r];
	}
	mint comb(int n, int r) {
		if (r < 0 || r > n) return 0;
		return fact[n] * ifact[r] * ifact[n-r];
	}
	mint comb_rep(int n, int r) {
		return comb(n+r-1, r);
	}
};