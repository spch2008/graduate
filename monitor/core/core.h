/*
 * Please do not edit this file.
 * It was generated using rpcgen.
 */

#ifndef _CORE_H_RPCGEN
#define _CORE_H_RPCGEN

#include <rpc/rpc.h>


#ifdef __cplusplus
extern "C" {
#endif


struct pm_info {
	char *ip;
	float memory_ratio;
	float cpu_ration;
	char *vm_flow_info;
};
typedef struct pm_info pm_info;

enum info_type {
	Memory_Only = 0,
	Cpu_Only = 1,
	VmFlow_Only = 2,
	Memory_Cpu = 3,
	All = 4,
};
typedef enum info_type info_type;

#define MONITOR_PROG 0x31230000
#define MONITOR_VERS 1

#if defined(__STDC__) || defined(__cplusplus)
#define GETPMINFO 1
extern  pm_info * getpminfo_1(info_type *, CLIENT *);
extern  pm_info * getpminfo_1_svc(info_type *, struct svc_req *);
extern int monitor_prog_1_freeresult (SVCXPRT *, xdrproc_t, caddr_t);

#else /* K&R C */
#define GETPMINFO 1
extern  pm_info * getpminfo_1();
extern  pm_info * getpminfo_1_svc();
extern int monitor_prog_1_freeresult ();
#endif /* K&R C */

/* the xdr functions */

#if defined(__STDC__) || defined(__cplusplus)
extern  bool_t xdr_pm_info (XDR *, pm_info*);
extern  bool_t xdr_info_type (XDR *, info_type*);

#else /* K&R C */
extern bool_t xdr_pm_info ();
extern bool_t xdr_info_type ();

#endif /* K&R C */

#ifdef __cplusplus
}
#endif

#endif /* !_CORE_H_RPCGEN */
