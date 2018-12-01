/**
Solution:
	Idea: We can precompute for each node the sqrt(n) furthest nodes with a simple dp. Then for each query
if the number of forbidden nodes is less than sqrt(n) then for sure the answer will be among the precomputed
otherwise we can simply solve it with a brutefore since the queries of this type will be less than sqrt(n).
*/
#include <bits/stdc++.h>

const int32_t MAX_N = 1e5;
const int32_t BUCKET_SIZE = 50;

bool isForbidden[MAX_N + 5];

class Graph {
private:
	struct Node {
		int32_t id;
		std::vector< std::pair< int32_t, int32_t > > dp;
		std::vector< Node* > v, rev;
	};

	int32_t cntNodes;
	Node nodes[MAX_N + 5];
	
public:
	void Init(int32_t _cntNodes) {
		cntNodes = _cntNodes;

		for(int32_t i = 1; i <= cntNodes; i++) {
			nodes[i].id = i;
		}
	}

	void AddEdge(int32_t from, int32_t to) {
		nodes[from].v.push_back(&nodes[to]);
		nodes[to].rev.push_back(&nodes[from]);
	}

	void Precompute() {
		for(int32_t i = 1; i <= cntNodes; i++) {
			std::vector< std::pair< int32_t, int32_t > > dists;
			
			dists.push_back({ 0, i });
			for(auto &x : nodes[i].v) {
				for(auto &y : x->dp) {
					dists.push_back({ y.first + 1, y.second });
				}
			}

			std::sort(dists.begin(), dists.end());
			
			for(int32_t j = dists.size() - 1; j >= 0; j--) {
				if(nodes[i].dp.size() == 0 || nodes[i].dp.back().second != dists[j].second) {
					nodes[i].dp.push_back(dists[j]);

					if(nodes[i].dp.size() == BUCKET_SIZE) {
						break;
					}
				}
			}
		}
	}

	int32_t SolveSmallK(int32_t t) {
		for(auto &x : nodes[t].dp) {
			if(!isForbidden[x.second]) {
				return x.first;
			}
		}

		return -1;
	}

	int32_t SolveBigK(int32_t t) {
		std::vector< int32_t > dp(t + 1, 0);

		int32_t ans = (isForbidden[t] ? -1 : 0);
		for(int32_t i = t - 1; i >= 1; i--) {
			bool isChanged = false;
			for(auto &x : nodes[i].rev) {
				if(x->id <= t && (x->id == t || dp[x->id] != 0)) {
					dp[i] = std::max(dp[i], dp[x->id] + 1);
					isChanged = true;
				}
			}
			
			if(isChanged && !isForbidden[i]) {
				ans = std::max(ans, dp[i]);
			}
		}

		return ans;
	}	
};

Graph g;

int main() {
	std::ios_base::sync_with_stdio(false);
	std::cin.tie(nullptr);

	int32_t n, m, q;
	std::cin >> n >> m >> q;

	g.Init(n);
	
	for(int32_t i = 0; i < m; i++) {
		int32_t u, v;
		std::cin >> u >> v;

		g.AddEdge(v, u);
	}
	
	g.Precompute();

	for(int32_t i = 0; i < q; i++) {
		int32_t t, k;
		std::cin >> t >> k;
	
		std::vector< int32_t > c(k);
		for(int32_t j = 0; j < k; j++) {
			std::cin >> c[j];

			isForbidden[c[j]] = true;
		}

		if(k >= BUCKET_SIZE) {
			std::cout << g.SolveBigK(t) << '\n';
		}
		else {
			std::cout << g.SolveSmallK(t) << '\n';
		}

		for(int32_t j = 0; j < k; j++) {
			isForbidden[c[j]] = false;
		}
	}
}
