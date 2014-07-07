#ifndef _VERTEX_H_
#define _VERTEX_H_

#include <set>
#include "Edge.h"
using std::set;

class Vertex
{
public:
	Vertex();
	Vertex(string v);
	~Vertex(void);

	void InsertEdge(Edge &edge);
	void InsertEdge(string dvertex, float weight, float used_weight = 0);

	void RemoveEdge(string dvertex);
	void RemoveEdge(Edge &edge);

	string GetVertexName() const;
	const Edge *NextEdge();
private:
	set<Edge> adjacents;
	string vname;

	set<Edge>::const_iterator next_iter;
	bool isFirst;
};

#endif