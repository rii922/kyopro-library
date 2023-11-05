#include <bits/stdc++.h>
using namespace std;

template<typename T>
struct Matrix {
	int h, w;
	vector<vector<T>> d;
	Matrix(int h, int w, T val = T()): h(h), w(w), d(h, vector<T>(w, val)) {}
	Matrix(int h): h(h), w(h), d(h, vector<T>(h)) {}
	Matrix &unit() {
		assert(h == w);
		for (int i = 0; i < h; i++) d[i][i] = 1;
		return *this;
	}
	Matrix &zero() { return *this = Matrix(h, w); }
	const vector<T> &operator[](int i) const { return d[i]; }
	vector<T> &operator[](int i) { return d[i]; }
	Matrix operator-() const {
		Matrix r(h, w);
		for (int i = 0; i < h; i++) for (int j = 0; j < w; j++) {
			r[i][j] = -d[i][j];
		}
		return r;
	}
	Matrix &operator+=(const Matrix &a) {
		assert(h == a.h && w == a.w);
		for (int i = 0; i < h; i++) for (int j = 0; j < w; j++) {
			d[i][j] += a[i][j];
		}
		return *this;
	}
	Matrix &operator-=(const Matrix &a) {
		assert(h == a.h && w == a.w);
		for (int i = 0; i < h; i++) for (int j = 0; j < w; j++) {
			d[i][j] -= a[i][j];
		}
		return *this;
	}
	Matrix &operator*=(const Matrix &a) {
		assert(w == a.h);
		Matrix r(h, a.w);
		for (int i = 0; i < h; i++) for (int k = 0; k < w; k++) for (int j = 0; j < a.w; j++) {
			r[i][j] += d[i][k]*a[k][j];
		}
		return *this = r;
	}
	Matrix operator+(const Matrix &a) { return Matrix(*this) += a; }
	Matrix operator-(const Matrix &a) { return Matrix(*this) -= a; }
	Matrix operator*(const Matrix &a) { return Matrix(*this) *= a; }
	Matrix pow(long long t) const {
		assert(h == w);
		if (t < 0) return Matrix(h);
		if (t == 0) return Matrix(h).unit();
		if (t == 1) return *this;
		Matrix r = pow(t >> 1);
		r *= r;
		if (t & 1) r *= *this;
		return r;
	}
	friend istream &operator>>(istream &is, Matrix &a) {
		for (int i = 0; i < a.h; i++) for (int j = 0; j < a.w; j++) {
			is >> a[i][j];
		}
		return is;
	}
	friend ostream &operator<<(ostream &os, const Matrix &a) {
		for (int i = 0; i < a.h; i++) for (int j = 0; j < a.w; j++) {
			os << a[i][j] << (j==a.w-1 ? '\n' : ' ');
		}
		return os;
	}
};