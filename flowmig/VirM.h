#pragma once
class VirM
{
public:
	VirM(int i, int c, int m);
	~VirM(void);

	bool operator<(const VirM &that) const;
	inline int CPU() const { return cpu; }
	inline int Memory() const { return memory;}
	inline int ID() const { return id; }

private:
	int cpu;
	int memory;
	int id;
};

