#include "Edge.h"


Edge::Edge(string dvert, float w, float uw)
{
	vertex = dvert;
	weight = w;
	used_weight = uw;
}


Edge::~Edge(void)
{
}

float Edge::GetWeight() const
{
	return weight;
}

float Edge::GetUsedWeight() const
{
	return used_weight;
}

bool Edge::operator<(const Edge& that) const
{
	return vertex < that.vertex;
}

string Edge::GetAdjVertexName()const
{
	return vertex;
}

void Edge::SetUsedWeight(float w)
{
	used_weight = w;
}