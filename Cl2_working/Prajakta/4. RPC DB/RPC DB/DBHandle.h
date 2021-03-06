/*
 * Please do not edit this file.
 * It was generated using rpcgen.
 */

#ifndef _DBHANDLE_H_RPCGEN
#define _DBHANDLE_H_RPCGEN

#include <rpc/rpc.h>


#ifdef __cplusplus
extern "C" {
#endif


struct record {
	int rollno;
	char name[32];
	float marks;
};
typedef struct record record;

typedef record user_record;

#define DBPROG 22855
#define DBPROG1 1

#if defined(__STDC__) || defined(__cplusplus)
#define insert 99
extern  int * insert_1(user_record *, CLIENT *);
extern  int * insert_1_svc(user_record *, struct svc_req *);
#define delete 9
extern  int * delete_1(int *, CLIENT *);
extern  int * delete_1_svc(int *, struct svc_req *);
#define update 34
extern  int * update_1(user_record *, CLIENT *);
extern  int * update_1_svc(user_record *, struct svc_req *);
#define retrive 6
extern  user_record * retrive_1(int *, CLIENT *);
extern  user_record * retrive_1_svc(int *, struct svc_req *);
extern int dbprog_1_freeresult (SVCXPRT *, xdrproc_t, caddr_t);

#else /* K&R C */
#define insert 99
extern  int * insert_1();
extern  int * insert_1_svc();
#define delete 9
extern  int * delete_1();
extern  int * delete_1_svc();
#define update 34
extern  int * update_1();
extern  int * update_1_svc();
#define retrive 6
extern  user_record * retrive_1();
extern  user_record * retrive_1_svc();
extern int dbprog_1_freeresult ();
#endif /* K&R C */

/* the xdr functions */

#if defined(__STDC__) || defined(__cplusplus)
extern  bool_t xdr_record (XDR *, record*);
extern  bool_t xdr_user_record (XDR *, user_record*);

#else /* K&R C */
extern bool_t xdr_record ();
extern bool_t xdr_user_record ();

#endif /* K&R C */

#ifdef __cplusplus
}
#endif

#endif /* !_DBHANDLE_H_RPCGEN */
