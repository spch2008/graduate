#ifndef _EDGE_H_
#define _EDGE_H_

#include <string>

using std::string;

class Edge
{
public:
	Edge(string dvert, float w, float uw = 0);
	~Edge(void);

public:
	float GetWeight() const;
	float GetUsedWeight() const;
	string GetAdjVertexName() const;
	bool operator<(const Edge& that) const;
	void SetUsedWeight(float w);

private:
	float weight;
	float used_weight;
	string vertex;
};

#endif