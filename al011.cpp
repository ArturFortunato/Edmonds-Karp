#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <vector>

using namespace std;

//Structs

class Node {
	public:
		//Bottom, Left, Up, Right
		Node **connections;
		int id;
		int *weight;
		char type;
		int lp; 
		int cp;
};

// Global variables

int m, n;
vector<vector<Node *> *> *nodes;
bool *inStack;

void printNode(int linha, int coluna) {
	Node *node = nodes -> at(linha) -> at(coluna);
	printf("\nNode %d %d\n", linha, coluna);
	printf("lp: %d\ncp:%d\n", node -> lp, node -> cp);
	for(int i = 0; i < 4; i++)
		if(node -> connections[i] == NULL) {
			if(i == 0)
				printf("Para baixo e nulo %d\n", i);
			else if (i == 1)	
				printf("Para a esquerda e nulo %d\n", i);
			else if (i == 2)
				printf("Para cima e nulo %d\n", i);
			else
				printf("Para a direita e nulo %d\n", i);
			}
	printf("\n-------\n");
}

void dfs(int linha, int coluna) {
	int i;
	printf("Estou em %d %d\n", linha, coluna);
	inStack[linha * n + coluna] = true;
	Node *node = nodes -> at(linha) -> at(coluna);
	for(i = 0; i < 4; i++) {
		if(node -> connections[i] != NULL) {
			if(i == 0 && !inStack[(linha + 1) * n + coluna])
				dfs(linha + 1, coluna);
			else if (i == 1 && !inStack[linha * n + coluna - 1])
				dfs(linha, coluna - 1);
			else if(i == 2 && !inStack[(linha - 1) * n + coluna])
				dfs(linha - 1, coluna);
			else if (i == 3 && !inStack[linha * n + coluna + 1])
				dfs(linha, coluna + 1);
		}
	}
	printf("Acabei %d %d\n", linha, coluna);
	inStack[linha * n + coluna] = false;
}

int main() {
	int i, j, temp;
	scanf("%d %d\n", &m, &n);
	if (m < 1 || n < 1) {
		printf("m and n must be greater than 1\n");
		exit(0);
	}

	nodes = new vector<vector<Node *> *>();
	inStack = new bool[m * n]();

	vector<Node *> *l;
	Node *node;

	for(i = 0; i < m; i++) {
		l = new vector<Node *>();
		nodes -> push_back(l);
	}

	for (i = 0; i < m; i++) {
		for (j = 0; j < n; j++) {
			node = new Node();
			node -> connections = new Node *[4];
			node -> weight = new int[4]();
			node -> id = 
			nodes -> at(i) -> push_back(node);
		}
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
			nodes -> at(i) -> at(j) -> weight[3] = temp;
			nodes -> at(i) -> at(j) -> connections[3] = nodes -> at(i) -> at(j + 1);
			nodes -> at(i) -> at(j + 1) -> weight[1] = temp;
			nodes -> at(i) -> at(j + 1) -> connections[1] = nodes -> at(i) -> at(j);
		}

	for (i = 0; i < m - 1; i++)
		for (j = 0; j < n; j++) {
			scanf("%d", &temp);	
			nodes -> at(i) -> at(j) -> weight[0] = temp;
			nodes -> at(i) -> at(j) -> connections[0] = nodes -> at(i + 1) -> at(j);
			nodes -> at(i + 1) -> at(j) -> weight[2] = temp;
			nodes -> at(i + 1) -> at(j) -> connections[2] = nodes -> at(i) -> at(j); 
		}
	dfs(0, 0);
	/*for (i = 0; i< m; i++)
		for (j = 0; j < n; j++)
			printNode(i, j);*/
	return 0;
}