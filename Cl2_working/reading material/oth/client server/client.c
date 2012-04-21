/*  vcclient.c --
 TCP network (virtual circuit) client  */

#include &ltstdio.h>
#include &lterrno.h>
#include &ltsys/types.h>
#include &ltsys/socket.h>

#include &ltnetinet/in.h>
   /* sockaddr_in structure */

/* This entry allows the program
to look up the name
   of the host and any alias
   names associated with it.    */

#include &ltnetdb.h>
      /* /etc/hosts table entries */

main (argc, argv)
int argc;
char *argv[];

/* Expected command line parameters:

   argv[0] -- name of executable
   argv[1] --
   the host name to which connection is desired
   argv[2] --
   the port number to be used by the client: the
     value is the port number assigned to the
  server by the server's host system.

This is not elegant, but is very useful for
debugging the connectivity code.  For example,
if the host name ( argv[1] ) were speficied
as "localhost" both client and server
could run on the same system but the network
connectivity code would still be fully exercised.*/

{
   int sock,        /* socket descriptor */
       val,         /* scratch variable */
       cnt;        /* number of bytes I/O */

   struct sockaddr_in myname;
     /* Internet socket name (addr) */
   struct sockaddr_in *nptr;
    /* pointer to get port number */

   char buf[80];
   /* I/O buffer, kind of small  */

 /* For lookup in /etc/hosts file.   */

   struct hostent *hp, *gethostbyaddr();

/* Check that the user supplied all
parameters on the command line.  If so,
convert argv[2] to integer; copy it
into the sin_port field of the myname
structure.  Use the htons function to
insure that the value is stored
in network byte order.     */

   if ( argc < 3 ) {
 printf("network client failure:
        required parameters");
printf(" missing from the command line\n");
printf("network client: usage");
printf("[executable-name] [host name]
    [port number]\n");
      exit(1);
   }

/* As in UNIX domain, create a client
 socket to request service          */

if (( sock = socket(AF_INET, SOCK_STREAM, 0)) < 0 )
{
 printf("network client socket failure %d\n", errno);
      perror("network client");
      exit(2);
   }

/* Convert user-supplied port number to integer.
There is  no consistency check possible
in this set-up.        */

   myname.sin_port = htons( atoi(argv[2]) );
   /* Server port # */

   myname.sin_family = AF_INET;
     /* Internet domain */

/* For display purposes only,
print out the host name and
      the converted port number.         */

printf("network client %s will try
to connect to host %s\n",
          argv[0], argv[1]);
printf("network client %s will use port number %d\n",
          argv[0], ntohs ( myname.sin_port ) );

   /* Obtain server host information.  */

   hp = gethostbyname ( argv[1] );

/* This is mainly debug code; if it becomes
necessary to insert it (or if it just feels
more comfortable to have it in place!!!)
don't remove it when all is working
well.  Rather, leave it in place as a comment.   */

   if ( hp == NULL ) {
 printf("network client gethostbyname failure %d\n",
           errno);
      perror("network client");
      close ( sock );
      exit(3);
   }
   else {
 printf("\nServer information obtained via");
      printf(" gethostbyname:\n");
printf("\tThe official host name is %s\n",
     hp -> h_name);
printf("\tThe address type is %d\n",
       hp -> h_addrtype);
printf("\tThe length of the address is %d bytes\n",
            hp -> h_length);
printf("\tThe first host address is %lx\n",
      ntohl ( * (int * ) hp -> h_addr_list[0] ) );
printf("\tAlias names for the host are:\n");
      while ( *hp -> h_aliases )
printf( "\t\t%s\n", *hp -> h_aliases++ );
   }

/* Use either memcpy or bcopy as appropriate
      (System V vs BSD). */

bcopy ( hp -> h_addr_list[0],
&myname.sin_addr.s_addr, hp -> h_length );
/* memcpy ( &myname.sin_addr.s_addr, hp ->
 h_addr_list[0],hp -> h_length ); */

/* More debug code: Verify the contents of
structure myname  prior to trying to connect
to the (remote) server.    */

printf("\nInformation provided in client's");
   printf(" connect request\n");
   printf("\tRemote host address is %lx\n",
        ntohl ( myname.sin_addr.s_addr ) );
   printf("\tPort number supplied is %d\n",
        ntohs ( myname.sin_port ) );
printf("\tInternet family ID is %d\n",
    myname.sin_family);
printf("\tsin_zero character string is: %s\n",
        myname.sin_zero);

/*Establish socket connection with (remote) server.*/

if ( ( connect ( sock, &myname, sizeof(myname) ) )
 < 0 ) {
 printf("network client %s connect failure %d\n",
              argv[0], errno);
         perror("network client");
         close (sock);
         exit(4);
      }

/*  Exchange data with client.
Clear buffer bytes first. */

/* Take your pick, depending
on your system's pedigree.  The
      System V function has not been tested. */

bzero  ( buf,    sizeof( buf) );
  /* zero buffer, BSD. */
/* memset ( buf, 0, sizeof( buf) );
 /* zero buffer S5. */

strcpy ( buf, "Message from client to server" );
   write ( sock, buf, sizeof(buf) );

/* Now rea
d message sent back by server.    */
if ( ( cnt = read (sock, buf, sizeof(buf) ) ) < 0)
    {
 printf("network client socket read failure &
        d\n", errno);
      perror("network client");
      close(sock);
      exit(5);
   }
   else
 printf("network client received the message %s\n",
      buf);

   /* Now send a message with 0 bytes. */

   bzero  ( buf,    sizeof( buf) );
   /* zero buffer, BSD. */
   /* memset ( buf, 0, sizeof( buf) );
   /* zero buffer S5. */
   write ( sock, buf, 0 );
   close (sock);
   exit(0);

}  /* end of main procedure */