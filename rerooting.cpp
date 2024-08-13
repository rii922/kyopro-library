#include <bits/stdc++.h>
using namespace std;

template<class S, S (*op)(S, S), S (*e)(), S (*put_vertex)(S, int), S (*put_edge)(S, int)>
struct rerooting {
	rerooting(int n) : _graph(n), _lprod(n), _rprod(n) {}
	int add_edge(int from, int to) {
		int pos = _graph[from].size();
		int rev_pos = _graph[to].size();
		int index = _nodes.size();
		_graph[from].push_back(node(to, rev_pos, index));
		_graph[to].push_back(node(from, pos, index));
		_nodes.push_back(node_memo(from, pos));
		return index;
	}
	vector<S> calc() {
		_dfs1();
		_dfs2();
		vector<S> res;
		for (int i = 0; i < _graph.size(); i++) res.push_back(put_vertex(_lprod[i][_graph[i].size()], i));
		return res;
	}
	S partial(int index, int s) {
		int from = _nodes[index].from;
		int pos = _nodes[index].pos;
		int to = _graph[from][pos].to;
		int rev_pos = _graph[from][pos].rev_pos;
		if (s != _graph[from][pos].to) {
			swap(from, to);
			swap(pos, rev_pos);
		}
		assert(s == _graph[from][pos].to);
		return put_vertex(op(_lprod[to][rev_pos], _rprod[to][rev_pos+1]), to);
	}
private:
	struct node {
		int to, rev_pos, index;
		S val;
		node(int to, int rev_pos, int index) : to(to), rev_pos(rev_pos), index(index), val(e()) {}
	};
	struct node_memo {
		int from, pos;
		node_memo(int from, int pos) : from(from), pos(pos) {}
	};
	vector<vector<node>> _graph;
	vector<node_memo> _nodes;
	vector<vector<S>> _lprod, _rprod;
	S _dfs1(int s=0, int p=-1) {
		int deg = _graph[s].size();
		S prod = e();
		for (int i = 0; i < deg; i++) {
			int t = _graph[s][i].to;
			if (t == p) continue;
			S partial = put_edge(_dfs1(t, s), _graph[s][i].index);
			_graph[s][i].val = partial;
			prod = op(prod, partial);
		}
		return put_vertex(prod, s);
	}
	void _dfs2(int s=0, int p=-1, S prod_par=e()) {
		int deg = _graph[s].size();
		_lprod[s].resize(deg+1, e());
		_rprod[s].resize(deg+1, e());
		S prod = e();
		for (int i = 0; i < deg; i++) {
			int t = _graph[s][i].to;
			if (t == p) _graph[s][i].val = put_edge(prod_par, _graph[s][i].index);
			prod = op(prod, _graph[s][i].val);
			_lprod[s][i+1] = prod;
		}
		prod = e();
		for (int i = deg-1; i >= 0; i--) {
			prod = op(prod, _graph[s][i].val);
			_rprod[s][i] = prod;
		}
		for (int i = 0; i < deg; i++) {
			int t = _graph[s][i].to;
			if (t == p) continue;
			_dfs2(t, s, put_vertex(op(_lprod[s][i], _rprod[s][i+1]), s));
		}
	}
};