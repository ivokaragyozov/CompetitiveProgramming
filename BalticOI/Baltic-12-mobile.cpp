/**
Solution:
	Idea: We can binary search over the target distance and the lineary check if the whole highway 
is covered by the circles which are formed from all the points and current target distance.
*/

#include <bits/stdc++.h>
 
const int32_t MAX_N = 1e6;
const double EPS = 1e-6;
 
int32_t n, l;
std::pair< int32_t, int32_t > points[MAX_N + 5];
 
bool Check(double r) {
	double curr = 0;
	for(int32_t i = 0; i < n; i++) {
		if(r * r < (int64_t) points[i].second * points[i].second) {
			continue;
		}
 
		double x1 = points[i].first - sqrt(r * r - (int64_t) points[i].second * points[i].second);
		double x2 = points[i].first + sqrt(r * r - (int64_t) points[i].second * points[i].second);
 
		if(!(x1 > curr)) {
			curr = std::max(curr, x2);
		}
	}
 
	if(!(curr < l)) {
		return true;
	}
	else {
		return false;
	}
}
 
int main() {
	std::ios_base::sync_with_stdio(false);
	std::cin.tie(nullptr);
 
	std::cin >> n >> l;
 
	for(int32_t i = 0; i < n; i++) {
		std::cin >> points[i].first >> points[i].second;
	}
 
	int64_t low = 1, high = 1e14;
	double ans;
	while(low <= high) {
		int64_t mid = (low + high) / 2;
 
		if(Check(mid / 10000.0)) {
			ans = mid / 10000.0;
			high = mid - 1;
		}
		else {
			low = mid + 1;
		}
	}
 
	std::cout << std::fixed << std::setprecision(7);
	std::cout << ans << '\n';
}
