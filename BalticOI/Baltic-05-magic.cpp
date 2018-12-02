/**
Solution:
	Idea: If it is possible then the answer will have the form 1, 1, 1, 1, ..., the remaining number of )
so we can check if we exchange ] with the above number of ) and check if it is a regular parenthesis sequence.
*/
#include <bits/stdc++.h>

const int32_t MAX_N = 1e7;

char s[MAX_N + 5];
std::vector< int32_t > ans;

int main() {
	std::ios_base::sync_with_stdio(false);
	std::cin.tie(nullptr);
	
	int32_t n, m;
	std::cin >> n >> m;
	
	int32_t cnt = 0, indOfSpecial = 0;
	for(int32_t i = 0; i < n; i++) {
		std::cin >> s[i];

		if(s[i] == '(') {
			cnt++;
		}
		else if(s[i] == ')') {
			cnt--;
		}
		else {
			cnt--;

			ans.push_back(1);
			indOfSpecial++;
		}
	}
	
	if(cnt < 0) {
		std::cout << 0 << '\n';
		return 0;
	}

	if(m != 0) {
		ans[indOfSpecial - 1] += cnt;
	}

	cnt = 0;
	indOfSpecial = 0;
	for(int32_t i = 0; i < n; i++) {
		if(s[i] == '(') {
			cnt++;
		}
		else if(s[i] == ')') {
			cnt--;
		}
		else {
			cnt -= ans[indOfSpecial];
			indOfSpecial++;
		}

		if(cnt < 0) {
			std::cout << 0 << '\n';
			return 0;
		}
	}

	std::cout << 1 << '\n';
	for(auto &x : ans) {
		std::cout << x << '\n';
	}
}

