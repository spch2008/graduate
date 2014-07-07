#include "OrmPV.h"

const float MAX_PERCENT = (float)0.6;
const int   PM_MAP_SW = 2;

OrmPV::OrmPV(void)
{
	isFirst = true;
}


OrmPV::~OrmPV(void)
{
}


void OrmPV::BuildPM(ifstream &fin)
{
	string line;
	int cpu, memory;
	int id = 0;

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
		sin >> cpu >> memory;

		PhyM *p = new PhyM(id, cpu, memory);
		pm_list[id] = p;
		id++;
	}

	fin.close();
}
	
void OrmPV::BuildVM(ifstream &fin)
{
	string line;
	int cpu, memory;
	int id = 0;

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
		sin >> cpu >> memory;

		VirM *v = new VirM(id, cpu, memory);
		vm_list_tmp[id] = v;
		id++;
	}

	fin.close();
}

void OrmPV::BindPmVm()
{
	map<int, PhyM*>::iterator iter = pm_list.begin();

	int vm_id = 0;
	for ( ; iter != pm_list.end(); )
	{
		VirM *vm = vm_list_tmp[vm_id];
		PhyM *pm = iter->second;

		if (pm->CpuRatio() < MAX_PERCENT)
		{
			if (pm->CanPut(vm))
			{
				pm->Put(vm);
				vm_id++;
			}
		}
		else
		{
			iter++;
		}
		if ((unsigned int)vm_id >= vm_list_tmp.size())
		{
			//vm_list_tmp.clear();
			break;
		}
	}
}

void OrmPV::Handle(string pm_txt, string vm_txt)
{
	ifstream pin(pm_txt.c_str());
	BuildPM(pin);

	ifstream vin(vm_txt.c_str());
	BuildVM(vin);

	BindPmVm();
}



string OrmPV::GetSwByVm(int vm_id)
{
	int pm_id = GetPmByVm(vm_id);
	if (pm_id == -1)
		return "";
	else
		return GetSwByPm(pm_id);
}

string OrmPV::GetSwByPm(int pm_id)
{
	if (pm_id < 0)
		return "";

	int s_id =  pm_id / PM_MAP_SW;

     stringstream num_s;
     num_s<<s_id;
     string s = num_s.str();

	return  "A" + s;
}
	
int OrmPV::GetPmByVm(int vm_id)
{
	map<int, PhyM*>::iterator iter;

	for (iter = pm_list.begin(); iter != pm_list.end(); iter++)
	{
		PhyM *p = iter->second;
		if (p->HasVm(vm_id))
			return iter->first;
	}
	return -1;
}

PhyM *OrmPV::NextPM()
{
	if (isFirst)
	{
		isFirst = false;
		pm_iter = pm_list.begin();
	}

	PhyM *p = NULL;
	if (pm_iter != pm_list.end())
	{
		return (pm_iter++)->second;
	}
	else
	{
		isFirst = true;
		return NULL;
	}
}

VirM *OrmPV::GetVM(int vm_id)
{
	if (vm_id < 0 || vm_id >= static_cast<unsigned>(vm_list_tmp.size()))
		return NULL;

	return vm_list_tmp[vm_id];
}

PhyM *OrmPV::GetPm(int pm_id)
{
	if (pm_id < 0 || pm_id >= static_cast<unsigned>(pm_list.size()))
		return NULL;

	return pm_list[pm_id];
}