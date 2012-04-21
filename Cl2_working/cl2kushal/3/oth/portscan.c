/* 
 * internet port scanner 
 *
 * This program will scan a hosts TCP ports printing all ports that accept
 * connections, and if known, the service name.
 * This program can be trivially altered to do UDP ports also.
 *
 * Kopywrong (K) Aug. 25, '94 pluvius@io.org
 *
 * Hey kiddies, this is a C program, to run it do this:
 * $ cc -o pscan pscan.c
 * $ pscan <host> [max port]
 *
 * No, this will not get you root.
 * 
 * Changes:
 * Changed fprintf to printf in line 34 to work with my Linux 1.1.18 box
 * Netrunner 1/18/95 11:30pm
 *
 * Changes:
 * converts port# to network byte order.
 * Therapy 10/29/96 9:00pm
 * 
*/
static char sccsid[] = "@(#)pscan.c     1.0     (KRAD) 08/25/94";
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#define MAX_PORT 1024 /* scan up to this port */
int s;
struct sockaddr_in addr;
char rmt_host[100];

int skan(port)
int port;
{
 int r;
    s = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (s < 0) {
       /* fprintf("ERROR: socket() failed\n"); */
       /* Changed to printf for my Linux 1.1.18 box */
       printf("ERROR: socket() failed\n");
       exit(0);
    }

    addr.sin_family = PF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = inet_addr(rmt_host);

    r = connect(s,(struct sockaddr *) &addr, sizeof(addr));

    close(s);
    if (r==-1) {
       return (1 == 0);
    }

    return (1 == 1);
}

main(argc,argv) 
int argc;
char *argv[];
{
 int a,b,c,d,e,f;
 struct hostent *foo;
 struct servent *bar;

   if (argc < 2) {
      fprintf(stderr,"usage: %s <host> [highest port]\n",argv[0]);
      exit(0);
   }

   if (sscanf(argv[1],"%d.%d.%d.%d",&a,&b,&c,&d) != 4) {
      foo = gethostbyname(argv[1]);
      if (foo == NULL) {
         fprintf(stderr,"error: cannot resolve host %s\n",argv[1]);
         exit(0);
      }
      sprintf(rmt_host,"%d.%d.%d.%d",(unsigned char )foo->h_addr_list[0][0],
              (unsigned char ) foo->h_addr_list[0][1], 
              (unsigned char ) foo->h_addr_list[0][2], 
              (unsigned char ) foo->h_addr_list[0][3]);
   } else {
      strncpy(rmt_host,argv[1],99);
   }


   if (argc > 2) {
      f = atoi(argv[2]);
   } else
      f = MAX_PORT;

   fprintf(stdout,"Scanning host %s - TCP ports 1 through %d\n",rmt_host,f);

   for (e =1;e<=f;e++) {
    char serv[100];
      if (skan(e)) {
         bar = getservbyport(htons(e),"tcp"); 
         printf("%d (%s) is running.\n",e,(bar == NULL) ? "UNKNOWN" :
                bar->s_name); 
      }
   }
}




------------------------------------------------------------------------------------

	Here is another simple portscanner written in PERL

#!/usr/bin/perl
#
# A simple TCP port scanner in perl
# James.Abendschan@nau.edu  27 January 1996
#
# output to stdout, logging to stderr
#
# todo - 
#  better arg handling :)
#  fork() scans?

# ports to scan:
# 21 - ftp
# 23 - telnet
# 25 - smtp
# 79 - finger
# 80 - www
# 119 - nntp
# 139 - netbios (wfwg over tcpip)
# 8000 - occasional www
# 8080 - ocassional www

@myports = (21, 23, 25, 79, 80, 119, 139, 8000, 8080);

require 'sys/socket.ph';
require 'flush.pl';

$SIG{'ALRM'} = 'do_alarm';

if ($ARGV[0] eq "") {
 print "please provide a subnet to scan!  e.g., 134.114.84\n";
 exit 1;
}

{
 $net = $ARGV[0];
 ($a, $b, $c) = split(/\./, $net);
 
  for ($d=0;$d<256;$d++) {
   $host = "$a.$b.$c.$d";

   @portlist = @myports;

   while(@portlist) {
    $port = shift(@portlist);
    print STDERR "Trying $host:$port\n";
    $data = scan($host, $port);
    if (index($data, "FAILED") != 0) {
     @addr = split(/\./, $host);
     $addr = pack(' C4', @addr[0], @addr[1], @addr[2], @addr[3]);
     ($name, $aliases, $type, $len, @addrs) = gethostbyaddr($addr, 2);
     if ($name eq "") {
      $name = $host;
     }
     print STDOUT "$name:$port:$data\n";
     flush(STDOUT);
    }
   }
  }
}

#
# scan (host, port)
# returns error or banner
#

sub scan
{
 
 $hostname = shift @_;
 $serverport = shift @_;

 $connecttimeout = "1";		# time to wait for a reply
 $bannertimeout = "7";		# time to wait for data after a connect

init:
    
   # seed & pick a random port number 
 
   for ($i=0; $i < $$; $i++) { rand(); }

   $clientport = int(rand(32768) + 1024);
   $sockaddr = 'S n a4 x8';
   $locport=pack($sockaddr, &AF_INET, $clientport, "\0\0\0\0");

   if (!socket(C, &PF_INET, &SOCK_STREAM, $proto)) {
    #print "WARNING - couldn't create client socket: $!\n";
    sleep 5;
    goto init;
   }

   if (!bind(C, $locport)) { 
    # die("cannot bind client socket: $!\n");
    #print "WARNING - couldn't bind client socket:$!\n";
    sleep (5);
    goto init;
   }

   ($name, $aliases, $proto) = getprotobyname('tcp');
   ($name, $aliases, $type, $len, $thisaddr) = gethostbyname($hostname);
   ($a,$b,$c,$d) = unpack('C4', $thisaddr);

   $ipaddr="$a.$b.$c.$d";
   
   $thatport = pack($sockaddr, &AF_INET, $serverport, $thisaddr);
   
   alarm(0);
   alarm($connecttimeout);

   if (!connect(C, $thatport)) {
    return "FAILED: $!\n";
   }

   alarm(0);

   select(C); 
   $| = 1;
   select(STDOUT);

   # Now send/rec data to C

   # nudge it..

   print C "\r\n";

   alarm($bannertimeout); 

   $banner = "";
   while ($data = <C>) {
    $banner = "$banner$data" 
   }

   alarm(0);

   if ($banner eq "") {
    $banner = $data;
   }

   shutdown(C, 1);
   close(C);

   $banner =~ tr/\r/\./;
   $banner =~ tr/\n/\./;

   return $banner;
}

#
# Handle timeouts
#

sub do_alarm {
  alarm(0); # reset alarm clock
  $SIG{'ALRM'} = 'do_alarm';
  close (C);
  return "FAILED: timeout";
}





--------------------------------------------------------------------------------

