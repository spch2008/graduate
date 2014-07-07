#pragma once
#include "VirM.h"
#include <set>
using namespace std;

class PhyM
{
public:
	PhyM(int pm_id, int c, int m);
	~PhyM(void);

	bool CanPut(VirM *vm);
	bool Put(VirM *vm);
	bool HasVm(int vm_id);
	int  GetId() const;
	void RemoveVM(int vm_id);
	float CpuRatio();

	const VirM *NextVM();
private:
	int cpu;
	int memory;
	int id;

	int used_cpu;
	int used_memory;

	set<VirM*> vm_list; 
	set<VirM*>::iterator vm_iter;
	bool isFirst;
};

