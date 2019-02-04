/**
Solution:
	Idea: We can use matrix exponentials to compute the number of path with size less or equal to t from 1 to 1. Now we only
have to deal with the ingridients. Here comes the inclusion-exclusion method. First we add all paths then subtract those not
having one ingridient then add those which don't have two ingridients and so on.
*/

#include <bits/stdc++.h>

const int32_t MAX_N = 25;
const int32_t MAX_M = 500;
const int32_t MOD = 5557;
const std::string INGR = "BJMP";

class Matrix {
public:
	int32_t size, vals[2 * MAX_N + 5][2 * MAX_N + 5];

	void Init(int32_t pSize) {
		size = pSize;
	}

	Matrix operator* (const Matrix &other) const {
		Matrix newMat;
		newMat.Init(size);

		for(int32_t i = 0; i < size; i++) {
			for(int32_t j = 0; j < size; j++) {
				int32_t sum = 0;
				for(int32_t k = 0; k < size; k++) {
					sum = (sum + vals[i][k] * other.vals[k][j]) % MOD;
				}
				newMat.vals[i][j] = sum;
			}
		}

		return newMat;
	}

	static Matrix RaiseToPower(const Matrix &m, int32_t pw) {
		if(pw == 1) {
			return m; 
		}
		else if(pw % 2 == 0) {
			Matrix aux = RaiseToPower(m, pw / 2);
			return aux * aux;
		}
		else {
			Matrix aux = RaiseToPower(m, pw - 1);
			return m * aux;
		}
	}
};

int32_t n, m, x[MAX_M + 5], y[MAX_M + 5];
std::string s[MAX_M + 5];
Matrix mat;

void Update(int32_t mask) {
	for(int32_t i = 0; i < m; i++) {
		bool isPos = true;
		for(int32_t j = 0; j < 4; j++) {
			if((mask & (1 << j)) == 0) {
				if(s[i].find(INGR[j]) != std::string::npos) {
					isPos = false;
				}
			}
		}

		if(isPos) {
			mat.vals[2 * x[i] - 1][2 * y[i] - 2] = 1;
		}
		else {
			mat.vals[2 * x[i] - 1][2 * y[i] - 2] = 0;
		}
	}
}

int main() {
	std::ios_base::sync_with_stdio(false);
	std::cin.tie(nullptr);

	std::cin >> n >> m;

	mat.Init(2 * n + 1);
	for(int32_t i = 0; i < 2 * n; i += 2) {
		mat.vals[i][i + 1] = 1;
	}

	for(int32_t i = 0; i < m; i++) {
		std::cin >> x[i] >> y[i] >> s[i];
	
		mat.vals[2 * x[i] - 1][2 * y[i] - 1] = 1;
		mat.vals[2 * x[i] - 1][2 * y[i] - 2] = 1;
	}
	
	mat.vals[1][2 * n] = 1;
	mat.vals[2 * n][2 * n] = 1;

	int32_t t;
	std::cin >> t;

	Matrix aux = Matrix::RaiseToPower(mat, t + 1);
	int32_t ans = aux.vals[1][2 * n];
	
	int32_t currMask = (1 << 4) - 1;
	for(int32_t mask = 0; mask < (1 << 4) - 1; mask++) {
		Update(mask);
		
		aux = Matrix::RaiseToPower(mat, t + 1);
		if(__builtin_popcount(mask) % 2 == 0) {
			ans = (ans + aux.vals[1][2 * n]) % MOD;
		}
		else {
			ans -= aux.vals[1][2 * n];
			if(ans < 0) {
				ans += MOD;
			}
		}

		currMask = mask;
	}

	std::cout << ans << '\n';
}

