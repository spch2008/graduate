#include "PhyM.h"


PhyM::PhyM(int i, int c, int m)
{
	cpu = c;
	memory = m;
	id = i;

	used_cpu = 0;
	used_memory = 0;

	isFirst = true;
}


PhyM::~PhyM(void)
{
}

bool PhyM::CanPut(VirM *vm)
{
	if ((cpu - used_cpu) >= vm->CPU() && (memory - used_memory) >= vm->Memory())
		return true;
	return false;
}

bool PhyM::Put(VirM *vm)
{
	if (!CanPut(vm))
		return false;

	used_cpu += vm->CPU();
	used_memory += vm->Memory();

	vm_list.insert(vm);
	return true;
}

int PhyM::GetId() const
{
	return id;
}

void PhyM::RemoveVM(int vm_id)
{
	//VirM v(vm_id, 0, 0);
	//vm_list.erase(&v);

	set<VirM*>::iterator iter;
	for (iter = vm_list.begin(); iter != vm_list.end(); iter++)
	{
		if ((*iter)->ID() == vm_id)
		{
			used_cpu -= (*iter)->CPU();
			used_memory -= (*iter)->Memory();
			vm_list.erase(iter);
			break;
		}
	}
}


float PhyM::CpuRatio()
{
	if (cpu == 0)
		return 1;

	return static_cast<float>(used_cpu * 1.0 / cpu);
}

bool PhyM::HasVm(int vm_id)
{
	set<VirM*>::iterator iter;
	for (iter = vm_list.begin(); iter != vm_list.end(); iter++)
	{
		if ((*iter)->ID() == vm_id)
			return true;
	}
	return false;
}

const VirM *PhyM::NextVM()
{
	if (isFirst)
	{
		isFirst = false;
		vm_iter = vm_list.begin();
	}

	if (vm_iter != vm_list.end())
	{
		return *vm_iter++;
	}
	else
	{
		isFirst = true;
		return NULL;
	}

}