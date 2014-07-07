OBJS=monitor-agent.o connect-controller.o

all:$(OBJS)
	cc -o connect-controller connect-controller.o
	cc -o monitor-agent monitor-agent.o ../core/core_svc.o ../core/core_xdr.o

monitor-agent.o:monitor-agent.c
	cc -o monitor-agent.o -c monitor-agent.c

connect-controller.o:connect-controller.c
	cc -o connect-controller.o -c connect-controller.c

clean:
	rm -rf *.o connect-controller monitor-agent
