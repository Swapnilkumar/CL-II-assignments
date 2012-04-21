//WAP to simulate the functionality of Lamport's Logical clock in C.

#include

#include

long p1(int);

long p2(int);

long p3(int);

long p4(int);

void main()

{

int k;

clrscr();

printf("Enter process no.");

scanf("%d",&k);

while(!kbhit())

{

if(k==1)

p1(1);

if(k==2)

p2(1);

if(k==3)

p3(1);

if(k==4)

p4(1);

}

getch();

printf("\n Logical Clock\n");

printf("P1:%ld\nP2:%ld\nP3:%ld\nP4:%ld\n",p1(0),p2(0),p3(0),p4(0));

getch();

}

long p1(int i)

{

static long a=0;

if(i==1)

{

a++;

p2(1);

return 1;

}

else

return a;

}

long p2(int i)

{

static long b=0;

if(i==1)

{

b++;

p3(1);

p4(1);

return 1;

}

else

return b;

}

long p3(int i)

{

static long c=0;

if(i==1)

{

c++;

return 1;

}

else

return c;

}

long p4(int i)

{

static long d=0;

if(i==1)

{

d++;

p3(1);

return 1;

}

else

return d;

}