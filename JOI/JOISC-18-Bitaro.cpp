/**
Solution:
	Idea: We can precompute for each node the sqrt(n) furthest nodes with a simple dp. Then for each query
if the number of forbidden nodes is less than sqrt(n) then for sure the answer will be among the precomputed
otherwise we can simply solve it with a brutefore since the queries of this type will be less than sqrt(n).
*/
#include <bits/stdc++.h>

const int32_t MAX_N = 1e5;
const int32_t BUCKET_SIZE = 300;
const int32_t INF = 2e9;

bool isInVector[MAX_N + 5];
bool isForbidden[MAX_N + 5];

std::vector< std::pair< int32_t, int32_t > > Merge(const std::vector< std::pair< int32_t, int32_t> > &v1, 
		const std::vector< std::pair< int32_t, int32_t > > &v2) {
	std::vector< std::pair< int32_t, int32_t > > ans;

	int32_t ind1 = 0, ind2 = 0;
	while((ind1 < v1.size() || ind2 < v2.size()) && ans.size() < BUCKET_SIZE) {
		if(ind1 < v1.size()) {
			if(ind2 < v2.size()) {
				if(v1[ind1].first >= v2[ind2].first + 1) {
					if(!isInVector[v1[ind1].second]) {
						isInVector[v1[ind1].second] = true;
						ans.push_back(v1[ind1]);
					}
					ind1++;
				}
				else {
					if(!isInVector[v2[ind2].second]) {
						isInVector[v2[ind2].second] = true;
						ans.push_back({ v2[ind2].first + 1, v2[ind2].second });
					}
					ind2++;
				}
			}
			else {
				if(!isInVector[v1[ind1].second]) {
					isInVector[v1[ind1].second] = true;
					ans.push_back(v1[ind1]);
				}
				ind1++;
			}
		}
		else {
			if(!isInVector[v2[ind2].second]) {
				isInVector[v2[ind2].second] = true;
				ans.push_back({ v2[ind2].first + 1, v2[ind2].second });
			}
			ind2++;
		}
	}

	for(auto &x : ans) {
		isInVector[x.second] = false;
	}

	return ans;
}

class Graph {
private:
	struct Node {
		int32_t id;
		std::vector< std::pair< int32_t, int32_t > > dp;
		std::vector< Node* > v;
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
	}

	void Precompute() {
		for(int32_t i = 1; i <= cntNodes; i++) {
			std::vector< std::pair< int32_t, int32_t > > dists;
			
			nodes[i].dp.push_back({ 0, i });
			for(auto &x : nodes[i].v) {
				nodes[i].dp = Merge(nodes[i].dp, x->dp);
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

		for(int32_t i = 1; i <= t; i++) {
			if(isForbidden[i]) {
				dp[i] = -INF;
			}
			else {
				dp[i] = 0;
			}

			for(auto &x : nodes[i].v) {
				dp[i] = std::max(dp[i], dp[x->id] + 1);
			}
		}

		return std::max(dp[t], -1);
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
