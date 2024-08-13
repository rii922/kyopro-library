#include <bits/stdc++.h>
using namespace std;

template<class T>
struct matrix {
	int h, w;
	vector<vector<T>> d;
	matrix(int h, int w, T val=0) : h(h), w(w), d(h, vector<T>(w, val)) {}
	matrix(int h) : h(h), w(h), d(h, vector<T>(h, 0)) {}
	static matrix<T> unit(int n) {
		matrix<T> res(n);
		for (int i = 0; i < n; i++) res[i][i] = 1;
		return res;
	}
	const vector<T> &operator[](int i) const { return d[i]; }
	vector<T> &operator[](int i) { return d[i]; }
	matrix<T> operator-() const {
		matrix<T> res(h, w);
		for (int i = 0; i < h; i++) for (int j = 0; j < w; j++) {
			res[i][j] = -d[i][j];
		}
		return res;
	}
	matrix<T> &operator+=(const matrix<T> &a) {
		assert(h == a.h && w == a.w);
		for (int i = 0; i < h; i++) for (int j = 0; j < w; j++) {
			d[i][j] += a[i][j];
		}
		return *this;
	}
	matrix<T> &operator-=(const matrix<T> &a) {
		assert(h == a.h && w == a.w);
		for (int i = 0; i < h; i++) for (int j = 0; j < w; j++) {
			d[i][j] -= a[i][j];
		}
		return *this;
	}
	matrix<T> &operator*=(const matrix<T> &a) {
		assert(w == a.h);
		matrix<T> res(h, a.w);
		for (int i = 0; i < h; i++) for (int k = 0; k < w; k++) for (int j = 0; j < a.w; j++) {
			res[i][j] += d[i][k] * a[k][j];
		}
		return *this = res;
	}
	matrix<T> operator+(const matrix<T> &a) { return matrix<T>(*this) += a; }
	matrix<T> operator-(const matrix<T> &a) { return matrix<T>(*this) -= a; }
	matrix<T> operator*(const matrix<T> &a) { return matrix<T>(*this) *= a; }
	matrix<T> pow(long long t) const {
		assert(h == w);
		if (t < 0) return matrix<T>(h);
		if (t == 0) return matrix<T>::unit(h);
		if (t == 1) return *this;
		matrix<T> res = pow(t >> 1);
		res *= res;
		if (t & 1) res *= *this;
		return res;
	}
	friend istream &operator>>(istream &is, matrix<T> &a) {
		for (int i = 0; i < a.h; i++) for (int j = 0; j < a.w; j++) {
			is >> a[i][j];
		}
		return is;
	}
	friend ostream &operator<<(ostream &os, const matrix<T> &a) {
		for (int i = 0; i < a.h; i++) for (int j = 0; j < a.w; j++) {
			os << a[i][j] << (j == a.w-1 ? '\n' : ' ');
		}
		return os;
	}
};