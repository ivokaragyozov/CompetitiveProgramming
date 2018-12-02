/**
Solution:
	Idea: Let f(n) be the answer for n no matter what are the prime numbers. f(n) is increasing so the greedy
solution to subtract the biggest possible number from n until n > 0 will be correct. So we can calculate for 
each i smaller than MAX_N the maximum number which can be subtracted then we can do a simple dp to calculate
the number of steps needed to make n equal to 0.
*/
#include <bits/stdc++.h>

const int32_t MAX_N = 1e7 + 1e6;

int32_t dp[MAX_N + 5];

int main() {
	std::ios_base::sync_with_stdio(false);
	std::cin.tie(nullptr);

	int32_t m, q;
	std::cin >> m >> q;

	int64_t lcm = 1;
	for(int32_t i = 0; i < m; i++) {
		int32_t p;
		std::cin >> p;

		for(int32_t j = p - 1; j <= MAX_N; j += p) {
			dp[j] = p - 1;
		}

		if(lcm < MAX_N) {
			lcm *= (int64_t) p;
		}
	}

	for(int32_t i = MAX_N - 1; i >= 1; i--) {
		dp[i] = std::max(dp[i], dp[i + 1] - 1);
	}

	dp[0] = 0;
	for(int32_t i = 1; i <= MAX_N; i++) {
		dp[i] = dp[i - dp[i]] + 1;
	}

	for(int32_t i = 0; i < q; i++) {
		int32_t n;
		std::cin >> n;

		if(n >= lcm) {
			std::cout << "oo" << '\n';
		}
		else {
			std::cout << dp[n] << '\n';
		}
	}
}
