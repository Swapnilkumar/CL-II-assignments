
/*
struct input_data {
  double input_data<200>;
};
typedef struct input_data input_data;

program AVERAGEPROG {
    version AVERAGEVERS {
        double AVERAGE(input_data) = 1;
    } = 1;
} = 22855;
*/

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

