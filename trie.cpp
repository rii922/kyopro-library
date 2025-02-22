#include <bits/stdc++.h>
using namespace std;

/// EXPAND FROM HERE

template<int base, int char_size>
struct trie {
	trie() : _root(nullptr) {}
	trie(initializer_list<string> init) : _root(nullptr) {
		for (const string &x : init) insert(x);
	}
	void insert(const string &s) {
		_root = _insert(_root, s, 0);
	}
	void erase(const string &s) {
		_erase(_root, s, 0);
	}
	string operator[](int n) {
		string res;
		_nth(_root, n, res);
		return res;
	}
	int size() {
		return _size(_root);
	}
	int count(const string &s) {
		node *t = _root;
		for (int i = 0; i < s.size(); i++) {
			if (t == nullptr) return 0;
			t = t->next[s[i]-base];
		}
		return _count(t);
	}
	int count_perfix(const string &s) {
		node *t = _root;
		for (int i = 0; i < s.size(); i++) {
			if (t == nullptr) return 0;
			t = t->next[s[i]-base];
		}
		return _size(t);
	}
private:
	struct node {
		int size, count;
		array<node *, char_size> next;
		node() : size(0), count(0) {
			next.fill(nullptr);
		}
	};
	node *_root;
	int _size(node *t) {
		return t == nullptr ? 0 : t->size;
	}
	int _count(node *t) {
		return t == nullptr ? 0 : t->count;
	}
	node *_insert(node *t, const string &s, int p) {
		if (t == nullptr) t = new node();
		t->size++;
		if (p >= s.size()) t->count++;
		else t->next[s[p]-base] = _insert(t->next[s[p]-base], s, p+1);
		return t;
	}
	bool _erase(node *t, const string &s, int p) {
		if (t == nullptr) return false;
		if (p >= s.size()) {
			if (t->count > 0) {
				t->size--;
				t->count--;
				return true;
			}
			else return false;
		}
		else if (_erase(t->next[s[p]-base], s, p+1)) {
			t->size--;
			return true;
		}
		else return false;
	}
	void _nth(node *t, int n, string &res) {
		assert(n < _size(t));
		if (n < _count(t)) return;
		n -= _count(t);
		for (int i = 0; i < char_size; i++) {
			if (n < _size(t->next[i])) {
				res += char(i+base);
				_nth(t->next[i], n, res);
				return;
			}
			else n -= _size(t->next[i]);
		}
	}
};
using upper_trie = trie<'A', 26>;
using lower_trie = trie<'a', 26>;