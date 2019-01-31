/**
Solution:
		Idea: If we suppose that the beetle will drink exactly k drops then the score is (m - t1) + (m - t2) + ... + (m - tk)  or k * m - (t1 + t2 + t3 + ... + tk) so our goal is to minimize the sum of the ts (t1 + t2 + ... + tk). We can have simple dp  
which can is of the form dp[low][high][side][k] where the interval of drops [low, high] is already drunk side is either 1 or 0 depending in which index is the beetle at the moment (0 -> low, 1 -> high) and k is the number of drops remaining to be drunk. 
This dp will say what is the minimum sum of times to drink k more drops considering you have drunk the interval [low, high] and
your current positions is side. For simplicity we have split the dp into two - l and r and we have removed the side state this way. The solution has complexity of O(N ^ 3) but you have to code it with O(N ^ 2) memory otherwise you won't get full score.
*/
#include <bits/stdc++.h>

const int32_t MAX_N = 300;

int32_t n, m, x[MAX_N + 5];
int32_t l[MAX_N + 5][MAX_N + 5][2], r[MAX_N + 5][MAX_N + 5][2];

int main() {
	std::ios_base::sync_with_stdio(false);
	std::cin.tie(nullptr);
	
	std::cin >> n >> m;	

	for(int32_t i = 0; i < n; i++) {
		std::cin >> x[i];
	}

	n++;
	std::sort(x, x + n);

	int32_t start;
	for(int32_t i = 0; i < n; i++) {
		if(x[i] == 0) {
			start = i;
		}
	}
	
	int32_t ans = 0;
	for(int32_t k = 1; k < n; k++) {
		int32_t curr = (k & 1), prev = ((k - 1) & 1);
		for(int32_t i = 0; i < n; i++) {
			for(int32_t j = i; j < n; j++) {
				int32_t rem = n - (j - i + 1);
				if(k > rem) {
					continue;
				}

				if(i == 0) {
					l[i][j][curr] = r[i][j + 1][prev] + k * (x[j + 1] - x[i]);
					r[i][j][curr] = r[i][j + 1][prev] + k * (x[j + 1] - x[j]);
				}
				else if(j == n - 1) {
					l[i][j][curr] = l[i - 1][j][prev] + k * (x[i] - x[i - 1]);
					r[i][j][curr] = l[i - 1][j][prev] + k * (x[j] - x[i - 1]);
				}
				else {
					int32_t aux1 = r[i][j + 1][prev] + k * (x[j + 1] - x[i]);
					int32_t aux2 = l[i - 1][j][prev] + k * (x[i] - x[i - 1]);
					l[i][j][curr] = std::min(aux1, aux2);

					aux1 = r[i][j + 1][prev] + k * (x[j + 1] - x[j]);
					aux2 = l[i - 1][j][prev] + k * (x[j] - x[i - 1]);
					r[i][j][curr] = std::min(aux1, aux2);
				}
			}
		}

		ans = std::max(ans, k * m - l[start][start][curr]);
	}

	std::cout << ans << '\n';
}
