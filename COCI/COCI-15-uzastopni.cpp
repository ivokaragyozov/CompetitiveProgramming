/**
Solution:
	Idea: We can do a dp on tree. The dp in each node will be the set 
of intervals of jokes which are possible to be taken if we consider
only the subtree of the current node. We can compute the dp for each
node after computing the dps of all of its children by mergeing them.
How do we merge? Iterate from MAX_K to 1 and for each beggining compute
possible endings by combining intervals. This solution has complexity 
O(N * K ^ 3) but we can optimize it by using bitset.
*/

#include <bits/stdc++.h>

const int32_t MAX_N = 10000;
const int32_t MAX_K = 100;

std::vector< int32_t > q[MAX_K + 5];

class Tree {
private:
	struct Node {
		bool isVisited;
		int32_t id, joke;
		std::vector< Node* > v;
		std::vector< std::pair< int32_t, int32_t > > dp;
		std::bitset< MAX_K + 5 > intervals[MAX_K + 5];
	};

public:
	int32_t cntNodes;
	Node nodes[MAX_N + 5];

	void Init(int32_t pCntNodes = 0) {
		cntNodes = pCntNodes;
		for(int32_t i = 1; i <= cntNodes; i++) {
			nodes[i].id = i;
		}
	}

	void AddEdge(Node *x, Node *y) {
		x->v.push_back(y);
		y->v.push_back(x);
	}

	void CalcDp(Node *nd) {
		nd->isVisited = true;
		
		Node *par = nullptr;
		for(auto &x : nd->v) {
			if(!x->isVisited) {
				CalcDp(x);
			}
			else {
				par = x;
			}
		}

		for(int32_t i = 1; i <= MAX_K; i++) {
			q[i].clear();
		}
		
		for(auto &x : nd->v) {
			if(x != par) {
				for(auto &i : x->dp) {
					q[i.first].push_back(i.second);
				}
			}
		}

		for(int32_t i = MAX_K; i >= 1; i--) {
			if(i == nd->joke) {
				nd->intervals[i] |= nd->intervals[i + 1];
				nd->intervals[i].set(i);
			}
			else {
				for(auto &x : q[i]) {
					if(x < nd->joke || i > nd->joke) {
						nd->intervals[i] |= nd->intervals[x + 1];
						nd->intervals[i].set(x);
					}
				}
			}

			for(int32_t j = MAX_K; j >= i; j--) {
				if(nd->intervals[i].test(j)) {
					if(nd->joke >= i && nd->joke <= j) {
						nd->dp.push_back({ i, j });
					}
				}
			}
		}
	}
};

Tree t;

int main() {
	std::ios_base::sync_with_stdio(false);
	std::cin.tie(nullptr);

	int32_t n;
	std::cin >> n;
	
	t.Init(n);
	for(int32_t i = 1; i <= n; i++) {
		std::cin >> t.nodes[i].joke;
	}

	for(int32_t i = 0; i < n - 1; i++) {
		int32_t x, y;
		std::cin >> x >> y;
		t.AddEdge(&t.nodes[x], &t.nodes[y]);
	}

	t.CalcDp(&t.nodes[1]);

	std::cout << t.nodes[1].dp.size() << '\n';
}

