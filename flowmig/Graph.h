#ifndef _GRAPH_H_
#define _GRAPH_H_

#include <map>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include "Vertex.h"
#include <stdlib.h>

#include "Dijkstra.h"
using namespace std;


class Graph
{
public:
	enum GraphType { DIRECTED, UNDIRECTED };

public:
	Graph(GraphType type = UNDIRECTED);
	~Graph(void);

	void BuildGraph(ifstream &fin);

	void AddVertex(string vertex);
	void AddEdge(string srcv, string dstv, float weight);

	void RemoveVertex(string vertex);
	void RemoveEdge(string srcv, string dstv);

	void ShortPath(string src, string dst, vector<string> &path);
	const Vertex *NextVertex() const;
	
	Vertex *FindVertex(string vertex);
	Edge* GetEdge(string src, string dst);
protected:
	
	void PrintError(string error);

	///friend class ShortPath;
private:
	map<string, Vertex> vertices;
	GraphType graphType;

	mutable map<string, Vertex>::const_iterator vertex_iter;
	mutable bool isFirst;
};


#endif
