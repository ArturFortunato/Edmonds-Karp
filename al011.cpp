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
		//Bottom, Left, Up, Right, t, s
		Node **connections;
		int id;
		int *weight;
		int *currFlow;

		//0 -> background 1 -> foreground
		int type;
		int lp; 
		int cp;
		int parent;
		Node* daddy;
		int color;
};

// Global variables

int m = 0, n = 0, flowTotal = 0;
vector<vector<Node *> *> *nodes;
vector<Node *> *paths;
vector<pair<int,int> > *cut;
queue<Node *> *Q = new queue<Node *>();
Node *s = new Node();
Node *t = new Node();

void printNode(int linha, int coluna) {
	Node *node = nodes -> at(linha) -> at(coluna);
	printf("\n(%d, %d): Parent %d", linha, coluna, node -> parent);
	printf("\n-------\n");
}

void createPath(){
	int flow = -1;
	Node *n = t;
	while(n != s) {
	    if(flow == -1)
			flow = n -> daddy -> weight[4] - n -> daddy -> currFlow[4];
		else {
			flow = min(n -> weight[n -> parent] - n -> currFlow[n -> parent], flow);
		}
		paths -> insert(paths -> begin(), n);
		n = n -> daddy;
	}
	paths -> insert(paths -> begin(), s);

	while(paths -> size() != 1) {
		n = paths -> at(paths -> size() - 1);
		paths -> pop_back();
		if (n != t) {
			n -> currFlow[n -> parent] += flow;
			if (n -> parent == 0 || n -> parent == 1)
				n -> daddy -> currFlow[n -> parent + 2] += flow;
			else if  (n -> parent == 2 || n -> parent == 3)
				n -> daddy -> currFlow[n -> parent - 2] += flow;
			else 
				s -> currFlow[n -> id] += flow;
			if(n -> currFlow[n -> parent] == n -> weight[n -> parent])
				cut -> push_back(make_pair(n -> id, n -> parent));
		}
		else
			paths -> at(paths -> size() - 1) -> currFlow[4] += flow;
	}
	flowTotal += flow;
	printf("%d %d %d\n", s -> currFlow[0], nodes -> at(0) -> at(0) -> currFlow[5], nodes -> at(0) -> at(0) -> currFlow[4]);
}


int getParentId(Node *n) {
	Node *parent = n -> daddy;
	int i;
	for(i = 0; i < 4; i++) {
		if(n -> connections[i] == parent)
			return i;
	}
	return 5;
}

void bfs(){
	Node *u;
	Node *node; 
	int i, size;
	while(!Q -> empty()){
		u = Q -> front();
		Q -> pop();
		if (u == s)
			size = m * n;
		else
			 size = 5;
		for(i = 0; i < size; i++) {
			if((node = u -> connections[i]) != NULL && node -> color == 0 && node != t) {
				node -> daddy = u;
				node -> parent = getParentId(node);
				node -> color = 1;
			}
			if(node == t){
			    node -> daddy = u;
				createPath();
			}
		}
		u -> color = 2;
	}
}

int main() {
	int i = 0, j = 0, k = 0, temp = 0;
	scanf("%d %d\n", &m, &n);

	if (m < 1 || n < 1) {
		printf("m and n must be greater than 1\n");
		exit(0);
	}

	nodes = new vector<vector<Node *> *>();
	paths = new vector<Node *>();
	cut = new vector<pair<int,int> >();

	Node *node;

	s -> lp = 0;
	s -> cp = 0;
	s -> weight = new int[m * n];
	s -> currFlow = new int[m * n];
	s -> id = -1;
	s -> color = 0;
	s -> connections = new Node *[m * n];
	s -> type = -1;
	Q -> push(s);

	for (i = 0; i < m * n; i++)
		s -> currFlow[i] = 0;

	t -> lp = 0;
	t -> cp = 0;
	t -> id = -2;
	t -> color = 0;
	t -> type = -1;

	for(i = 0; i < m; i++) 
		nodes -> push_back(new vector<Node *>());

	for (i = 0; i < m; i++) {
		for (j = 0; j < n; j++) {
			node = new Node();
			node -> connections = new Node *[6];
			node -> currFlow = new int[6];
			for(k = 0; k < 6; k++) {
				node -> connections[k] = NULL;
				node -> currFlow[k] = 0;
			}
			node -> weight = new int[6]();
			node -> connections[4] = t;
			node -> connections[5] = s;
			node -> id = i * n + j;
			node -> color = 0;
			node -> daddy = NULL;
			node -> parent = -1;
			nodes -> at(i) -> push_back(node);
			s -> connections[node -> id] = node;
			Q -> push(node);
		}
	}

	for (i = 0; i < m; i++)
		for (j = 0; j < n; j++) {
			scanf("%d", &temp);
			nodes -> at(i) -> at(j) -> lp = temp;
			nodes -> at(i) -> at(j) -> weight[5] = temp;
			s -> weight[i * n + j] = temp;
		}

	for (i = 0; i < m; i++)
		for (j = 0; j < n; j++) {
			scanf("%d", &temp);	
			nodes -> at(i) -> at(j) -> cp = temp;
			nodes -> at(i) -> at(j) -> weight[4] = temp;
			s -> weight[i * n + j] = temp;
		}

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
	bfs();
	/*for (i = 0; i< m; i++)
		for (j = 0; j < n; j++)
			printNode(i, j);*/
	return 0;
}