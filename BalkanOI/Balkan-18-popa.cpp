/**
Solution:
	Idea: If we start building the tree from 0 to n - 1 we see that when
we add a node to the tree we always will insert it on the rightmost side
of the tree. We can loop from the rightmost node to the top and try to 
insert the new node in the highest possible place.
*/

#include <bits/stdc++.h>
#include "popa.h"

#define int32_t int
#define int64_t long long
#define Solve solve
#define Query query

const int32_t MAX_N = 1000;

int32_t parent[MAX_N + 5];

int32_t Solve(int32_t n, int32_t *left, int32_t *right) {
	for(int32_t i = 0; i < n; i++) {
		left[i] = -1;
		right[i] = -1;
		parent[i] = -1;
	}

	int32_t root = -1;
	for(int32_t i = 0; i < n; i++) {
		int32_t curr = i - 1;
		while(curr != -1 && Query(i, i, curr, i)) {
			right[curr] = left[i];
			parent[left[i]] = curr;
			left[i] = curr;
			int32_t nxt = parent[curr];
			parent[curr] = i;
			curr = nxt;
		}

		if(curr == -1) {
			left[i] = root;
			if(root != -1) {
				parent[root] = i;
			}
			root = i;
		}
		else {
			right[curr] = i;
			parent[i] = curr;
		}
	}

	return root;
}
