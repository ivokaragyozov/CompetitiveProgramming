/**
Solution:
	Idea: If we build a graph where we have an edge between two stocks which can't be in the same row
then if that graph is not bipartite we don't have solution so the answer is 0 and when it is bipartite
then the answer is 2 rised to power the number of components. We can do a sweep line + segment tree to 
compute the target components for each node and then see if the we can simulate the ordering with the 
resulted targets (this is equal to checking if the graph is bipartite).
*/
#include <bits/stdc++.h>

const int32_t MAX_N = 1e6;
const int32_t INF = 2e9;
const int32_t MOD = 1e9 + 7;

class SegmentTree {
private:
	bool isMax;
	int32_t treeSize;
	int32_t data[8 * MAX_N + 5];

	void Update(int32_t node, int32_t low, int32_t high, int32_t qInd, int32_t qVal) {
		if(low > qInd || high < qInd) {
			return;
		}
		else if(low == qInd && high == qInd) {
			data[node] = qVal;
			return;
		}

		int32_t mid = (low + high) / 2;
		Update(2 * node, low, mid, qInd, qVal);
		Update(2 * node + 1, mid + 1, high, qInd, qVal);

		if(isMax) {
			data[node] = std::max(data[2 * node], data[2 * node + 1]);
		}
		else {
			data[node] = std::min(data[2 * node], data[2 * node + 1]);
		}
	}

	int32_t Query(int32_t node, int32_t low, int32_t high, int32_t qLow, int32_t qHigh) {
		if(low > qHigh || high < qLow) {
			if(isMax) {
				return -1;
			}
			else {
				return INF;
			}
		}
		else if(low >= qLow && high <= qHigh) {
			return data[node];
		}

		int32_t mid = (low + high) / 2;
		
		if(isMax) {
			return std::max(Query(2 * node, low, mid, qLow, qHigh), Query(2 * node + 1, mid + 1, high, qLow, qHigh));
		}
		else {
			return std::min(Query(2 * node, low, mid, qLow, qHigh), Query(2 * node + 1, mid + 1, high, qLow, qHigh));
		}
	}

public:
	void Init(int32_t _treeSize, bool _isMax) {
		treeSize = _treeSize;
		isMax = _isMax;
	}

	void Update(int32_t ind, int32_t val) {
		Update(1, 1, treeSize, ind, val);
	}

	int32_t Query(int32_t low, int32_t high) {
		return Query(1, 1, treeSize, low, high);
	}
};

int32_t comps[MAX_N + 5], indOf[2 * MAX_N + 5];
std::pair< int32_t, int32_t > a[MAX_N + 5];
SegmentTree segTreeStarts, segTreeEnds;

void Dfs(int32_t ind, int32_t currComp) {
	comps[ind] = currComp;

	segTreeStarts.Update(a[ind].first, -1);
	segTreeEnds.Update(a[ind].second, INF);
	
	while(1) {
		int32_t aux = segTreeStarts.Query(a[ind].first + 1, a[ind].second - 1);
		
		if(aux > a[ind].second) {
			Dfs(indOf[aux], (currComp ^ 1));
		}
		else {
			break;
		}
	}

	while(1) {
		int32_t aux = segTreeEnds.Query(a[ind].first + 1, a[ind].second - 1);
	
		if(aux < a[ind].first) {
			Dfs(indOf[aux], (currComp ^ 1));
		}
		else {
			break;
		}
	}
}

bool Check(int32_t n) {
	std::vector< int32_t > side1, side2;
	for(int32_t i = 0; i < n; i++) {
		if(comps[i] == 0) {
			side1.push_back(i);
		}
		else {
			side2.push_back(i);
		}
	}

	std::stack< int32_t > st;
	for(auto &x : side1) {
		while(!st.empty() && st.top() < a[x].first) {
			st.pop();
		}

		if(!st.empty() && st.top() < a[x].second) {
			return false;
		}
		else {
			st.push(a[x].second);
		}
	}

	while(!st.empty()) {
		st.pop();
	}

	for(auto &x : side2) {
		while(!st.empty() && st.top() < a[x].first) {
			st.pop();
		}

		if(!st.empty() && st.top() < a[x].second) {
			return false;
		}
		else {
			st.push(a[x].second);
		}
	}	

	return true;
}

int main() {
	std::ios_base::sync_with_stdio(false);
	std::cin.tie(nullptr);

	int32_t n;
	std::cin >> n;

	segTreeStarts.Init(2 * n, 1);
	segTreeEnds.Init(2 * n, 0);

	for(int32_t i = 0; i < n; i++) {
		std::cin >> a[i].first >> a[i].second;
	
		segTreeStarts.Update(a[i].first, a[i].second);
		segTreeStarts.Update(a[i].second, -1);

		segTreeEnds.Update(a[i].second, a[i].first);
		segTreeEnds.Update(a[i].first, INF);
	}

	std::sort(a, a + n);
	for(int32_t i = 0; i < n; i++) {
		indOf[a[i].first] = i;
		indOf[a[i].second] = i;
	}

	int32_t cntComps = 0;
	memset(comps, -1, sizeof(comps));
	for(int32_t i = 0; i < n; i++) {
		if(comps[i] == -1) {
			cntComps++;
			Dfs(i, 0);
		}
	}

	if(Check(n)) {
		int32_t ans = 1;
		for(int32_t i = 0; i < cntComps; i++) {
			ans = ((int64_t) 2 * ans) % MOD;
		}

		std::cout << ans << '\n';
	}
	else {
		std::cout << 0 << '\n';
	}
}

