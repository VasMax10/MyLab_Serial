#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <time.h>
#include <iostream>

struct Edge
{
	int from;
	int to;
	double weight;
};

struct Graph
{
	int V; // кількість вершин
	int E; // кількість ребер

	Edge* edge;
};

// A structure to represent a Set for union-find
struct Set
{
	int parent;
	int rank;
};

// Function prototypes for union-find (These functions are defined after boruvkaMST() )
int find(struct Set sets[], int i);
void Union(struct Set sets[], int x, int y);

void serialBoruvkaMST(struct Graph* graph)
{
	int V = graph->V;
	int E = graph->E;
	Edge* edge = graph->edge;

	struct Set* sets = new Set[V];

	// масив для збереження індексів ребер найменшої ваги.
	int* cheapest = new int[V];

	// Create V subsets with single elements
	for (int v = 0; v < V; v++)
	{
		sets[v].parent = v;
		sets[v].rank = 0;
		cheapest[v] = -1;
	}

	// Початково маємо V різних дерев.
	// Фінально отримаємо одне дерево, що і буде MST.
	int numTrees = V;
	double MSTweight = 0;

	// Keep combining components (or sets) until all compnentes are not combined into single MST.

	while (numTrees > 1)
	{
		// Everytime initialize cheapest array
		for (int v = 0; v < V; v++)
		{
			cheapest[v] = -1;
		}

		// Traverse through all edges and update cheapest of every component
		for (int i = 0; i < E; i++)
		{
			int set1 = find(sets, edge[i].from);
			int set2 = find(sets, edge[i].to);

			// If two corners of current edge belong to same set, ignore current edge. 
			if (set1 == set2)
				continue;
			// Else check if current edge is closer to previous cheapest edges of set1 and set2
			else
			{
				if (cheapest[set1] == -1 ||
						edge[cheapest[set1]].weight > edge[i].weight)
					cheapest[set1] = i;
				if (cheapest[set2] == -1 ||
						edge[cheapest[set2]].weight > edge[i].weight)
					cheapest[set2] = i;
			}
		}

		// Consider the above picked cheapest edges and add them to MST
		for (int v = 0; v < V; v++)
		{
			// Check if cheapest for current set exists
			if (cheapest[v] != -1)
			{
				int set1 = find(sets, edge[cheapest[v]].from);
				int set2 = find(sets, edge[cheapest[v]].to);

				if (set1 == set2)
					continue;
				MSTweight += edge[cheapest[v]].weight;
				//printf("Edge %d-%d included in MST\n",
					//edge[cheapest[v]].from, edge[cheapest[v]].to);
				// Do a union of set1 and set2 and decrease number of trees

				Union(sets, set1, set2);
				numTrees--;
			}
		}
	}
	
	printf("Weight of MST is %f\n", MSTweight);
	return;
}

struct Graph* createGraph(int V, int E)
{
	Graph* graph = new Graph;
	graph->V = V;
	graph->E = E;
	graph->edge = new Edge[E];
	return graph;
}

// A utility function to find set of an element i
// (uses path compression technique)

int find(struct Set sets[], int i)
{
	// find root and make root as parent of i
	// (path compression)
	if (sets[i].parent != i)
		sets[i].parent = find(sets, sets[i].parent);
	return sets[i].parent;
}

void Union(struct Set sets[], int x, int y)
{
	int xroot = find(sets, x);
	int yroot = find(sets, y);

	// Attach smaller rank tree under root of high
	// rank tree (Union by Rank)
	if (sets[xroot].rank < sets[yroot].rank)
		sets[xroot].parent = yroot;
	else if (sets[xroot].rank > sets[yroot].rank)
		sets[yroot].parent = xroot;
	// If ranks are same, then make one as root and
	// increment its rank by one
	else
	{
		sets[yroot].parent = xroot;
		sets[xroot].rank++;
	}
}

int main()
{
	int V;  // Number of vertices in graph
	int E;  // Number of edges in graph

	//std::cin >> V >> E;
	printf("Serial Minimum Spanning Tree Program\n\n");
	printf("Enter count of vertices: ");
	std::cin >> V;
	E = V * (V - 1) / 2;
	struct Graph* graph = createGraph(V, E);
	
	int count = 0;
	time_t start1, finish1;
	double duration1;
	start1 = clock();
	srand(time(NULL));
	for (int i = 0; i < V; i++)
			for (int j = i + 1; j < V; j++)
			{
				double a, b, c;
				a = i;
				b = j;
				c = rand() % V + 1;//b + a;
				graph->edge[count].from = a;
				graph->edge[count].to = b;
				graph->edge[count].weight = c;
				//std::cout << a << " " << b << " " << c << std::endl;
				count++;
			}
	finish1 = clock();
	duration1 = (finish1 - start1) / double(CLOCKS_PER_SEC);

	//printf("\nTime of initialization: %f\n\n", duration1);
		
	time_t start, finish;
	double duration;
	start = clock();
	
	serialBoruvkaMST(graph);
	
	finish = clock();
	duration = (finish - start) / double(CLOCKS_PER_SEC);
	
	printf("Time of serial execution: %f\n", duration);// +duration1);

	return 0;
}