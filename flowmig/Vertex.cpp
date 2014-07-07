#include "Vertex.h"


Vertex::Vertex()
{
	vname = "";
	isFirst = true;
}

Vertex::Vertex(string v)
{
	vname = v;
	isFirst = true;
}


Vertex::~Vertex(void)
{
}


void Vertex::InsertEdge(string dvertex, float weight, float used_weight)
{
	Edge edge(dvertex, weight, used_weight);
	InsertEdge(edge);
}

void Vertex::InsertEdge(Edge &edge)
{
	adjacents.insert(edge);
}


void Vertex::RemoveEdge(string dvertex)
{
	Edge edge(dvertex, 0);
	RemoveEdge(edge);
}


void Vertex::RemoveEdge(Edge &edge)
{
	adjacents.erase(edge);
}

string Vertex::GetVertexName() const
{
	return vname;
}

const Edge *Vertex::NextEdge()
{
	set<Edge>::const_iterator oldIter;

	if (isFirst)
	{
		next_iter = adjacents.begin();
		isFirst = false;
	}

	if (next_iter != adjacents.end())
	{
		oldIter = next_iter++;
		return &(*oldIter);
	}
	else
	{
		isFirst = true;
		return NULL;
	}

	/*
	static set<Edge>::iterator iter = adjacents.begin();

	set<Edge>::iterator oldIter;
	set<Edge>::iterator end = adjacents.end();
	if (iter != end)
	{
		oldIter = iter++;
		return &(*oldIter);
	}
	else
	{
		iter = adjacents.begin();
		return NULL;
	}
	*/
}