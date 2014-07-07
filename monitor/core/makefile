OBJS=core_clnt.o core_svc.o core_xdr.o

all:$(OBJS)
	

core_clnt.o:core_clnt.c
	cc -c core_clnt.c -o core_clnt.o

core_svc.o:core_svc.c
	cc -c core_svc.c -o core_svc.o

core_xdr.o:core_xdr.c
	cc -c core_xdr.c -o core_xdr.o

clean:
	rm -rf *.o
