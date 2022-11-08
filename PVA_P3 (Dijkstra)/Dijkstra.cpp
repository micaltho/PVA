#include <iostream>
#include <climits>
#include <string>
#include <fstream>
#include <sstream>
#include <math.h>
#include "oneapi/tbb.h"
#include <oneapi/tbb/task_arena.h>
#include <cassert>

using namespace std;

size_t V = 0; // Größe der Matrix (Anzahl der Knoten)

int get_next_node(vector<bool> visited_nodes, vector<int> min_distance)
{
	int min_dist = INT_MAX;
	int loc_node = INT_MAX;
    
	for (size_t i = 0; i < V; i++)
	{
		if (!visited_nodes[i])
		{
			if (min_distance[i] < min_dist)
			{
				cout << "hallo" << std::endl;
				min_dist = min_distance[i];
				loc_node = i;
				
			}
			
		}
	}
	return loc_node;
}

void readFromFile(vector<vector<int>> &graph)
{
	// Einlesen der Daten und schreiben in Matrix
	string zeile;
	ifstream file("user.txt");
	while (getline(file, zeile))
	{
		stringstream zeilenpuffer(zeile);
		int b, c, d;
		string a;
		zeilenpuffer >> a >> b >> c >> d;
		if (a == "a")
		{
			graph[b - 1][c - 1] = d;
		}
	}
}

void print10x10Matrix(vector<vector<int>> &graph)
{
	// Ausgeben der Matrix bis max. 10x10
	size_t max_output = V;
	if (V > 10)
		max_output = 10;

	for (size_t x = 0; x < max_output; x++)
	{
		cout << x + 1 << ":  ";
		for (size_t y = 0; y < max_output; y++)
		{
			cout << graph[x][y];
			cout << "\t";
		}
		cout << endl;
	}
	cout << endl;
}

void Dijkstra()
{
	cout << "Bitte Matrix-Groeße eingeben (Anzahl der Knoten): " << endl;
	cin >> V;

	// 2D Matrix as Vector
	vector<vector<int>> graph(V, vector<int>(V, 0));

	readFromFile(graph);
	print10x10Matrix(graph);

	int start_point = 0;				  // Startpunkt
	vector<int> min_distance(V, INT_MAX); // Feld mit minimalen Distanzen
	vector<bool> visited_nodes(V, false); // Feld aller besuchten Knoen
	vector<int> predecessor(V, 0);		  // Feld mit Vorgängern
	bool finish = false;				  // Alle Knoten besucht

	// Distanz vom Startknoten = 0
	min_distance[start_point] = 0;
	visited_nodes[0] = true;
    
	int row = 0;

	while (!finish)
	{
		
		oneapi::tbb::parallel_for(oneapi::tbb::blocked_range<size_t>(0, V),
								  [&](const oneapi::tbb::blocked_range<size_t> &range)
								  {
									  for (size_t it = range.begin(); it < range.end(); it++)
										  if (graph[row][it] > 0 && visited_nodes[it] == false)
											  if (min_distance[it] > graph[row][it] + min_distance[row])
											  {
												  min_distance[it] = graph[row][it] + min_distance[row];
												  predecessor[it] = row;
											  }
								  });

		row = get_next_node(visited_nodes, min_distance);
		visited_nodes[row] = true;
		
		if (!finish)
			cout << "Next Node: " << row + 1 << endl;
		
		if (count(visited_nodes.begin(), visited_nodes.end(), false) == 0)
			finish = true;
			
	}

	//Ausgeben der Minimalen Distanzen zu 1
	for (size_t i = 0; i < V; i++)
	{
		cout << i + 1 << ": " << min_distance[i] << "  Vorgaenger: " << predecessor[i] + 1 << endl;
	}
}

int main()
{
	oneapi::tbb::global_control global_limit(oneapi::tbb::global_control::max_allowed_parallelism, 2);
	auto num_threads = oneapi::tbb::global_control::active_value(oneapi::tbb::global_control::max_allowed_parallelism);
	cout << "Number of active threads: " << num_threads << endl;
	cout << "Starte Dijkstra-Algorithmus..." << endl;
	Dijkstra();
	return 0;
}