/*RPCGEN file*/

struct record
{
	int rollno;
	char name[32];
	float marks; 
};

typedef struct record user_record;

program  DBPROG {
    version DBPROG1 {
        int insert(user_record) = 99;
        int delete(int rollno)=9;
        int update(user_record)=34;
        user_record retrive(int rollno)=6;
    } = 1;
} = 22855;


