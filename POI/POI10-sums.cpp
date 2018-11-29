/**
Solution:
	Idea: Every number from A' can be represented as S = (another number from A') + k * a[i] so
when we module by a[0] we get that S mod a[0] should be equal to some other sumber from A' mod a[0]
and this another number should be less than S. So for each reminder from 0 to a[0] - 1 we need to find
the smallest number which is in A' and that number mod a[0] is equal to the reminder. For 0 it is 0.
Then we can build a graph with a[0] nodes in which we have an edge between i and j with weight 
a[p] if (i + a[p]) mod a[0] is equal to j then we need to find shortest paths starting from 0. 
And for each query x we should check the distance for which x mod a[0] and if that distance is
greater than x then the answer is "Yes" otherwise it is "No".
*/
#include <bits/stdc++.h>

const int32_t MAX_N = 5000;
const int32_t MAX_A = 50000;
const int32_t INF = 2e9;

struct State {
	int32_t x, dist;

	bool operator< (const State &other) const {
		return dist > other.dist;
	}
};

bool isVisited[MAX_A + 5];
int32_t a[MAX_N + 5], dist[MAX_A + 5];

int main() {
	std::ios_base::sync_with_stdio(false);
	std::cin.tie(nullptr);

	int32_t n;
	std::cin >> n;

	for(int32_t i = 0; i < n; i++) {
		std::cin >> a[i];
	}

	for(int32_t i = 0; i < a[0]; i++) {
		dist[i] = INF;
	}

	std::priority_queue< State > pq;
	
	pq.push({ 0, 0 });
	dist[0] = 0;
	
	while(!pq.empty()) {
		int32_t curr = pq.top().x;
		int32_t d = pq.top().dist;
		pq.pop();

		if(isVisited[curr]) {
			continue;
		}
		isVisited[curr] = true;

		for(int32_t i = 0; i < n; i++) {
			int32_t nxtNode = (curr + a[i]) % a[0];

			if(!isVisited[nxtNode] && (dist[nxtNode] > d + a[i])) {
				dist[nxtNode] = d + a[i];
				pq.push({ nxtNode, dist[nxtNode] });
			}
		}
	}

	int32_t q;
	std::cin >> q;

	for(int32_t i = 0; i < q; i++) {
		int32_t x;
		std::cin >> x;

		if(dist[x % a[0]] <= x) {
			std::cout << "TAK" << '\n';
		}
		else {
			std::cout << "NIE" << '\n';
		}
	}
}

