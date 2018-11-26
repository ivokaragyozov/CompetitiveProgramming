/**
Solution:
	Idea: As edge weights are positive our goal is to make as many nodes to pass
through each edge. Each edge splits the tree into two subtrees so the optimal number of
passes through that edge will be the size of the smaller component.

	1) Do a DFS to compute subtree sizes and parents for each nodes
	2) For each edge add to the answer 2 * x * w where x is the size
of the smaller component and w is the weight of the edge. To find the sizes of the two components
we can use the precomputed values from the DFS. We take the lower vertex and the two components' 
sizes are (this node's subtree size) and (n - this node's subtree size)
*/

#include <bits/stdc++.h>

const int32_t MAX_N = 1e5;

class Tree {
private:
	struct Node {
		int32_t subtreeSize;
		Node *parent;
		std::vector< Node* > v;
	
		void Init() {
			subtreeSize = 1;
			v.clear();
			parent = nullptr;
		}
	};

public:
	Node nodes[MAX_N + 5];

	void Init(int32_t _cntNodes) {
		for(int32_t i = 1; i <= _cntNodes; i++) {
			nodes[i].Init();
		}
	}
	
	void AddEdge(int32_t u, int32_t v) {
		nodes[u].v.push_back(&nodes[v]);
		nodes[v].v.push_back(&nodes[u]);
	}

	void Dfs(Node *nd) {
		for(auto &x : nd->v) {
			if(x == nd->parent) {
				continue;
			}
			
			x->parent = nd;
			Dfs(x);
			nd->subtreeSize += x->subtreeSize;
		}
	}
};

int32_t u[MAX_N + 5], v[MAX_N + 5], w[MAX_N + 5];
Tree t;

int main() {
	std::ios_base::sync_with_stdio(false);
	std::cin.tie(nullptr);

	int32_t cntTests;
	std::cin >> cntTests;

	for(int32_t cs = 1; cs <= cntTests; cs++) {
		int32_t n;
		std::cin >> n;

		t.Init(n);

		for(int32_t i = 0; i < n - 1; i++) {
			std::cin >> u[i] >> v[i] >> w[i];

			t.AddEdge(u[i], v[i]);
		}

		t.Dfs(&t.nodes[1]);
		
		int64_t ans = 0;
		for(int32_t i = 0; i < n - 1; i++) {
			if(t.nodes[v[i]].parent == &t.nodes[u[i]]) {
				std::swap(u[i], v[i]);
			}

			int32_t subtree1 = t.nodes[u[i]].subtreeSize;
			int32_t subtree2 = n - t.nodes[u[i]].subtreeSize;

			ans += (int64_t) 2 * std::min(subtree1, subtree2) * w[i];
		}

		std::cout << "Case #" << cs << ": " << ans << '\n';
	}
}

