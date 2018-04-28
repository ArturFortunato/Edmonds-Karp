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
queue<Node *> *QAux = new queue<Node *>();
Node *s = new Node();
Node *t = new Node();

void printNode(int linha, int coluna) {
	Node *node = nodes -> at(linha) -> at(coluna);
	printf("\n(%d, %d): Parent %d", linha, coluna, node -> parent);
	printf("\n-------\n");
}

int getParentId(Node *n) {
	Node *parent = n -> daddy;
	int i;
	if (n == t)
		return parent -> id;
	for(i = 0; i < 6; i++) {
		if(n -> connections[i] == parent)
			break;
	}
	return i;
}

int getChildId(Node *parent, Node *child) {
	int i;
	if (parent == s)
		return child -> id;
	for (i = 0; i < 5; i++)
		if (parent -> connections[i] == child)
			return i;
	return -1;
}

int bfs() {
	int i, size, flow = 0, totalFlow = 0;
	Node *u;
	Node *node;
	int *flows = new int[m * n + 1];

	for (i = 0; i <= m * n; i++)
		flows[i] = -1;
	while(!Q -> empty()) {
		u = Q -> front();
		Q -> pop();
		if (u == s || u -> daddy != NULL) {
			if (u == s)
				size = m * n;
			else
				size = 5;
			for (i = 0; i < size; i++) {
				node = u -> connections[i];
				if(node != NULL && node -> color == 0 && u -> daddy != node) {
					node -> daddy = u;
					node -> parent = getParentId(node);
					if (node -> daddy == s)
						flows[node -> id] = s -> weight[node -> id] - s -> currFlow[node -> id];
					else
						flows[node -> id] = min(node -> daddy -> weight[i] - node -> daddy -> currFlow[i], flows[node -> daddy -> id]);
					if(node != t)
						node -> color = 1;
					else {
						flow = flows[node -> id];
						//printf("FLOW: %d\n", flow);
						totalFlow += flow;
						while (node != s) {
							//printf("No: %d Pai: %d parent: %d\n", node -> id, node -> daddy -> id, node -> parent);
							if (node -> weight[node -> parent] - node -> currFlow[node -> parent] == flow) {
								node -> connections[node -> parent] = NULL; 
								node -> daddy -> connections[getChildId(node->daddy, node)] = NULL;
								if(node -> id != m*n && node -> id != m*n+1 && node -> daddy -> id != m*n && node -> daddy -> id !=m*n+1)
									printf("Node 1: %d\tNode 2: %d\n", node -> id, node -> daddy -> id);
								//guardar aresta
							}
							node -> currFlow[node -> parent] += flow;
							node -> daddy -> currFlow[getChildId(node->daddy, node)] += flow;
							node = node -> daddy;
						}
						for(i = 0; i < m * n; i++) {
							nodes -> at(i / n) -> at(i % n) -> color = 0;
							nodes -> at(i / n) -> at(i % n) -> daddy = NULL;
							nodes -> at(i / n) -> at(i % n) -> parent = -1;	
						}
						for(i = 0; i <= m * n; i++) {
							flows[i] = -1;
						}
						break;
					}
				}
			}
		}
	}
	return totalFlow;
}


void edmundo() {
	int currentFlow = 0;
	while(true) {
		*Q = *QAux;
		currentFlow = bfs();
		//printf("currentFlow no edmundo: %d\n", flowTotal);
		if(currentFlow == 0) break;
		else flowTotal += currentFlow;
	}
}

int main() {
	int i = 0, j = 0, k = 0, temp = 0;
	scanf("%d %d\n", &m, &n);
	if (m < 1 || n < 1) {
		printf("m and n must be greater than 1\n");
		exit(0);
	}
	if(m == 1 && n == 1) {
		int x, y;
		scanf("%d\n%d", &x, &y);
		if (x <= y)
			printf("%d\n\nP \n", x);
		else
			printf("%d\n\nC \n", y);
		return 0;
	}

	nodes = new vector<vector<Node *> *>();
	paths = new vector<Node *>();
	cut = new vector<pair<int,int> >();

	Node *node;

	s -> lp = 0;
	s -> cp = 0;
	s -> weight = new int[m * n];
	s -> currFlow = new int[m * n];
	s -> id = m * n + 1;
	s -> color = 0;
	s -> connections = new Node *[m * n];
	s -> type = -1;
	QAux -> push(s);

	t -> lp = 0;
	t -> cp = 0;
	t -> id = m * n;
	t -> color = 0;
	t -> type = -1;
	t-> connections = new Node *[m * n];
	t -> weight = new int[m * n];
	t -> currFlow = new int[m * n];

	for (i = 0; i < m * n; i++) {
		s -> currFlow[i] = 0;
		t -> currFlow[i] = 0;
	}

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
			t -> connections[node -> id] = node;
			QAux -> push (node);
		}
	}

	for (i = 0; i < m; i++)
		for (j = 0; j < n; j++) {
			scanf("%d", &temp);
			nodes -> at(i) -> at(j) -> lp = temp;
			if(temp != 0) {
				nodes -> at(i) -> at(j) -> weight[5] = temp;
				s -> weight[i * n + j] = temp;
			}
			else {
				nodes -> at(i) -> at(j) -> connections[5] = NULL;
				s -> connections[i * n + j] = NULL;
			}
		}

	for (i = 0; i < m; i++)
		for (j = 0; j < n; j++) {
			scanf("%d", &temp);	
			nodes -> at(i) -> at(j) -> cp = temp;
			nodes -> at(i) -> at(j) -> weight[4] = temp;
			t -> weight[i * n + j] = temp;
			if (temp == 0) {
				nodes -> at(i) -> at(j) -> connections[4] = NULL;
				t -> connections[i * n + j] = NULL;
			}
		}

	for (i = 0; i < m; i++)
		for (j = 0; j < n - 1; j++) {
			scanf("%d", &temp);	
			nodes -> at(i) -> at(j) -> weight[3] = temp;
			nodes -> at(i) -> at(j + 1) -> weight[1] = temp;
			if(temp != 0) {
				nodes -> at(i) -> at(j) -> connections[3] = nodes -> at(i) -> at(j + 1);
				nodes -> at(i) -> at(j + 1) -> connections[1] = nodes -> at(i) -> at(j);
			}
			else {
				printf("Node 1: %d\tNode 2: %d\n", nodes -> at(i) -> at(j) -> id, nodes -> at(i) -> at(j + 1) -> id );
				nodes -> at(i) -> at(j) -> connections[3] = NULL;
				nodes -> at(i) -> at(j + 1) -> connections[1] = NULL;
			}
		}

	for (i = 0; i < m - 1; i++)
		for (j = 0; j < n; j++) {
			scanf("%d", &temp);	
			nodes -> at(i) -> at(j) -> weight[0] = temp;
			nodes -> at(i + 1) -> at(j) -> weight[2] = temp;
			if (temp != 0) {
				nodes -> at(i) -> at(j) -> connections[0] = nodes -> at(i + 1) -> at(j);
				nodes -> at(i + 1) -> at(j) -> connections[2] = nodes -> at(i) -> at(j); 
			}
			else{
				nodes -> at(i) -> at(j) -> connections[0] = NULL;
				nodes -> at(i + 1) -> at(j) -> connections[2] = NULL;
			}
		}
	edmundo();
	printf("\n%d\n", flowTotal);
	/*for (i = 0; i < (int) cut -> size(); i++)
		printf("%d-%d\n", cut -> at(i).first, cut -> at(i).second);*/	return 0;
}