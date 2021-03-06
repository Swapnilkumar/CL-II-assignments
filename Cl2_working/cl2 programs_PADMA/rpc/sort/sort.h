/*
 * Please do not edit this file.
 * It was generated using rpcgen.
 */

#ifndef _SORT_H_RPCGEN
#define _SORT_H_RPCGEN

#include <rpc/rpc.h>


#ifdef __cplusplus
extern "C" {
#endif

#define MAXAVGSIZE 200

struct input_data {
	struct {
		u_int input_data_len;
		double *input_data_val;
	} input_data;
};
typedef struct input_data input_data;


#define SORTPROG 22855
#define SORTVERS 1

#if defined(__STDC__) || defined(__cplusplus)
#define SORT 1
extern  input_data * sort_1(input_data *, CLIENT *);
extern  input_data * sort_1_svc(input_data *, struct svc_req *);
extern int sortprog_1_freeresult (SVCXPRT *, xdrproc_t, caddr_t);

#else /* K&R C */
#define SORT 1
extern  input_data * sort_1();
extern  input_data * sort_1_svc();
extern int sortprog_1_freeresult ();
#endif /* K&R C */

/* the xdr functions */

#if defined(__STDC__) || defined(__cplusplus)
extern  bool_t xdr_input_data (XDR *, input_data*);
extern  bool_t xdr_input_data (XDR *, input_data*);

#else /* K&R C */
extern bool_t xdr_input_data ();
extern bool_t xdr_input_data ();

#endif /* K&R C */

#ifdef __cplusplus
}
#endif

#endif /* !_SORT_H_RPCGEN */
