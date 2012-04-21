/*  vcserver.c --
TCP network (virtual circuit)
server   */

#include &ltstdio.h>
#include &lterrno.h>
#include &ltsys/types.h>
#include &ltsys/socket.h>
#include &ltnetinet/in.h>
    /* sockaddr_in structure */

/* This entry allows the program
 to look up the name
   of the host and any alias
   names associated with it. */

#include &ltnetdb.h>
   /* /etc/hosts table entries */

main (int argc, char *argv[])

{
   int rc,
    /* system call return code */
       new_sd, sock,
/* server/listen socket descriptors */
       adrlen,
        /* sockaddr length */
       cnt;
         /* number of bytes I/O */

   struct sockaddr_in myname;
   /* Internet socket name */
   struct sockaddr_in *nptr;
    /* ptr to get port number */
   struct sockaddr    addr;
    /* generic socket name */

   char buf[80];
    /* I/O buffer, kind of small  */

   /* For lookup in /etc/hosts file.   */

   struct hostent *hp, *gethostbyaddr();

   /* Identify the server process. */

   printf("\nThis is the network
   server with pid %d\n",  getpid() );

 /* As in UNIX domain sockets,
 create a "listen" socket */

 if (( sock = socket(AF_INET,
 SOCK_STREAM, 0)) < 0 ) {
 printf("network server socket failure %d\n",
      errno);
      perror("network server");
      exit(1);
 /* Initialize the fields in the
 Internet socket name   structure.   */

   myname.sin_family = AF_INET;
   /* Internet address */
   myname.sin_port = 0;
   /* System will assign port #  */
   myname.sin_addr.s_addr = INADDR_ANY;
    /* "Wildcard" */

/* Bind the Internet address to
the Internet socket */

if (bind(sock, &myname, sizeof(myname) ) < 0 )
{
      close(sock);  /* defensive programming  */
 printf("network server bind failure %d\n", errno);
      perror("network server");
      exit(2);
   }

/*Get the port number assigned to the Internet
socket.getsockname() obtains the port number
ssociated with the bound socket and returns it
as part of the information in the sockaddr addr
structure.  Note that, since the port number is
not passed directly by this program to any client,
the only way to "advertise" it is to print it,
that is, send it to the user's stdout.  Other
than this printout, this code is not intrinsic to
the connectivity process.
	*/

   adrlen = sizeof(addr);
    /* need int for return value */
if ( ( rc = getsockname
( sock, &addr, &adrlen ) ) < 0 )
	{
printf("setwork server getsockname failure %d\n",
				errno);
      perror("network server");
      close (sock);
      exit(3);
   }

 /*DEBUG CODE: the generic address "addr" is used to
return the socket value obtained from the
getsockname() call.  Print this information.  In the
generic structure definition, all but the address
family is defined as a char string.  After this
call, the generic address structure addr is used to
hold information about the client process. */

   printf("\nAfter getsockname():");
   printf(" server listen socket data\n");
   printf("\taddr.sa_family field value is: %d\n",
        addr.sa_family);
   printf("\taddr.sa_data string is %d bytes long;\n",
        sizeof ( addr.sa_data ) );
   printf("\taddr.sa_data string is:");
for ( cnt = 0: cnt < sizeof (addr.sa_data); cnt++)
        printf(" %x", addr.sa_data[cnt]);
   printf("\n");

/*Now "advertise" the port number assigned to the
socket.  In this example, this port number must be
used as the second command line parameter when
starting up the client process.  */

/*Note the use of the pointer nptr
, with a different
mapping of the allocated memory, to point
to the generic address structure. */

   nptr = (struct sockaddr_in *) &addr;
    /* port # */
printf("\n\tnetwork server: server has port number:
       %d\n", ntohs ( nptr -> sin_port ) );

/* Mark the socket as a "listen-only"or passive socket*/

   if ( listen ( sock, 5 ) < 0 ) {
printf("network server bind failure %d\n", errno);
      perror("network server");
      close (sock);
      exit(4);
   }

 /* Debug output: information
 contained in myname structure
      (the Internet socket).                      */

printf("Server has set up client socket with values:\n");
printf("\tInternet address is %lx\n",
 myname.sin_addr.s_addr);
printf("\tPort number used is %d\n", myname.sin_port);
printf("\tInternet family ID is %d\n", myname.sin_family);
printf("\tValues are filled in after connection request ");
 printf("is accepted.");

/* Set up "infinite loop" to listen for clients.Since the
structure "myname" is bound to the listen socket, the
socket structure name and socket length parameter
values are omitted from the accept call.  The bound values
are used.                                           */

   while (1) {
if ( ( new_sd = accept ( sock, 0, 0 ) ) < 0 ) {
 printf("network server accept failure %d\n", errno);
         perror("network server");
         close (sock);
         exit(5);
      }

/*Fork child process to handle client service request*/

      if ( ( fork() ) == 0 ) {
       /* Child process */

         int pid;

         pid = getpid();
           /* PID of child process */
         close (sock);
      /* Do not need listen socket in child. */


 /* Find out who the client is.  Note the use of the
 generic address structure addr to hold information
 about the (connected) client.                    */

if ((rc = getpeername
( new_sd, &addr, &adrlen )) < 0) {
 printf("network server %d getpeername failure %d\n",
                 pid, errno);
            perror("network server");
            close(new_sd);
            exit(6);
         }
 /* Just for grins, "announce" the client.  Note that,
 since pointer nptr is of type struct sockaddr_in,
the field names as defined in the structure template
sockaddr_in can be used to access values in the addr
generic structure.                               */

         printf("\n\tnetwork server %d:", pid);
         printf(" client socket from host %s\n",
              inet_ntoa ( nptr -> sin_addr ) );
printf("\t  has port number %d\n",nptr -> sin_port);

/* Now find all names associated with the client; this
 is the reason for the /etc/hosts file lookup
 declarations.   */

if (( hp = gethostbyaddr
 (&nptr -> sin_addr,4,AF_INET))
                  != NULL ) {
printf ("\tfrom hostname: %s\n\twith aliases: ",
                   hp -> h_name );
            while ( *hp -> h_aliases )
   printf ("\n\t\t\t%s", *hp -> h_aliases++ );
            printf("\n\n");
         }
         else {
            printf("network server %d ", pid);
  printf("gethostbyaddr failure %d\n", h_errno);
            perror("network server");
         }

 /*Exchange data with client.  Clear buffer first*/

         do {

 /* Take your pick, depending on system pedigree.
  The System V function has not been tested as of
   this edition.                               */

            bzero( buf, sizeof(buf));
            /* zero buf, BSD call. */
            /* memset (buf,0,sizeof(buf));
             /* zero buf, S5. */

/*Read message from remote client; if message
 length   = 0, quit. */

if(( cnt = read (new_sd, buf, sizeof(buf))) <0)
{
             printf("network server %d ", pid);
 printf("socket read failure &d\n", errno);
               perror("network server");
               close(new_sd);
               exit(7);
            }
            else
            if (cnt == 0) {
 printf("network server received message");
               printf(" of length %d\n", cnt);
               printf("network server closing");
               printf(" client connection...\n");
               close (new_sd);
               continue; /* break out of loop */
            }
            else {

/* Print out message received from client. Send
                   a message back.        */

 printf("network server %d received message",pid);
               printf(" of length %d\n", cnt);
 printf("network server %d  received", pid));
               printf(" the message %s\n", buf);
  bzero (buf, sizeof(buf)); /* zero buf, BSD. */
  /* memset(buf,0,sizeof(buf)); /* zero buf, S5. */
   strcpy(buf, "Message from server to client");
               write (new_sd, buf, sizeof(buf));
            }  /* end of message-print else */

         }  /* end of do loop statement */

         while (cnt != 0);  /* do loop condition */
         exit(0);
          /* Exit child process */

      }
   /* End of if-child-process true condition */

      else
 /* Not child process; must be parent process */

         close (new_sd);
       /* Parent doesn't need work socket. */

   }  /* end of while (1) */

}  /* end of main procedure */
