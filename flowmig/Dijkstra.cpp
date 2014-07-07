#include "Dijkstra.h"

Dijkstra::Dijkstra(Graph &g) : graph(g)
{
}


Dijkstra::~Dijkstra(void)
{
}

void Dijkstra::InitTable(string src)
{
	table.clear();

	const Vertex *v;
	while ((v = graph.NextVertex()) != NULL)
	{
		table[v->GetVertexName()] = PathInfo();
	}
	table[src].dist = 0;
	
}

string Dijkstra::GetMinUnkownV()
{
	
	float min = MAX_DIST;
	string vertex;

	map<string, PathInfo>::iterator iter;
	for(iter = table.begin(); iter != table.end(); iter++)
	{
		if (iter->second.known == true)
			continue;

		if (iter->second.dist < min)
		{
			min = iter->second.dist;
			vertex = iter->first;
		}
	}
	return vertex;
}

void Dijkstra::ShortPath(string src, string dst, vector<string> &path)
{
	string v, w;
	const Edge *e = NULL;
	Vertex *node;
	
	if (src == dst)
		return;

	InitTable(src);
	for (; ;)
	{
		v = GetMinUnkownV();
		if (v == "")
			break;

		table[v].known = 1;
		node = graph.FindVertex(v);

		while ((e = node->NextEdge()) != NULL)
		{
			w = e->GetAdjVertexName();
			if (table[w].known == 1)
				continue;

			float leftFlow = 1/(e->GetWeight() - e->GetUsedWeight()) * 10000;
			if (table[v].dist + leftFlow < table[w].dist)
			{
				table[w].dist = table[v].dist + leftFlow;
				table[w].path = v;
			}
		}
	}

	stack<string> s;
	s.push(dst);
	while (true)
	{
		v = table[dst].path;
		s.push(v);

		if (v == src)
			break;
		dst = v;
	}

	while (!s.empty())
	{
		path.push_back(s.top());
		s.pop();
	}
}