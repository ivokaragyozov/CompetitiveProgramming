/**
Solution:
	Idea: We will be using union-find data structure to execute each of the queries. In each component
we need to say fastly if the set of numbers of elements in it equals the set of elements equal to the sorted
array coresponding to indexes in the component. We can compare two sets by using hashing.  If in every component the 
two sets are equal then the answer to the 3rd query is "DA" otherwise it is "NE". For the fourth query we need to 
find two nodes for which P[A] - Q[A] = -(P[B] - Q[B]) where P[A], P[B] and Q[A], Q[B] are coresponding to the
sets described above. We can easily maintain a map for each difference which will let us know the number of pairs
satisfying the 4th query.
*/

#include <bits/stdc++.h>

const int64_t BASE = 1234577;

const int64_t MAX_N = 1e6;
const int64_t MAX_P = 1e6;

int64_t p[MAX_N + 5], q[MAX_N + 5];
int64_t basePowers[MAX_P + 5];
std::unordered_map< int64_t, int64_t > totalWithDiff;
int64_t totalPairs;

void Add(int64_t diff, int64_t cnt) {
	if(diff != 0) {
		totalPairs += (int64_t) cnt * totalWithDiff[-diff];
	}
	
	totalWithDiff[diff] += cnt;
}

void Remove(int64_t diff, int64_t cnt) {
	if(diff != 0) {
		totalPairs -= (int64_t) cnt * totalWithDiff[-diff];
	}

	totalWithDiff[diff] -= cnt;
}

class UnionFind {
private:
	int64_t parents[MAX_N + 5], sizes[MAX_N + 5], sumsP[MAX_N + 5], sumsQ[MAX_N + 5];

public:
	void Init(int64_t pCntNodes) {
		for(int64_t i = 1; i <= pCntNodes; i++) {
			sumsP[i] = basePowers[p[i] - 1];
			sumsQ[i] = basePowers[q[i] - 1];
			parents[i] = i;
			sizes[i] = 1;
			
			int64_t diff = sumsP[i] - sumsQ[i];
			Add(diff, 1);
		}
	}

	int64_t Find(int64_t x) {
		if(x == parents[x]) {
			return x;
		}
		else {
			parents[x] = Find(parents[x]);
			return parents[x];
		}
	}

	void Swap(int64_t x, int64_t y) {
		int64_t parX = Find(x);
		int64_t parY = Find(y);

		if(parX == parY) {
			std::swap(p[x], p[y]);
			return;
		}

		int64_t diffX = sumsP[parX] - sumsQ[parX];
		Remove(diffX, sizes[parX]);
		
		int64_t diffY = sumsP[parY] - sumsQ[parY];
		Remove(diffY, sizes[parY]);

		sumsP[parX] -= basePowers[p[x] - 1];
		sumsP[parX] = sumsP[parX] + basePowers[p[y] - 1];

		sumsP[parY] -= basePowers[p[y] - 1];
		sumsP[parY] = sumsP[parY] + basePowers[p[x] - 1];
		
		std::swap(p[x], p[y]);

		diffX = sumsP[parX] - sumsQ[parX];
		Add(diffX, sizes[parX]);
		
		diffY = sumsP[parY] - sumsQ[parY];
		Add(diffY, sizes[parY]);
	}

	void Union(int64_t x, int64_t y) {
		int64_t parX = Find(x);
		int64_t parY = Find(y);

		if(parX == parY) {
			return;
		}

		int64_t diffX = sumsP[parX] - sumsQ[parX];
		Remove(diffX, sizes[parX]);
		
		int64_t diffY = sumsP[parY] - sumsQ[parY];
		Remove(diffY, sizes[parY]);

		if(sizes[parX] <= sizes[parY]) {
			parents[parX] = parY;
			sizes[parY] += sizes[parX];
			sumsP[parY] = sumsP[parY] + sumsP[parX];
			sumsQ[parY] = sumsQ[parY] + sumsQ[parX];
			
			diffY = sumsP[parY] - sumsQ[parY];
			Add(diffY, sizes[parY]);
		}
		else {
			parents[parY] = parX;
			sizes[parX] += sizes[parY];
			sumsP[parX] = sumsP[parX] + sumsP[parY];
			sumsQ[parX] = sumsQ[parX] + sumsQ[parY];
		
			diffX = sumsP[parX] - sumsQ[parX];
			Add(diffX, sizes[parX]);
		}
	}
};

UnionFind dsu;

int main() {
	std::ios_base::sync_with_stdio(false);
	std::cin.tie(nullptr);

	int64_t n, m;
	std::cin >> n >> m;

	for(int64_t i = 1; i <= n; i++) {
		std::cin >> p[i];
		q[i] = p[i];
	}

	std::sort(q + 1, q + 1 + n);

	basePowers[0] = 1;
	for(int64_t i = 1; i < q[n]; i++) {
		basePowers[i] = basePowers[i - 1] * BASE;
	}
	
	dsu.Init(n);

	for(int64_t i = 0; i < m; i++) {
		int64_t t;
		std::cin >> t;

		if(t == 1) {
			int64_t a, b;
			std::cin >> a >> b;
		
			dsu.Swap(a, b);
		}
		else if(t == 2) {
			int64_t a, b;
			std::cin >> a >> b;

			dsu.Union(a, b);
		}
		else if(t == 3) {
			std::cout << (totalWithDiff[0] == n ? "DA" : "NE") << '\n';
		}
		else {
			std::cout << totalPairs << '\n';
		}
	}
}
