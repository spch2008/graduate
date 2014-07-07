
OBJS=flow.o Dijkstra.o Edge.o FlowMig.o Graph.o OrmPV.o \
PathRecord.o PhyM.o Vertex.o VirM.o

all:$(OBJS)
	g++ -o flow $(OBJS)

%.o:%.cpp
	g++ -o $@ -c $<

clean:
	rm -rf flow  *.o

