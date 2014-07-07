#include "VirM.h"


VirM::VirM(int i, int c, int m)
{
	cpu = c;
	memory = m;
	id = i;
}


VirM::~VirM(void)
{
}



bool VirM::operator<(const VirM &that) const
{
	return id < that.id;
}