#include <bits/stdc++.h>

const int32_t MAX_N = 500;
const int32_t MAX_K = MAX_N * MAX_N;
const int32_t MAX_M = 500;
const int32_t INF = 2e9;

class Graph {
private:
	struct Node {
		bool isVisited;
		int32_t id, dep;
		std::vector< int32_t > v;
	};

	struct Edge {
		Node *to;
		int32_t cap;
	
		Edge(Node *_to = nullptr, int32_t _cap = 0): to(_to), cap(_cap) {}
	};
	
	int32_t cntNodes, edgeInd;
	Node nodes[MAX_N + MAX_M + 5];
	Node *source, *sink;
	
	void Bfs() {
		std::queue< Node* > q;

		q.push(sink);
		sink->dep = 0;
	
		while(!q.empty()) {
			Node *curr = q.front();
			int32_t dep = curr->dep;
			q.pop();

			if(curr == source) {
				break;
			}

			for(auto &x : curr->v) {
				Node *y = edges[x].to;
				
				if(y->dep == -1 && edges[(x ^ 1)].cap != 0) {
					q.push(y);
					y->dep = dep + 1;
				}
			}
		}
	}

	int32_t Dfs(Node *nd, int32_t curr) {
		if(nd == sink) {
			return curr;
		}

		for(auto &x : nd->v) {
			Node *y = edges[x].to;
			int32_t cap = edges[x].cap;

			if(!y->isVisited && y->dep == nd->dep - 1 && cap != 0) {
				int32_t aux = Dfs(y, std::min(curr, cap));

				if(aux != 0) {
					edges[x].cap -= aux;
					edges[(x ^ 1)].cap += aux;

					return aux;
				}
			}
		}

		nd->isVisited = true;
		return 0;
	}

public:
	Edge edges[MAX_K + MAX_N + MAX_M + 5];
	
	void Init(int32_t _cntNodes) {
		source = &nodes[0];
		sink = &nodes[_cntNodes + 1];
		
		edgeInd = 0;
		cntNodes = _cntNodes;

		for(int32_t i = 0; i <= _cntNodes + 1; i++) {
			nodes[i].id = i;
		}
	}

	void Build(int32_t n, int32_t m, int32_t cntTasks) {
		for(int32_t i = 1; i <= n; i++) {
			edges[edgeInd] = Edge(&nodes[i], cntTasks);
			source->v.push_back(edgeInd);
			edgeInd++;
			
			edges[edgeInd] = Edge(source, 0);
			nodes[i].v.push_back(edgeInd);
			edgeInd++;
		}
		for(int32_t i = 1; i <= m; i++) {
			edges[edgeInd] = Edge(sink, 1);
			nodes[n + i].v.push_back(edgeInd);
			edgeInd++;

			edges[edgeInd] = Edge(&nodes[n + i], 0);
			sink->v.push_back(edgeInd);
			edgeInd++;
		}
	}

	void AddEdge(int32_t u, int32_t v) {
		edges[edgeInd] = Edge(&nodes[v], 1);
		nodes[u].v.push_back(edgeInd);
		edgeInd++;
		
		edges[edgeInd] = Edge(&nodes[u], 0);
		nodes[v].v.push_back(edgeInd);
		edgeInd++;
	}

	int32_t CalcMaxFlow() {
		int32_t maxFlow = 0;

		while(1) {
			for(int32_t i = 1; i <= cntNodes; i++) {
				nodes[i].dep = -1;
				nodes[i].isVisited = false;
			}
			source->dep = -1;
			source->isVisited = false;

			sink->dep = -1;
			sink->isVisited = false;

			Bfs();

			if(source->dep == -1) {
				break;
			}
			
			while(1) {
				int32_t aux = Dfs(source, INF);

				if(aux == 0) {
					break;
				}

				maxFlow += aux;
			}
		}

		return maxFlow;
	}
};

Graph g;

int main() {
	std::ios_base::sync_with_stdio(false);
	std::cin.tie(nullptr);

	int32_t n, m, r, t, k;
	std::cin >> n >> m >> r >> t >> k;
	
	g.Init(n + m);
	g.Build(n, m, t / r);

	for(int32_t i = 0; i < k; i++) {
		int32_t a, b;
		std::cin >> a >> b;

		g.AddEdge(a, n + b);
	}

	std::cout << g.CalcMaxFlow() << '\n';
}

