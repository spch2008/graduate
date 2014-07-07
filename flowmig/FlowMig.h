#pragma once
#include "PathRecord.h"
#include "Graph.h"
#include "OrmPV.h"
#include <ctime>
#include <stdlib.h>

enum OP {ADD, DEL};
enum Method {RANDOM, NF, STH, WMMC};

class FlowMig
{
public:
	FlowMig(void);
	~FlowMig(void);

	
	void Init();
	void AddFlowData(int vm_id);
	void DelFlowData(int vm_id);
	void BuildMatrix(ifstream &fin);
	void InitVmNetData();
	void CheckJam(vector<string> &jam_path_list);
	// src - dst ÓµÈûÂ·¾¶
	void GetJamVM(string src, string dst, vector<int> &jam_vm_list);

	void RemoveJam(Method m);
	float MaxLinkRation();
	float AverageFlow();
	int   JamNum();
	float   MigTime() { return migTime; }

protected:
	const vector<string> *GetPath(int s_vm_id, int d_vm_id);
	void PrintError(string error);
	inline void HandleFlowData(int vm_id, OP op);
	void Do_Handle(const vector<string> &path, float flow);

	void RandomSelect();
	void IteratorSelect();
	void NetFlowSelect();
	void WMMCSelect();
	float GetMinNetWidth(int s_pm_id, int d_pm_id);

	PhyM *GetRandomPm(VirM *vm);
	PhyM *GetIteratorPm(VirM *vm);
	bool IsJam(string src, string dst);

	float Do_MigTime(float memory, int src_pm_id, int dst_pm_id);
private:
	void Test(const vector<string> &path, int data, int i, int j);

private:
	PathRecord pathRecord;
	Graph graph;
	OrmPV pv;
	vector< vector<float> > flowMatrix;

	float migTime;
};

