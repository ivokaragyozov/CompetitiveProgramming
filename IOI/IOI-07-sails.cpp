/*
Solution:
	Idea: Greedy algorithm sort - masts by height and start and for every mast fill the 
Kth rows with smallest number of sails on them
	
	1) Sort masts by height
	2) Keep in index tree the sorted numbers of sails on each row (keep the differences between each two
consecutive elements)
	3) Be careful when updating some rows which have same number of sails - first update the rightmost in the 
Index tree
*/

#include <bits/stdc++.h>

const int32_t MAX_N = 1e5;
const int32_t MAX_H = 1e5;

class IndexTree {
private:
	int32_t data[MAX_H + 5];

public:
	void Add(int32_t ind, int32_t val) {
		while(ind <= MAX_H) {
			data[ind] += val;
			ind += (ind & (-ind));
		}
	}

	int32_t Query(int32_t ind) {
		int32_t ans = 0;
		
		while(ind > 0) {
			ans += data[ind];
			ind -= (ind & (-ind));
		}

		return ans;
	}
};

IndexTree indTree;
std::pair< int32_t, int32_t > masts[MAX_N + 5];

int32_t BinarySearchLow(int32_t low, int32_t high, int32_t ind) {
	int32_t ans = low, sumToInd = indTree.Query(ind);

	while(low <= high) {
		int32_t mid = (low + high) / 2;

		if(indTree.Query(mid) != sumToInd) {
			ans = mid + 1;
			low = mid + 1;
		}
		else {
			high = mid - 1;
		}
	}

	return ans;
}

int32_t BinarySearchHigh(int32_t low, int32_t high, int32_t ind) {
	int32_t ans = high + 1, sumToInd = indTree.Query(ind);

	while(low <= high) {
		int32_t mid = (low + high) / 2;

		if(indTree.Query(mid) != sumToInd) {
			ans = mid;
			high = mid - 1;
		}
		else {
			low = mid + 1;
		}
	}

	return ans;
}

int main() {
	std::ios_base::sync_with_stdio(false);
	std::cin.tie(nullptr);

	int32_t n;
	std::cin >> n;

	for(int32_t i = 0; i < n; i++) {
		std::cin >> masts[i].first >> masts[i].second;
	}

	std::sort(masts, masts + n);
	
	for(int32_t i = 0; i < n; i++) {
		if(i != 0 && masts[i - 1].first != masts[i].first) {
			indTree.Add(masts[i - 1].first + 1, -indTree.Query(masts[i - 1].first));
		}

		int32_t ind = masts[i].first - masts[i].second + 1;

		if(ind != 1 && indTree.Query(ind) == indTree.Query(ind - 1)) {
			int32_t low = BinarySearchLow(1, ind - 1, ind);	
			int32_t high = BinarySearchHigh(ind + 1, masts[i].first, ind);
		
			int32_t aux = low + high - ind - 1;
			
			indTree.Add(low, 1);
			indTree.Add(aux + 1, -1);
			
			if(high != masts[i].first + 1) {
				indTree.Add(high, 1);
			}
		}
		else {
			indTree.Add(ind, 1);
		}
	}

	int64_t ans = 0;
	for(int32_t i = 1; i <= masts[n - 1].first; i++) {
		int32_t curr = indTree.Query(i);
		
		ans += (int64_t) curr * (curr - 1) / 2;
	}
	
	std::cout << ans << '\n';
}

