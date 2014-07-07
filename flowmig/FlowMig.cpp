#include "FlowMig.h"


const float JAM_RATIO = static_cast<float>(0.8);
const double TIMES = 10;     //次数
const double PERCENT = 0.2;  //脏页
const double MIGCOST = 0.04;  //迁移代价, 分钟

FlowMig::FlowMig(void)
{
	Init();
	migTime = 0;
}


FlowMig::~FlowMig(void)
{
}

void FlowMig::Init()
{
	//建图
	ifstream fin("graph.txt");
	graph.BuildGraph(fin);

	//虚拟机物理机,物理机交换机关联
	pv.Handle("pm.txt", "vm.txt");

	//流量矩阵

	ifstream f("netdata.txt");
	BuildMatrix(f);

	//初始流量加载
	InitVmNetData();
}




void FlowMig::BuildMatrix(ifstream &fin)
{
	string line;
	float val;
	unsigned index = 0;

	while(getline(fin, line))
	{
		istringstream in(line);
		while (in >> val)
		{
			if (index >= flowMatrix.size())
				flowMatrix.resize(index + 1);
			flowMatrix[index].push_back(val);
		}
		index++;
	}
	fin.close();
}

void FlowMig::InitVmNetData()
{
	const vector<string> *path;

	int matrix_size = flowMatrix.size();
	for (int i = 0; i < matrix_size; i++)
	{
		for (int j = 0; j < matrix_size; j++)
		{
			//排除自身通信 与 流量为0点
			if (i == j || flowMatrix[i][j] == 0)
				continue;

			string s_sw = pv.GetSwByVm(i);
			string d_sw = pv.GetSwByVm(j);

			//排除同一交换机下物理机
			if (s_sw == d_sw)
				continue;

			path = GetPath(i, j);
			//虚拟机路径记录
			Do_Handle(*path, flowMatrix[i][j]);
		}
	}
}


//保证肯定能获取路径
const vector<string> *FlowMig::GetPath(int s_vm_id,  int d_vm_id)
{
	vector<string> *p = pathRecord.GetPath(s_vm_id, d_vm_id);
	if (p != NULL)
		return p;

	vector<string> npath;
	string src = pv.GetSwByVm(s_vm_id);
	string dst = pv.GetSwByVm(d_vm_id);

	graph.ShortPath(src, dst, npath);
	pathRecord.SetPath(s_vm_id, d_vm_id, npath);

	p = pathRecord.GetPath(s_vm_id, d_vm_id);
	if (p == NULL)
		PrintError("GetPath Error");

	return p;
}


void FlowMig::PrintError(string error)
{
	std::cout << error << endl;
	//system("pause");
	exit(1);
}

void FlowMig::AddFlowData(int vm_id)
{
	HandleFlowData(vm_id, ADD);
}

void FlowMig::DelFlowData(int vm_id)
{
	HandleFlowData(vm_id, DEL);
}

void FlowMig::HandleFlowData(int vm_id, OP op)
{
	const vector<string> *path;

	int factor = 1;
	if (op == DEL)
		factor = -1;

	int matrix_size = flowMatrix.size();
	for (int i = 0; i < matrix_size; i++)
	{
		for (int j = 0; j < matrix_size; j++)
		{
			//排除自身通信 与 流量为0点
			if (i == j || flowMatrix[i][j] == 0)
				continue;

			//排除不与vm_id通信的虚拟机
			if (!(i == vm_id || j == vm_id))
				continue;

			string s_sw = pv.GetSwByVm(i);
			string d_sw = pv.GetSwByVm(j);

			//排除同一交换机下物理机
			if (s_sw == d_sw)
				continue;

			
			path = GetPath(i, j);
			Do_Handle(*path, flowMatrix[i][j] * factor);
		}
	}
}

void FlowMig::Do_Handle(const vector<string> &path, float flow)
{
	if (path.size() == 0)
		return;

	vector<string>::const_iterator prev_iter;
	vector<string>::const_iterator curr_iter;


	prev_iter = path.begin();
	curr_iter = prev_iter;
	for (curr_iter++; curr_iter != path.end(); curr_iter++)
	{
		Edge *edge = graph.GetEdge(*prev_iter, *curr_iter);
		
		float new_flow = edge->GetUsedWeight() + flow;
		edge->SetUsedWeight(new_flow);
		prev_iter = curr_iter;
	}

	//Test(path, flow, 0, 0);
}



void FlowMig::Test(const vector<string> &path, int data, int i, int j)
{
	if (path.size() == 0)
		return;

	vector<string>::const_iterator iter;
	for (iter = path.begin(); iter != path.end(); iter++)
		cout << *iter << " ";
	cout << endl;

	cout << i << " " << j << " " << data << endl;

	cout << endl;
}

void FlowMig::CheckJam(vector<string> &jam_path_list)
{
	const Vertex *v;
	while ((v = graph.NextVertex()) != NULL)
	{
		const Edge *e;
		while ((e = const_cast<Vertex*>(v)->NextEdge()) != NULL)
		{
			float ratio = e->GetUsedWeight() / e->GetWeight();
			if (ratio > JAM_RATIO)
			{
				jam_path_list.push_back(v->GetVertexName());
				jam_path_list.push_back(e->GetAdjVertexName());
			}
		}
	}
}

void FlowMig::GetJamVM(string src, string dst, vector<int> &jam_vm_list)
{
	pathRecord.PathVmList(src, dst, jam_vm_list);
}

void FlowMig::RemoveJam(Method m)
{
	switch (m)
	{
	case RANDOM:
		RandomSelect();
		break;
	case NF:
		IteratorSelect();
		break;
	case STH:
		NetFlowSelect();
		break;
	case WMMC:
		WMMCSelect();
		break;
	}
}

float FlowMig::GetMinNetWidth(int s_pm_id, int d_pm_id)
{
	string src = pv.GetSwByPm(s_pm_id);
	string dst = pv.GetSwByPm(d_pm_id);
	vector<string> path;
	graph.ShortPath(src, dst, path);

	//同一交换机下,无穷大
	if (path.size() == 0)
		return 9999999.0;


	vector<string>::iterator prev_iter = path.begin();
	vector<string>::iterator curr_iter = path.begin();
	
	if (path.size() < 2)
		return 1;

	float sofar = 0, max = 0;
	for (curr_iter++; curr_iter != path.end(); curr_iter++)
	{
		Edge *e = graph.GetEdge(*prev_iter, *curr_iter);
		if (e != NULL)
		{
			sofar = e->GetWeight() - e->GetUsedWeight();
			if (sofar > max)
				max = sofar;
		}
		prev_iter = curr_iter;
	}
	
	return max <= 0 ? 1 : max;
}


void FlowMig::WMMCSelect()
{
	vector<string> jam_path_list;
	CheckJam(jam_path_list);

	if (jam_path_list.size() == 0)
		return;

	vector<int> jam_vm_list;
	while (!jam_path_list.empty())
	{
		string dst = jam_path_list.back();
		jam_path_list.pop_back();
		string src = jam_path_list.back();
		jam_path_list.pop_back();

		jam_vm_list.clear();
		GetJamVM(src, dst, jam_vm_list);


		int tmp_cnt = 0;
		while (!jam_vm_list.empty())
		{
			//此处应该有一个排序, 流量大小
			int vm_id = jam_vm_list.back();
			jam_vm_list.pop_back();

			//取得虚拟机
			VirM *vm = pv.GetVM(vm_id);

			int old_pm_id = pv.GetPmByVm(vm->ID());
			PhyM *old_pm  = pv.GetPm(old_pm_id); 
			PhyM *init_pm = old_pm;

			// 依次选取物理机
			PhyM *pm = NULL;
			while ((pm = pv.NextPM()) != NULL)
			{
				old_pm_id = old_pm->GetId();
				if (pm->GetId() == old_pm_id || !pm->CanPut(vm))
					continue;
				
				tmp_cnt++;
				if (tmp_cnt == 18)
					int a = 3;
				// 去除流量
				DelFlowData(vm->ID());

				// 删除虚拟机
				old_pm->RemoveVM(vm->ID());

				// 去除通路
				pathRecord.RemovePath(vm->ID());

				// 放置虚拟机
				pm->Put(vm);
				old_pm = pm;

				// 重新加载流量
				AddFlowData(vm->ID());


				vector<string> jam_path_list_tmp;
				CheckJam(jam_path_list_tmp);

				// 若没有新拥塞
				if (jam_path_list_tmp.size() <= jam_path_list.size() + 2)
				{
				    float cost = static_cast<float>(vm->Memory() + vm->Memory() * TIMES * PERCENT);
					float migCost = cost / GetMinNetWidth(old_pm_id, pm->GetId());
					migCost = migCost / 60;
					//cout << migCost << endl;
					if (migCost < MIGCOST)
					{
						migTime += Do_MigTime(vm->Memory(), old_pm_id, pm->GetId());
						break;
					}
					
				}
			}
			
			//恢复初始状态
			if (pm != NULL)
			{
				while ((pm = pv.NextPM()) != NULL);
			}
			else //迁移失败
			{
				pm = init_pm;

				// 去除流量
				DelFlowData(vm->ID());
				// 删除虚拟机
				old_pm->RemoveVM(vm->ID());

				// 去除通路
				pathRecord.RemovePath(vm->ID());

				// 放置虚拟机
				pm->Put(vm);

				// 重新加载流量
				AddFlowData(vm->ID());
			}
	
			if (IsJam(src, dst) == false)
				break;
	
		}

	}
}

void FlowMig::NetFlowSelect()
{
	vector<string> jam_path_list;
	CheckJam(jam_path_list);

	if (jam_path_list.size() == 0)
		return;

	vector<int> jam_vm_list;
	while (!jam_path_list.empty())
	{
		string dst = jam_path_list.back();
		jam_path_list.pop_back();
		string src = jam_path_list.back();
		jam_path_list.pop_back();

		jam_vm_list.clear();
		GetJamVM(src, dst, jam_vm_list);

		while (!jam_vm_list.empty())
		{

			//此处应该有一个排序, 流量大小
			int vm_id = jam_vm_list.back();
			jam_vm_list.pop_back();

			//取得虚拟机
			VirM *vm = pv.GetVM(vm_id);

			int old_pm_id = pv.GetPmByVm(vm->ID());
			PhyM *old_pm  = pv.GetPm(old_pm_id); 

			// 依次选取物理机
			PhyM *pm = NULL;
			while ((pm = pv.NextPM()) != NULL)
			{
				if (pm->GetId() == old_pm_id || !pm->CanPut(vm))
					continue;


				// 去除流量
				DelFlowData(vm->ID());

				// 删除虚拟机
				old_pm->RemoveVM(vm->ID());

				// 去除通路
				pathRecord.RemovePath(vm->ID());

				// 放置虚拟机
				pm->Put(vm);
				old_pm = pm;

				// 重新加载流量
				AddFlowData(vm->ID());


				vector<string> jam_path_list_tmp;
				CheckJam(jam_path_list_tmp);

				// 若没有新拥塞
				if (jam_path_list_tmp.size() <= jam_path_list.size() + 2)
				{
					migTime += Do_MigTime(vm->Memory(), old_pm_id, pm->GetId());
					break;
				}
			}
			//恢复初始状态
			if (pm != NULL)
				while ((pm = pv.NextPM()) != NULL)
					;

			if (IsJam(src, dst) == false)
				break;
	
		}

	}
}


int FlowMig::JamNum()
{
	vector<string> jam_path_list;
	CheckJam(jam_path_list);

	return jam_path_list.size();
}



void FlowMig::IteratorSelect()
{
	vector<string> jam_path_list;
	CheckJam(jam_path_list);

	if (jam_path_list.size() == 0)
		return;

	vector<int> jam_vm_list;
	while (!jam_path_list.empty())
	{
		string dst = jam_path_list.back();
		jam_path_list.pop_back();
		string src = jam_path_list.back();
		jam_path_list.pop_back();

		jam_vm_list.clear();
		GetJamVM(src, dst, jam_vm_list);

		while (!jam_vm_list.empty())
		{

			//此处应该有一个排序, 流量大小
			int vm_id = jam_vm_list.back();
			jam_vm_list.pop_back();

			VirM *vm = pv.GetVM(vm_id);
			PhyM *pm = GetIteratorPm(vm);
			
			int old_pm_id = pv.GetPmByVm(vm_id);
			PhyM *old_pm  = pv.GetPm(old_pm_id); 

			//if (pm == NULL) 转第二种方法
		
			if (pm != NULL)
			{
				// 去除流量
				DelFlowData(vm->ID());

				// 去除通路
				pathRecord.RemovePath(vm->ID());

				// 删除虚拟机
				old_pm->RemoveVM(vm->ID());

				// 放置虚拟机
				pm->Put(vm);

				// 重新加载流量
				AddFlowData(vm->ID());

				migTime += Do_MigTime(vm->Memory(), old_pm_id, pm->GetId());

			}
			else
			{
				PrintError("NULL Error");
			}

			if (IsJam(src, dst) == false)
				break;
			
		}
	}
}

PhyM *FlowMig::GetIteratorPm(VirM *vm)
{
	PhyM *p = NULL;
	PhyM *target = NULL;
	bool find = false;

	int old_pm = pv.GetPmByVm(vm->ID());
	while ((p = pv.NextPM()) != NULL)
	{
		if (find == false && p->GetId() != old_pm && p->CanPut(vm))
		{
			target = p;
			find = true;
		}
	}
	return target;
}

PhyM *FlowMig::GetRandomPm(VirM *vm)
{
	srand(static_cast<unsigned>(time(NULL)));
	int oldpm = pv.GetPmByVm(vm->ID());

	int cnt = 0;
	int pm_id;
	while (cnt++ < 5)
	{
		pm_id = rand() % pv.GetPmSize();
		PhyM *p = pv.GetPm(pm_id);

		if (p->CanPut(vm) && pm_id != oldpm)
			return p;
	}

	return NULL;
}

bool FlowMig::IsJam(string src, string dst)
{
	Vertex *v = graph.FindVertex(src);
	const Edge *e = NULL;
	const Edge *target = NULL;

	while ((e = v->NextEdge()) != NULL)
	{
		if (e->GetAdjVertexName() == dst)
		{
			target = e;
		}
	}

	if (target != NULL)
	{
		if (target->GetUsedWeight() / target->GetWeight() > JAM_RATIO)
			return true;
	}
	return false;
}

void FlowMig::RandomSelect()
{
	vector<string> jam_path_list;
	CheckJam(jam_path_list);

	if (jam_path_list.size() == 0)
		return;

	vector<int> jam_vm_list;
	while (!jam_path_list.empty())
	{
		string dst = jam_path_list.back();
		jam_path_list.pop_back();
		string src = jam_path_list.back();
		jam_path_list.pop_back();

		jam_vm_list.clear();
		GetJamVM(src, dst, jam_vm_list);

		while (!jam_vm_list.empty())
		{

			//此处应该有一个排序, 流量大小
			int vm_id = jam_vm_list.back();
			jam_vm_list.pop_back();

			VirM *vm = pv.GetVM(vm_id);
			PhyM *pm = GetRandomPm(vm);
			
			int old_pm_id = pv.GetPmByVm(vm_id);
			PhyM *old_pm  = pv.GetPm(old_pm_id); 

			//if (pm == NULL) 转第二种方法
		
			if (pm != NULL)
			{
				// 去除流量
				DelFlowData(vm->ID());

				// 去除通路
				pathRecord.RemovePath(vm->ID());

				// 删除虚拟机
				old_pm->RemoveVM(vm->ID());

				// 放置虚拟机
				pm->Put(vm);

				// 重新加载流量
				AddFlowData(vm->ID());

				migTime += Do_MigTime(vm->Memory(), old_pm_id, pm->GetId());

			}

			if (IsJam(src, dst) == false)
				break;
			
		}
	}
}

float FlowMig::Do_MigTime(float memory, int src_pm_id, int dst_pm_id)
{
	float cost     = static_cast<float>(memory + memory * TIMES * PERCENT);
	float migCost  = cost / GetMinNetWidth(src_pm_id, dst_pm_id);
	migCost = migCost / 60;

	return migCost;
}

float FlowMig::MaxLinkRation()
{
	const Vertex *v;

	float sofar, max = -1;
	while ((v = graph.NextVertex()) != NULL)
	{
		const Edge *e;
		while ((e = const_cast<Vertex*>(v)->NextEdge()) != NULL)
		{
			sofar = e->GetUsedWeight() / e->GetWeight();
			if (sofar > max)
				max = sofar;
		}
	}
	return max;
}

float FlowMig::AverageFlow()
{
	const Vertex *v;

	float used = 0, w = 0;
	while ((v = graph.NextVertex()) != NULL)
	{
		const Edge *e;
		while ((e = const_cast<Vertex*>(v)->NextEdge()) != NULL)
		{
			used += e->GetUsedWeight();
			w    += e->GetWeight();
		}
	}
	
	return used / w;
}