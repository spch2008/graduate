// Á÷Á¿Ç¨ÒÆ.cpp : Defines the entry point for the console application.
//

#include <cstdlib>
#include "FlowMig.h"


void PrintInfo(FlowMig &flowMig)
{
	cout << "max link ratio:" << flowMig.MaxLinkRation() << endl;
	cout << "average link ratio:" << flowMig.AverageFlow() << endl;
	cout << "jav num:" << flowMig.JamNum() << endl;
	cout << "migation time:" << flowMig.MigTime() << endl;
	cout << endl;
}

void Random_Method()
{
	FlowMig flowMig;

	flowMig.RemoveJam(RANDOM);

	PrintInfo(flowMig);
}

void NF_Method()
{
	FlowMig flowMig;
	flowMig.RemoveJam(NF);
	PrintInfo(flowMig);
}

void STH_Method()
{
	FlowMig flowMig;
	flowMig.RemoveJam(STH);
	PrintInfo(flowMig);
}

void WMMC_Method()
{
	FlowMig flowMig;
	flowMig.RemoveJam(WMMC);
	PrintInfo(flowMig);
}


void PrintInit()
{
	FlowMig flowMig;
	cout <<"max link ratio:" << flowMig.MaxLinkRation() << endl;
	cout <<"average link ratio:" << flowMig.AverageFlow() << endl;
	cout <<"jam num:" << flowMig.JamNum() << endl;

	cout << endl;
}

int main(int argc, char* argv[])
{
        cout << "init state" << endl;
	PrintInit();

        cout << "===================" << endl;
        cout << endl;

        cout << "after" << endl;
//	Random_Method();
//	NF_Method();
//	STH_Method();
	WMMC_Method();

	return 0;
}

