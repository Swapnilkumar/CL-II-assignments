/*
* msg.x: Remote message printing protocol
*/
struct twono{
int no1;
int no2;
};

program IDLEWORK {
version IDLEWORK1{
float getcpu() = 1;
int add(struct twono)=2;
} = 1;
} = 99;

