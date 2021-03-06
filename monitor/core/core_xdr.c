/*
 * Please do not edit this file.
 * It was generated using rpcgen.
 */

#include "core.h"

bool_t
xdr_pm_info (XDR *xdrs, pm_info *objp)
{
	register int32_t *buf;

	 if (!xdr_pointer (xdrs, (char **)&objp->ip, sizeof (char), (xdrproc_t) xdr_char))
		 return FALSE;
	 if (!xdr_float (xdrs, &objp->memory_ratio))
		 return FALSE;
	 if (!xdr_float (xdrs, &objp->cpu_ration))
		 return FALSE;
	 if (!xdr_pointer (xdrs, (char **)&objp->vm_flow_info, sizeof (char), (xdrproc_t) xdr_char))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_info_type (XDR *xdrs, info_type *objp)
{
	register int32_t *buf;

	 if (!xdr_enum (xdrs, (enum_t *) objp))
		 return FALSE;
	return TRUE;
}
