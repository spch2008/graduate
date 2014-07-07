/*
 * Please do not edit this file.
 * It was generated using rpcgen.
 */

#include <memory.h> /* for memset */
#include "core.h"

/* Default timeout can be changed using clnt_control() */
static struct timeval TIMEOUT = { 25, 0 };

pm_info *
getpminfo_1(info_type *argp, CLIENT *clnt)
{
	static pm_info clnt_res;

	memset((char *)&clnt_res, 0, sizeof(clnt_res));
	if (clnt_call (clnt, GETPMINFO,
		(xdrproc_t) xdr_info_type, (caddr_t) argp,
		(xdrproc_t) xdr_pm_info, (caddr_t) &clnt_res,
		TIMEOUT) != RPC_SUCCESS) {
		return (NULL);
	}
	return (&clnt_res);
}