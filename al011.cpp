#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <queue>
#include <unistd.h>

using namespace std;

//Structs

class Node {
	public:
		//Bottom, Left, Up, Right, s, t
		Node **connections;
		int id;
		int *weight;

		//0 -> background 1 -> foreground
		int type;
		int lp; 
		int cp;
		Node* parent;
		int color;
		int distance;
};

// Global variables

int m, n;
vector<vector<Node *> *> *nodes;
bool *inStack;
queue<Node *> Q;
Node *s = new Node();
Node *t = new Node();

void printNode(int linha, int coluna) {
	Node *node = nodes -> at(linha) -> at(coluna);
	printf("\nNode %d %d\n", linha, coluna);
	printf("lp: %d\ncp:%d\n", node -> lp, node -> cp);
	for(int i = 0; i < 6; i++)
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

void bfs(Node *current){
	Node *u;
	Node *n;
	if(current -> color != 2)
		current -> color = 1;
	while(!Q.empty()){
		u = Q.front();
		Q.pop();
		printf("%d------%d\n", u -> id, (int) Q.size());
		for(int i = 0; i < 4; i++){
			printf("%d\n", i);
			if((n = u -> connections[i]) != NULL && n -> color == 0){
				printf("if\n");
				n -> color = 1;
				n -> distance = u -> distance + 1;
				n -> parent = u;
			}
			else
				printf("else\n");
		}
	}
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

	s -> connections = new Node *[m * n];
	t -> connections = new Node *[m * n];

	for(i = 0; i < m; i++) {
		l = new vector<Node *>();
		nodes -> push_back(l);
	}

	for (i = 0; i < m; i++) {
		for (j = 0; j < n; j++) {
			node = new Node();
			node -> connections = new Node *[6];
			node -> weight = new int[6]();
			node -> connections[4] = s;
			node -> connections[5] = t;
			node -> id = i * n + j;
			node -> distance = 0;
			node -> color = 0;
			nodes -> at(i) -> push_back(node);
			Q.push(node);
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
	bfs(nodes -> at(0) -> at(0));
	/*for (i = 0; i< m; i++)
		for (j = 0; j < n; j++)
			printNode(i, j);*/
	return 0;
}