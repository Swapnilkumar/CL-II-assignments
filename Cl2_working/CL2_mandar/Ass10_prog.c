/*
	Assignment 10 ==> Web Server
*/


/*=================================  CALC.CPP  ==================================*/

#include "soapH.h" /* get the gSOAP-generated definitions */
#include "calc.nsmap" /* get the gSOAP-generated namespace bindings */
#include <math.h>  


int main(int argc, char **argv)  
{
	int m, s; /* master and slave sockets */
	struct soap *soap = soap_new();
	if (argc < 2)
		soap_serve(soap); /* serve as CGI application */
	else
	{
		m = soap_bind(soap, NULL, atoi(argv[1]), 100); /* bind to the port supplied as command-line argument */
		if (m < 0)
		{
			soap_print_fault(soap, stderr);
			exit(-1);
		}
		fprintf(stderr, "Socket connection successful: master socket = %d\n", m);
		for (;;)
		{
			s = soap_accept(soap);
			fprintf(stderr, "Socket connection successful: slave socket = %d\n", s);
			if (s < 0)
			{
				soap_print_fault(soap, stderr);
				exit(1);
			} 
			soap_serve(soap);
			soap_end(soap);
		}
	}
	soap_done(soap);
	free(soap);
	return 0;
}

 
int ns__add(struct soap *soap, double a, double b, double *result)  
{
	*result = a + b;  
	return SOAP_OK;  
}  

int ns__sub(struct soap *soap, double a, double b, double *result)  
{
	*result = a - b;  
	return SOAP_OK;  
}  

int ns__sqrt(struct soap *soap, double a, double *result)
{
	if (a >= 0)  
	{
		*result = sqrt(a);  
		return SOAP_OK;  
	}  
	else 
		return soap_sender_fault(soap, "Square root of negative value", "I can only take the square root of non-negative values");    
}




====================================  CLIENT.CPP  ======================================

#include "soapcalcProxy.h"
#include "calc.nsmap"
#include "strings.h"
#include "iostream"
using namespace std;
int main()
{ calc c; /* calc object */
  double n,x,y; /* result value */
  cout<<"Welcome to my calc\nSupported operations\n";	
  cout<<"add x y\n";
  cout<<"sub x y\n";	
  cout<<"sqrt x \n";
  	
    
  while(1)
  {
	char op[5];
	cout<<">>> ";
        cin>>op;
	if(strcmp(op,"sqrt")==0)
	{
		cin>>x;
		if (c.ns__sqrt(x,&n) == SOAP_OK)
 		   cout << "sqrt result = " << n << endl;
  		else
    	   	    soap_print_fault(c.soap, stderr); 

	}
	else if(strcmp(op,"add")==0)
	{
		cin>>x>>y;
		if (c.ns__add(x, y, &n) == SOAP_OK)
 		   cout << "add result = " << n << endl;
  		else
    	   	    soap_print_fault(c.soap, stderr); 
 		
	}
	else if(strcmp(op,"sub")==0)
	{
		cin>>x>>y;
		if (c.ns__sub(x, y, &n) == SOAP_OK)
 		   cout << "sub result = " << n << endl;
  		else
    	   	    soap_print_fault(c.soap, stderr); 

	}
	cin.ignore(50,'\n');
}
  return 0;
}


=================================  OUTPUT  =======================================

oslab@ubuntu:~$ soapcpp2 calc.h
oslab@ubuntu:~$ c++ -o service calc.cpp soapC.cpp soapServer.cpp -lgsoap++
oslab@ubuntu:~$ c++ -o client client.cpp soapC.cpp soapClient.cpp -lgsoap++
oslab@ubuntu:~/Desktop/gsoap$ ./client 
Welcome to my calc
Supported operations
add x y
sub x y
sqrt x 
>>> add 10 20
add result = 30
>>> sqrt 10
sqrt result = 3.16228
>>> sub 30 40
sub result = -10
>>> 
