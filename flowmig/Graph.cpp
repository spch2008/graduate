#include "Graph.h"
class ShortPath;

Graph::Graph(GraphType type)
{
	graphType = type;
	isFirst = true;
}


Graph::~Graph(void)
{
}



void Graph::BuildGraph(ifstream &fin)
{
	string line, src, dst;
	float weight;

	while (getline(fin, line))
	{
		if (line.find("#begin") != string::npos)
			break;
	}

	while (getline(fin, line))
	{
		if (line == "")
			continue;
		else if (line.find("#end") != string::npos)
			break;

		istringstream sin(line);
		sin >> src >> dst >> weight;
		
		AddEdge(src, dst, weight);
	}

	fin.close();
}

void Graph::AddVertex(string vertex)
{
	if (FindVertex(vertex) != NULL)
		return;

	Vertex v(vertex);
	vertices[vertex] = v;
}

void Graph::AddEdge(string srcv, string dstv, float weight)
{
	AddVertex(srcv);
	Vertex *sv = FindVertex(srcv);

	if (sv == NULL)
		PrintError("ERROR: AddEdge");
	
	sv->InsertEdge(dstv, weight, 0);

	if (graphType == UNDIRECTED)
	{
		AddVertex(dstv);
		Vertex *dv = FindVertex(dstv);

		if (dv == NULL)
			PrintError("ERROR: AddEdge");
		dv->InsertEdge(srcv, weight, 0);
	}

}

void Graph::RemoveVertex(string vertex)
{
	Vertex *v = FindVertex(vertex);
	if (v == NULL)
		return;

	map<string, Vertex>::iterator iter = vertices.find(vertex);
	vertices.erase(iter);
}

void Graph::RemoveEdge(string srcv, string dstv)
{
	Vertex *sv = FindVertex(srcv);
	if (sv == NULL)
		PrintError("Not have a edge");

	sv->RemoveEdge(dstv);

	if (graphType == UNDIRECTED)
	{
		Vertex *dv = FindVertex(dstv);
		if (dv == NULL)
			PrintError("Not have a edge");
		dv->RemoveEdge(srcv);
	}
}


Vertex *Graph::FindVertex(string vertex)
{
	if (vertices.count(vertex) == 0)
		return NULL;

	Vertex &v = vertices[vertex];
	return &v;
}


void Graph::PrintError(string error)
{
	std::cout << error << std::endl;
	//system("pause");
	exit(0);
}


const Vertex *Graph::NextVertex() const
{
	/*
	static map<string, Vertex>::const_iterator iter = vertices.begin();
	map<string, Vertex>::const_iterator oldIter;

	if (iter != vertices.end())
	{
		oldIter = iter;
		iter++;
		return &(oldIter->second);
	}
	else
	{
		iter = vertices.begin();
		return NULL;
	}
	*/

	if (isFirst)
	{
		isFirst = false;
		vertex_iter = vertices.begin();
	}

	if (vertex_iter != vertices.end())
		return &((vertex_iter++)->second);
	else
	{
		isFirst = true;
		return NULL;
	}
}


void Graph::ShortPath(string src, string dst, vector<string> &path)
{
	Dijkstra dj(*this);
	dj.ShortPath(src, dst, path);
}


Edge* Graph::GetEdge(string src, string dst)
{
	Vertex *v = FindVertex(src);
	
	const Edge *e;
	const Edge *dst_e = NULL;
	while ((e = v->NextEdge()) != NULL)  //必须遍历一遍，让其回到初始状态
	{
		if(e->GetAdjVertexName() == dst)
		{
			dst_e = e;
		}
	}
	return const_cast<Edge*>(dst_e);
	
}