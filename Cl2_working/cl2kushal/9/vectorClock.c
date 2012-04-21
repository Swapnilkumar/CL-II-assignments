//WAP to Implement Vector clock in C.

#include

#include

#include

#include

long *p1(int i,long *comp);

long *p2(int i,long *comp);

long *p3(int i,long *comp);

void main()

{

long start[]={0,0,0},*vector;

clrscr();

while(!kbhit())

{

p1(1,&start[0]);

}

printf("\n Process Vector\n");

vector=p1(0,&start[0]);

printf("p1[%ld%ld%ld]\n",*vector,*(vector+1),*(vector+2));

vector=p2(0,&start[0]);

printf("p2[%ld%ld%ld]\n",*vector,*(vector+1),*(vector+2));

vector=p3(0,&start[0]);

printf("p3[%ld%ld%ld]\n",*vector,*(vector+1),*(vector+2));

}

long *p1(int i,long *comp)

{

static long a[]={0,0,0};

int next;

if(i==1)

{

a[0]++;

if(*(comp+1)>a[1])

a[1]=*(comp+1);

if(*(comp+2)>a[2])

a[2]=*(comp+2);

next=random(2);

if(next==0)

p2(1,&a[0]);

else if(next==1)

p3(1,&a[0]);

return(&a[0]);

}

else

return(&a[0]);

}

long *p2(int i,long *comp)

{

static long b[]={0,0,0};

int next;

if(i==1)

{

b[i]++;

if(*comp>b[0])

b[0]=*(comp);

if(*(comp+2)>b[2])

b[2]=*(comp+2);

next=random(2);

if(next==0)

p1(1,&b[0]);

else if(next==1)

p3(1,&b[0]);

return &b[0];

}

else

return &b[0];

}

long *p3(int i,long *comp)

{

static long c[]={0,0,0};

int next;

if(i==1)

{

c[2]++;

if(*comp>c[0])

c[0]=*(comp);

if(*(comp+1)>c[1])

c[1]=*(comp+1);

next=random(2);

if(next==0)

p1(1,&c[0]);

return &c[0];

}

else

return &c[0];

}