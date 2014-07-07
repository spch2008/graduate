#ifndef _DIJKSTRA_H_
#define _DIJKSTRA_H_

#include <stack>
#include <map>
#include <vector>
#include "Graph.h"

using namespace std;

class Graph;
const float MAX_DIST = 999999.0;

class Dijkstra
{
public:
	Dijkstra(Graph &g);
	~Dijkstra(void);

	void ShortPath(string src, string dst, vector<string> &path);

protected:
	void InitTable(string src);
	string GetMinUnkownV();

private:
	struct PathInfo
	{
		float  dist;
		bool   known;
		string path;

		PathInfo()
		{
			dist = MAX_DIST;
			known = 0;
			path = "";
		}
	};

	map<string, PathInfo> table;
	Graph &graph;
};

#endif