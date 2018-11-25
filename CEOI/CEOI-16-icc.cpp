/*
Solution:
	1) find the two connected components which are connected by the new edge
		1.1) let the current number of connected components be M then you will make logM querries - one for
each bit of their binary representation. This way you will know which bits from the numbers of the two components are different
		1.2) do a binary search in one of the above queries with answer 1 to find one of the components and find the other by the xor-ing
the first component with the mask we found in the first step
	2) do a binary search to find the exact nodes in the two components
*/

#include <bits/stdc++.h>
#include <icc.h>

#define Run run
#define Query query
#define SetRoad setRoad
#define int32_t int

const int32_t MAX_N = 100;

int32_t parent[MAX_N + 5], size[MAX_N + 5];

int32_t FindParent(int32_t x) {
	if(parent[x] == x) {
		return x;
	}
	else {
		parent[x] = FindParent(parent[x]);
		return parent[x];
	}
}

void Unite(int32_t x, int32_t y) {
	int32_t parX = FindParent(x);
	int32_t parY = FindParent(y);

	if(parX == parY) {
		return;
	}

	if(size[parX] <= size[parY]) {
		parent[parX] = parY;
		size[parY] += size[parX];
	}
	else {
		parent[parY] = parX;
		size[parX] += size[parY];
	}
}

void Run(int32_t n) {
	for(int32_t i = 1; i <= n; i++) {
		parent[i] = i;
		size[i] = 1;
	}
	
	bool isBitDiff[10];
	int32_t sizeA, sizeB, a[MAX_N + 5], b[MAX_N + 5];
	int32_t compInd[MAX_N + 5];

	for(int32_t q = 0; q < n - 1; q++) {	
		int32_t cntComps = 0;
		for(int32_t i = 1; i <= n; i++) {
			if(parent[i] == i) {
				compInd[i - 1] = cntComps;
				cntComps++;
			}
		}

		std::vector< int32_t > comps[MAX_N + 5];
		for(int32_t i = 0; i < n; i++) {
			comps[compInd[FindParent(i + 1) - 1]].push_back(i);
		}

		memset(isBitDiff, 0, sizeof(isBitDiff));
		int32_t comp1 = -1;
		for(int32_t j = 0; (1 << j) < cntComps; j++) {
			sizeA = 0;
			sizeB = 0;
			
			std::vector< int32_t > aComps;
			for(int32_t p = 0; p < cntComps; p++) {
				if((p & (1 << j)) == 0) {
					aComps.push_back(p);
					for(auto &x : comps[p]) {
						a[sizeA] = x + 1;
						sizeA++;
					}
				}
				else {
					for(auto &x : comps[p]) {
						b[sizeB] = x + 1;
						sizeB++;
					}
				}
			}

			if(Query(sizeA, sizeB, a, b) == 1) {
				isBitDiff[j] = true;
				
				if(comp1 == -1) {
					int32_t low = 0, high = aComps.size() - 1;
					while(low <= high) {
						if(low == high) {
							comp1 = aComps[low];
							break;
						}
						
						int32_t mid = (low + high) / 2;
							
						sizeA = 0;
						for(int32_t i = low; i <= mid; i++) {
							for(auto &x : comps[aComps[i]]) {
								a[sizeA] = x + 1;
								sizeA++;
							}
						}

						if(Query(sizeA, sizeB, a, b) == 1) {
							high = mid;
						}
						else {
							low = mid + 1;
						}
					}
				}
			}
		}

		int32_t comp2 = comp1;
		for(int32_t i = 0; (1 << i) < cntComps; i++) {
			if(isBitDiff[i]) {
				comp2 ^= (1 << i);
			}
		}

		sizeB = 0;
		for(auto &x : comps[comp2]) {
			b[sizeB] = x + 1;
			sizeB++;
		}
	
		int32_t u = -1;
		int32_t low = 0, high = comps[comp1].size() - 1;
		while(low <= high) {
			if(low == high) {
				u = comps[comp1][low];
				break;
			}

			int32_t mid = (low + high) / 2;

			sizeA = 0;
			for(int32_t i = low; i <= mid; i++) {
				a[sizeA] = comps[comp1][i] + 1;
				sizeA++;
			}

			if(Query(sizeA, sizeB, a, b) == 1) {
				high = mid;
			}
			else {
				low = mid + 1;
			}
		}

		a[0] = u + 1;
		sizeA = 1;
		
		int32_t v = -1;
		low = 0;
		high = comps[comp2].size() - 1;
		while(low <= high) {
			if(low == high) {
				v = comps[comp2][low];
				break;
			}

			int32_t mid = (low + high) / 2;		
		
			sizeB = 0;
			for(int32_t i = low; i <= mid; i++) {
				b[sizeB] = comps[comp2][i] + 1;
				sizeB++;
			}

			if(Query(sizeA, sizeB, a, b) == 1) {
				high = mid;
			}
			else {
				low = mid + 1;
			}
		}
		
		Unite(u + 1, v + 1);
		SetRoad(u + 1, v + 1);
	}
}

#undef Run
#undef Query
#undef int32_t
