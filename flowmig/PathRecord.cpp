#include "PathRecord.h"


PathRecord::PathRecord(void)
{
	//测试代码
	/*
	string p("A0A3");
	
	vector<string> v;
	v.push_back("V0");
	v.push_back("V1");
	v.push_back("V2");
	v.push_back("V3");

	pathRecord[p] = v;
	*/
}


PathRecord::~PathRecord(void)
{
}


vector<string> *PathRecord::GetPath(int s_id, int d_id)
{
	string src = IntToString(s_id);
	string dst = IntToString(d_id);
	string s = src.append("#");
	s = s.append(dst);

	if (pathRecord.count(s) == 0)
		return NULL;

	return &pathRecord[s];
}

void PathRecord::SetPath(int s_id, int d_id, vector<string> &p)
{
	string src = IntToString(s_id);
	string dst = IntToString(d_id);
	string s = src.append("#");
	s = s.append(dst);

	pathRecord[s] = p;
}

string PathRecord::IntToString(int value)
{
	stringstream s;  
	s << value;
	return s.str();
}

int PathRecord::StringToInt(string str)
{
	stringstream s(str);

	int n = 0;
	s >> n;
	return n;
}

//走某条链路下的虚拟机
void PathRecord::PathVmList(string src, string dst, vector<int> &vm_list)
{
	bool state = false;

	set<int> s;

	map< string, vector<string>, CMP >::iterator iter;
	for (iter = pathRecord.begin(); iter != pathRecord.end(); iter++)
	{
		vector<string> &v = iter->second;
		vector<string>::iterator pathIter;
		for (pathIter = v.begin(); pathIter != v.end(); pathIter++)
		{
			if (*pathIter == src && state == false)
			{
				state = true;
			}
			else if (*pathIter == dst && state == true)
			{
				string target = iter->first;
				int pos = target.find("#", 1);
				int vm_id = StringToInt(target.substr(0, pos));
				//vm_list.push_back(vm_id);
				s.insert(vm_id);
			}
			else
			{
				state = false;
			}
		}
	}

	set<int>::iterator s_iter;
	for (s_iter = s.begin(); s_iter != s.end(); s_iter++)
		vm_list.push_back(*s_iter);
}

void PathRecord::RemovePath(int vm_id)
{
	string vm_s = IntToString(vm_id);

	bool find = false;
	map< string, vector<string>, CMP>::iterator iter;
	map< string, vector<string>, CMP>::iterator oldIter;
	for (iter = pathRecord.begin(); iter != pathRecord.end(); )
	{
		if (iter->first.find(vm_s) != string::npos)
		{
			//迭代器是否失效,hui
			oldIter = iter;
			find = true;
		}
		iter++;
		if (find)
		{
			find = false;
			pathRecord.erase(oldIter);
		}
	}
}