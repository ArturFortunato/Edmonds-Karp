#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <vector>

using namespace std;

//Structs

class Node {
	public:
		Node *left;
		Node *right;
		Node *top;
		Node *bottom;
		char type;
		int wLeft;
		int wRight;
		int wTop;
		int wBottom;
		int lp; 
		int cp;
};

// Global variables

int m, n;
vector<vector<Node *> *> *nodes;


int main() {
	int i, j, temp;
	scanf("%d %d\n", &m, &n);
	if (m < 1 || n < 1) {
		printf("m and n must be greater than 1\n");
		exit(0);
	}

	nodes = new vector<vector<Node *> *>();

	vector<Node *> *l = new vector<Node *>();
	Node *node;
	
	for(i = 0; i < m; i++)
		nodes -> push_back(l);

	for (i = 0; i < m; i++)
		for (j = 0; j < n; j++) {
			node = new Node();
			nodes -> at(i) -> push_back(node);
		}

	for (i = 0; i < m; i++)
		for (j = 0; j < n; j++)
			scanf("%d", &(nodes -> at(i) -> at(j) -> lp));

	for (i = 0; i < m; i++)
		for (j = 0; j < n; j++)
			scanf("%d", &(nodes -> at(i) -> at(j) -> cp));	

	for (i = 0; i < m; i++)
		for (j = 0; j < n - 1; j++) {
			scanf("%d", &temp);	
			nodes -> at(i) -> at(j) -> wRight = temp;
			nodes -> at(i) -> at(j + 1) -> wLeft = temp;
		}

	for (i = 0; i < m - 1; i++)
		for (j = 0; j < n; j++) {
			scanf("%d", &temp);	
			nodes -> at(i) -> at(j) -> wBottom = temp;
			nodes -> at(i + 1) -> at(j) -> wTop = temp;
		}
	printf("%d", nodes -> at(1) -> at(0) -> wRight);
	return 0;
}