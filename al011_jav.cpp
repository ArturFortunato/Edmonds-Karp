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
		int min;
		int parent;
		Node* daddy;
		int color;
};

// Global variables

int m = 0, n = 0, flowTotal = 0;
vector<vector<Node *> *> *nodes;
queue<Node *> *Q = new queue<Node *>();
queue<Node *> *QAux = new queue<Node *>();
Node *s = new Node();
Node *t = new Node();

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

/*int bfs() {
	int i, size, flow = 0, totalFlow = 0, contador=0;
	Node *u;
	Node *node;

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
				if(node != NULL && node -> color == 0 &&  u -> daddy != node && u -> weight[i] != u -> currFlow[i]) {
					node -> daddy = u;
					node -> parent = getParentId(node);
					if(node != t)
						node -> color = 1;
					else {
						flow = node -> weight[node -> parent] - node -> currFlow[node -> parent];
						while (node != s) {
							printf("node: %d daddy: %d flow: %d capacidade: %d\n", node -> id, node -> daddy -> id, flow, node -> weight[node -> parent] - node -> currFlow[node -> parent]);
							flow = min(flow, node -> weight[node -> parent] - node -> currFlow[node -> parent]);
							node = node -> daddy;
						}
						if(flow > 0) {
							node = t;
							printf("FLOW: %d\n", flow);
 							while (node != s) {
								node -> currFlow[node -> parent] += flow;
								node -> daddy -> currFlow[getChildId(node -> daddy, node)] += flow;
								node = node -> daddy;
								contador++;
							}
							//printf("-------\n");
							if(contador > 2)
								totalFlow += flow;
							contador = 0;
							for(i = 0; i < m * n; i++) 
								nodes -> at(i / n) -> at(i % n) -> color = 0;
						}
					}
				}
			}
		}

	return totalFlow;
}*/

int bfs() {
	int i, size, flow = 0, totalFlow = 0;
	Node *u;
	Node *node;
	while(!Q -> empty()) {
		u = Q -> front();
		Q -> pop();
		if (u == s)
			size = m * n;
		else
			size = 5;
		for (i = 0; i < size; i++) {
			node = u -> connections[i];
			if( node != NULL && u -> weight[i] != u -> currFlow[i] && node -> daddy == NULL && node -> color == 0) {
				printf("sou o node: %d\n", node->id);
				node -> daddy = u;
				node -> parent = getParentId(node);
				if (node -> min == -1)
					node -> min = u -> weight[i] - u -> currFlow[i];
				else
					node -> min = min(u -> weight[i] - u -> currFlow[i], node -> min);
				if (node != t) {
					node -> color = 1;
				}
				else if (node -> min > 0) {
					while (node != s) {
							node -> currFlow[node -> parent] += flow;
							node -> daddy -> currFlow[getChildId(node -> daddy, node)] += flow;
							node = node -> daddy;
					}
					break;
				}
			}
		}
		for (i = 0; i < m * n; i++)
			nodes -> at(i / n) -> at(i % n) -> color = 0;
		if(t -> daddy == NULL) {
			for (i = 0; i < m * n; i++)
				totalFlow += nodes -> at(i / n) -> at(i % n) -> min;
			return totalFlow;
		}
	}
	return -1;
}

void getCut(Node *node) {
	Node *temp;
	int i;
	for(i = 0; i < 4; i++)
		if ((temp = node -> connections[i]) != NULL && node -> weight[i] != node -> currFlow[i] && temp -> color != 2) {
			temp -> color = 2;
			temp -> type = 0;
			getCut(temp);
		}

}

void edmundo() {
	int currentFlow = 0, i;
	for(i = 0; i < m * n; i++)
		if(s -> connections[i] != NULL) {
			if(s -> weight[i] > nodes -> at(i / n) -> at(i % n) -> weight[4]) {
				s -> currFlow[i] += nodes -> at(i / n) -> at(i % n) -> weight[4];
				nodes -> at(i / n) -> at(i % n) -> currFlow[4] = nodes -> at(i / n) -> at(i % n) -> weight[4];
			}
			else if (s -> weight[i] < nodes -> at(i / n) -> at(i % n) -> weight[4]){
				nodes -> at(i / n) -> at(i % n) -> currFlow[4] += s -> weight[i];
				s -> currFlow[i] = s -> weight[i];
			}
		}

	while(true) {
		*Q = *QAux;
		currentFlow = bfs();
		for(i = 0; i < m*n; i++){
			nodes -> at(i / n) -> at(i % n) -> daddy = NULL;
			nodes -> at(i / n) -> at(i % n) -> parent = -1;
		}
		if(currentFlow == 0) break;
		else flowTotal += currentFlow;
	}
}

int main() {
	int i = 0, j = 0, k = 0, temp = 0, type0c = 0, type0p = 0;
	scanf("%d %d\n", &m, &n);
	if (m < 1 || n < 1) {
		printf("m and n must be greater than 1\n");
		exit(0);
	}

	nodes = new vector<vector<Node *> *>();

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
	t -> min = -1;
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
			node -> type = 1;
			node -> min = -1;
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
			nodes -> at(i) -> at(j) -> weight[5] = temp;
			s -> weight[i * n + j] = temp;
		}

	for (i = 0; i < m; i++)
		for (j = 0; j < n; j++) {
			scanf("%d", &temp);	
			nodes -> at(i) -> at(j) -> cp = temp;
			nodes -> at(i) -> at(j) -> weight[4] = temp;
			t -> weight[i * n + j] = temp;
		}

	for (i = 0; i < m; i++)
		for (j = 0; j < n - 1; j++) {
			scanf("%d", &temp);	
			nodes -> at(i) -> at(j) -> weight[3] = temp;
			nodes -> at(i) -> at(j + 1) -> weight[1] = temp;
			nodes -> at(i) -> at(j) -> connections[3] = nodes -> at(i) -> at(j + 1);
			nodes -> at(i) -> at(j + 1) -> connections[1] = nodes -> at(i) -> at(j);
		}

	for (i = 0; i < m - 1; i++)
		for (j = 0; j < n; j++) {
			scanf("%d", &temp);	
			nodes -> at(i) -> at(j) -> weight[0] = temp;
			nodes -> at(i + 1) -> at(j) -> weight[2] = temp;
			nodes -> at(i) -> at(j) -> connections[0] = nodes -> at(i + 1) -> at(j);
			nodes -> at(i + 1) -> at(j) -> connections[2] = nodes -> at(i) -> at(j); 
		}
	edmundo();
	getCut(s);
	for (i = 0; i < m; i++)
		for(j = 0; j < n; j++) {
			if (nodes -> at(i) -> at(j) -> type == 0) {
				type0c += nodes -> at(i) -> at(j) -> cp;
				type0p += nodes -> at(i) -> at(j) -> lp;
			}
			else {
				type0c += nodes -> at(i) -> at(j) -> lp;
				type0p += nodes -> at(i) -> at(j) -> cp;
			}
		}
	printf("%d\n\n", flowTotal + min(type0p, type0c));
	for(i = 0; i < m; i++){
	    for(j = 0; j < n; j++){
	        Node* node = nodes -> at(i) -> at(j);
		    if(node -> type == 1 && type0c < type0p){
			    printf("P ");
		    }
		    else
			    printf("C ");
	    }
	    printf("\n");
	}
	return 0;
}