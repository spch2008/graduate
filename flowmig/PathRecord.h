#pragma once
#include <map>
#include <vector>
#include <cstring>
#include <sstream>
#include <set>
using namespace std;

struct CMP
{
	bool operator()(const string &lh, const string &rh) const
	{
		if (strcmp(lh.c_str(), rh.c_str()) < 0)
			return true;
		else
			return false;
	}
};

class PathRecord
{
public:
	PathRecord(void);
	~PathRecord(void);


	vector<string> *GetPath(int s_id, int d_id);
	void SetPath(int s_id, int d_id, vector<string> &p);
	void PathVmList(string src, string dst, vector<int> &vm_list);
	void RemovePath(int vm_id);

protected:
	string IntToString(int value);
	int StringToInt(string str);
private:
	map< string, vector<string>, CMP > pathRecord;
};

