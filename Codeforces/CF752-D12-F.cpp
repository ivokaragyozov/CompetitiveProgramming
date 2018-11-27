/**
Solution:
	Idea: According to centroid theorem we always have a node u in the tree such that if we root the
tree in u all the subtrees when we remove the root will have size less than or equal the half of the 
number of nodes in the whole tree. We can use that here to find the centroid considering only the
special nodes. Then the answer is always one and it is this centroid. The we match all special nodes
according to this node.
*/
#include <bits/stdc++.h>

const int32_t MAX_N = 2e5;

int32_t ansNode;
std::vector< std::pair< int32_t, int32_t > > ansPairs;

class Tree {
private:
	struct Node {
		bool isVisited, isSpecial;
		int32_t id, cntInSubtree;
		std::vector< Node* > v;
	};

public:
	int32_t cntNodes;
	Node nodes[MAX_N + 5];

	void Init(int32_t _cntNodes) {
		cntNodes = _cntNodes;
		for(int32_t i = 1; i <= _cntNodes; i++) {
			nodes[i].id = i;
		}
	}

	void AddEdge(int32_t u, int32_t v) {
		nodes[u].v.push_back(&nodes[v]);
		nodes[v].v.push_back(&nodes[u]);
	}

	void DfsPrecompute(Node *nd) {
        nd->isVisited = true;

        if(nd->isSpecial) {
			nd->cntInSubtree = 1;
        }
        else {
			nd->cntInSubtree = 0;
        }

        for(auto &x : nd->v) {
            if(!x->isVisited) {
				DfsPrecompute(x);
				nd->cntInSubtree += x->cntInSubtree;
            }
		}
	}

	Node* FindCentroid(Node *nd) {
		nd->isVisited = true;

		for(auto &x : nd->v) {
			if(!x->isVisited) {
				if(x->cntInSubtree > nodes[1].cntInSubtree / 2) {
					return FindCentroid(x);
				}
			}
		}

		return nd;
	}

	void GetSpecial(Node *nd, std::vector< int32_t > &q) {
		nd->isVisited = true;

		if(nd->isSpecial) {
            q.push_back(nd->id);
		}

		for(auto &x : nd->v) {
			if(!x->isVisited) {
				GetSpecial(x, q);
			}
		}
	}

	void Solve(int32_t k) {
		DfsPrecompute(&nodes[1]);
	
		for(int32_t i = 1; i <= cntNodes; i++) {
			nodes[i].isVisited = false;
		}
		Node *centroid = FindCentroid(&nodes[1]);
		ansNode = centroid->id;
		
		for(int32_t i = 1; i <= cntNodes; i++) {
			nodes[i].isVisited = false;
		}
		centroid->isVisited = true;

		std::vector< int32_t > currNodes;
		for(auto &x : centroid->v) {
			GetSpecial(x, currNodes);
		}

		if(centroid->isSpecial) {
			currNodes.push_back(centroid->id);
		}
		
		for(int32_t i = 0; i < k; i++) {
			ansPairs.push_back({ currNodes[i], currNodes[i + k] });
		}
	}
};

Tree t;

int main() {
	std::ios_base::sync_with_stdio(false);
	std::cin.tie(nullptr);

	int32_t n, k;
	std::cin >> n >> k;

	t.Init(n);
	for(int32_t i = 0; i < n - 1; i++) {
		int32_t u, v;
		std::cin >> u >> v;

		t.AddEdge(u, v);
	}

	for(int32_t i = 0; i < 2 * k; i++) {
		int32_t c;
		std::cin >> c;

		t.nodes[c].isSpecial = true;
	}

	t.Solve(k);

	std::cout << 1 << '\n';
	std::cout << ansNode << '\n';

	for(auto &x : ansPairs) {
		std::cout << x.first << " " << x.second << " " << ansNode << '\n';
	}
}
