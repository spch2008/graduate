#pragma once
#include "VirM.h"
#include "PhyM.h"
#include <string>
#include <sstream>
#include <fstream>
#include <map>
using namespace std;

class OrmPV
{
public:
	OrmPV(void);
	~OrmPV(void);

	void BuildPM(ifstream &fin);
	void BuildVM(ifstream &fin);
	void BindPmVm();
	void Handle(string pm_txt, string vm_txt);

	string GetSwByVm(int vm_id);
	string GetSwByPm(int pm_id);
	int    GetPmByVm(int vm_id);

	VirM *GetVM(int vm_id);
	PhyM *GetPm(int pm_id);

	PhyM *NextPM();

	int GetPmSize() { return pm_list.size(); }
private:
	map<int, PhyM*> pm_list;
	map<int, VirM*> vm_list_tmp;

	bool isFirst;
	map<int, PhyM*>::iterator pm_iter;
};

