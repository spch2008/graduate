OBJS=monitor-controller.o thread_pool.o single_list.o

all:$(OBJS)
	cc -o monitor-controller monitor-controller.o single_list.o thread_pool.o ../core/core_xdr.o ../core/core_clnt.o  -pthread

monitor-controller.o:monitor-controller.c
	cc -c monitor-controller.c -o monitor-controller.o

single_list.o:single_list.c
	cc -c single_list.c -o single_list.o

thread_pool.o:thread_pool.c
	cc -c thread_pool.c  -o thread_pool.o

clean:
	rm -rf monitor-controller *.o
