#include <unistd.h>
#include <stdio.h>
#include <pthread.h>

void* thfn1()
{
  int i ;
  printf("Started processing thread 1\n");
  
  for(i=0; i<100; i++)
  {
   printf("%d\n",i*2);
   sleep(5);
  }

}
void* thfn2()
{
  int i ;
  printf("Started processing thread 2\n");
  for(i=0; i<100; i++)
  {
   printf("\t%d\n",i*3);
   sleep(10);
  }

}
void* thfn3()
{
  int i ;
  printf("Started processing thread 3\n");
  for(i=0; i<100; i++)
  {
   printf("\t\t%d\n",i*7);
   sleep(1);
  }

}

int main()
{
  pthread_t tid1,tid2,tid3;
  //pthread_attr_t attr1,attr2,attr3;

  printf("Started main thread\n");
  printf("Thread1\tThread2\tThread3\n");
  pthread_create(&tid1,NULL,thfn1,NULL);
  pthread_create(&tid2,NULL,thfn2,NULL);
  pthread_create(&tid3,NULL,thfn3,NULL);

  pthread_join(tid1,NULL);
  pthread_join(tid2,NULL);
  pthread_join(tid3,NULL);
  
}


