/* Grupo 11
   86388 - Artur Fortunato
   86482 - Miguel Rocha
*/


//Libraries that were used
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
		Node* parentNode;
		int color;
};

// Global variables
int m = 0, n = 0, flowTotal = 0;
vector<vector<Node *> *> *nodes;
queue<Node *> *Q = new queue<Node *>();
queue<Node *> *QAux = new queue<Node *>();
Node *s = new Node();
Node *t = new Node();


/*--------------getParentId---------------------
| Returns the connection with the parent vertex
----------------------------------------------*/
int getParentId(Node *n) {
	Node *parent = n -> parentNode;
	int i;
	if (n == t)
		return parent -> id;
	for(i = 0; i < 6; i++) {
		if(n -> connections[i] == parent)
			break;
	}
	return i;
}

/*--------------getChildId---------------------
| Returns the connection with the child vertex
----------------------------------------------*/
int getChildId(Node *parent, Node *child) {
	int i;
	if (parent == s)
		return child -> id;
	for (i = 0; i < 5; i++)
		if (parent -> connections[i] == child)
			return i;
	return -1;
}

/*--------------------bfs----------------------
| Searches the graph for possible paths between
| the source and the target
----------------------------------------------*/
int bfs() {
	Node *u;
	Node *node;
	int i, flow = 0, totalFlow = 0, size;
	while(Q -> size()) {
		u = Q -> front();
		Q -> pop();
		if (u == s)
			size = m * n;
		else
			size = 5;
		if(u == s || u -> parentNode != NULL) {
			for(i = 0; i < size; i++) {
				node = u -> connections[i];
				if (node != NULL && u -> weight[i] != u -> currFlow[i] && node -> color == 0 ) {
					node -> parentNode = u;
					node -> parent = getParentId(node);
					if (node != t) {
						Q -> push(node);
							node -> color = 1;
					}
					else {
						flow = u -> weight[i] - u -> currFlow[i];
						while(node != s) {
							flow = min(flow, node -> parentNode -> weight[getChildId(node -> parentNode, node)] - node -> parentNode -> currFlow[getChildId(node -> parentNode, node)]);
							node = node -> parentNode;
						}
						node = t;
						totalFlow += flow;
						while(node != s) {
							node -> weight[node -> parent] += flow;
							node -> parentNode -> currFlow[getChildId(node -> parentNode, node)] += flow;
							node = node -> parentNode;
						}
						*Q = *QAux;
						for(i = 0; i < m*n; i++){
							nodes -> at(i / n) -> at(i % n) -> parentNode = NULL;
							nodes -> at(i / n) -> at(i % n) -> parent = -1;
							nodes -> at(i / n) -> at(i % n) -> color = 0;
						}
					}
				}
			}
		}
	}
	return totalFlow;
}

/*--------------------getCut---------------------
| After the entire flow is sent through the graph
| a second BFS will find the edges that aren't
| saturated and will classify them as background
-----------------------------------------------*/
void getCut() {
	Node *temp;
	Node *u;
	int i, size;
	while(QAux -> size()) {
		u = QAux -> front();
		QAux -> pop();
		if(u == s)
			size = m * n;
		else 
			size = 4;
		for(i = 0; i < size; i++) {
			if ((temp = u -> connections[i]) != NULL && u -> weight[i] != u -> currFlow[i] && temp -> color != 2) {
				temp -> color = 2;
				temp -> type = 0;
				QAux -> push(temp);
			}
		}
	}
}

/*-----------------edmondskarp-------------------
| Runs multiple BFS until no more flow can pass
| through the graph edges (when the BFS returns 0)
-----------------------------------------------*/
void edmondskarp() {
	int currentFlow = 0;
	while(true) {
		*Q = *QAux;
		currentFlow = bfs();
		if(currentFlow == 0) break;
		else flowTotal += currentFlow;
	}
}

/* --------------------------- main ---------------------------------
| Executes the program
------------------------------------------------------------------ */
int main() {
	int i = 0, j = 0, k = 0, temp = 0, type0c = 0, type0p = 0, totalCP = 0, totalLP = 0;
	string letra = "C ", str = "";
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
			node -> parentNode = NULL;
			node -> parent = -1;
			node -> type = 1;
			nodes -> at(i) -> push_back(node);
			s -> connections[node -> id] = node;
			t -> connections[node -> id] = node;
		}
	}

	for (i = 0; i < m; i++)
		for (j = 0; j < n; j++) {
			scanf("%d", &temp);
			nodes -> at(i) -> at(j) -> lp = temp;
			nodes -> at(i) -> at(j) -> weight[5] = temp;
			s -> weight[i * n + j] = temp;
			totalLP += temp;
		}

	for (i = 0; i < m; i++)
		for (j = 0; j < n; j++) {
			scanf("%d", &temp);	
			nodes -> at(i) -> at(j) -> cp = temp;
			nodes -> at(i) -> at(j) -> weight[4] = temp;
			t -> weight[i * n + j] = temp;
			totalCP += temp;
			s -> currFlow[i * n + j] += min(temp, nodes -> at(i) -> at(j) -> lp);
			t -> currFlow[i * n + j] += min(temp, nodes -> at(i) -> at(j) -> lp);
			nodes -> at(i) -> at(j) -> currFlow[4] += min(temp, nodes -> at(i) -> at(j) -> lp);
			nodes -> at(i) -> at(j) -> currFlow[5] += min(temp, nodes -> at(i) -> at(j) -> lp);
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
	edmondskarp();
	getCut();
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
	printf("%d\n\n", (min(type0p, type0c) < min(totalLP, totalCP)) ? flowTotal + min(min(type0p, type0c), min(totalLP, totalCP)):min(min(type0p, type0c), min(totalLP, totalCP)) );
	if(min(totalCP, totalLP) > min(type0p, type0c)) {
		for(i = 0; i < m; i++){
		    for(j = 0; j < n; j++){
	    	    Node* node = nodes -> at(i) -> at(j);
		    	if(node -> type == 1 && type0c < type0p)
			    	str += "P ";
		   		else
			    	str += "C ";
	    	}
	    	str += "\n";
		}
	}
	else {
		if (totalLP <= totalCP)
			letra = "P ";
		for(i = 0; i < m; i++){
		    for(j = 0; j < n; j++)
		    	str += letra;
	    str += "\n"; 
		}
	}
	printf("%s", str.c_str());
	return 0;
}