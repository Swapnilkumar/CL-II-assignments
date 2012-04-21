/*
 * mpstat: per-processor statistics
 * (C) 2000-2002 by Sebastien GODARD <sebastien.godard@wanadoo.fr>
 *
 ***************************************************************************
 * This program is free software; you can redistribute it and/or modify it *
 * under the terms of the GNU General Public License as published  by  the *
 * Free Software Foundation; either version 2 of the License, or (at  your *
 * option) any later version.                                              *
 *                                                                         *
 * This program is distributed in the hope that it  will  be  useful,  but *
 * WITHOUT ANY WARRANTY; without the implied warranty  of  MERCHANTABILITY *
 * or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License *
 * for more details.                                                       *
 *                                                                         *
 * You should have received a copy of the GNU General Public License along *
 * with this program; if not, write to the Free Software Foundation, Inc., *
 * 675 Mass Ave, Cambridge, MA 02139, USA.                                 *
 ***************************************************************************
 */


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>
#include <ctype.h>
#include <sys/utsname.h>
#include <sys/param.h>	/* for HZ */

#include "version.h"
#include "mpstat.h"
#include "common.h"


#ifdef USE_NLS
#include <locale.h>
#include <libintl.h>
#define _(string) gettext(string)
#else
#define _(string) (string)
#endif


struct mp_stats *st_mp_cpu[DIM];
/* NOTE: Use array of _char_ for bitmaps to avoid endianness problems...*/
unsigned char *cpu_bitmap;	/* Bit 0: Global; Bit 1: 1st proc; etc. */
struct mp_timestamp st_mp_tstamp[DIM];
int proc_used = -1;  /* Nb of processors on the machine. A value of 1 means two processors */
long interval = 0, count = 0;
unsigned int flags = 0;
struct tm loc_time;


/*
 * Print usage and exit
 */
void usage(char *progname)
{
   fprintf(stderr, _("sysstat version %s\n"
		   "(C) S. Godard <sebastien.godard@wanadoo.fr>\n"
	           "Usage: %s [ options... ]\n"
		   "Options are:\n"
		   "[ -P { <cpu> | ALL } ] [ -V ]\n"
		   "[ <interval> [ <count> ] ]\n"),
	   VERSION, progname);
   exit(1);
}


/*
 * SIGALRM signal handler
 */
void alarm_handler(int sig)
{
   signal(SIGALRM, alarm_handler);
   alarm(interval);
}


/*
 * Allocate mp_stats structures and cpu bitmap
 */
void salloc_mp_cpu(int nr_cpus)
{
   int i;

   for (i = 0; i < DIM; i++) {
      if ((st_mp_cpu[i] = (struct mp_stats *) malloc(MP_STATS_SIZE * nr_cpus)) == NULL) {
	 perror("malloc");
	 exit(4);
      }

      memset(st_mp_cpu[i], 0, MP_STATS_SIZE * nr_cpus);
   }

   if ((cpu_bitmap = (unsigned char *) malloc((nr_cpus >> 3) + 1)) == NULL) {
      perror("malloc");
      exit(4);
   }

   memset(cpu_bitmap, 0, (nr_cpus >> 3) + 1);
}


/*
 * Print statistics average
 */
void write_stats_avg(short curr, short dis)
{
   struct mp_stats *st_mp_cpu_i, *st_mp_cpu_j;
   unsigned long itv;
   int cpu;

   /* Interval value in jiffies, multiplied by the number of proc */
   itv = st_mp_tstamp[curr].uptime - st_mp_tstamp[2].uptime;

   if (!itv)	/* Paranoia checking */
      itv = 1;

   /* Print stats */
   if (dis)
      printf(_("\nAverage:     CPU   %%user   %%nice %%system   %%idle    intr/s\n"));

   /* Check if we want global average stats among all proc */
   if (*cpu_bitmap & 1) {

      printf(_("Average:     all"));

      printf("  %6.2f  %6.2f  %6.2f",
	     SP_VALUE(st_mp_cpu[2]->cpu_user,   st_mp_cpu[curr]->cpu_user,   itv),
	     SP_VALUE(st_mp_cpu[2]->cpu_nice,   st_mp_cpu[curr]->cpu_nice,   itv),
	     SP_VALUE(st_mp_cpu[2]->cpu_system, st_mp_cpu[curr]->cpu_system, itv));

      if (st_mp_cpu[curr]->cpu_idle < st_mp_cpu[2]->cpu_idle)	/* Handle buggy RTC (or kernels?) */
	 printf("    %.2f", 0.0);
      else
	 printf("  %6.2f",
		SP_VALUE(st_mp_cpu[2]->cpu_idle, st_mp_cpu[curr]->cpu_idle, itv));
   }

   /* Here, we reduce the interval value to one processor! */
   itv /= (proc_used + 1);
   if (!itv)
      itv = 1;

   if (*cpu_bitmap & 1) {
      printf(" %9.2f\n",
	  S_VALUE(st_mp_cpu[2]->irq, st_mp_cpu[curr]->irq, itv));
   }

   for (cpu = 1; cpu <= proc_used + 1; cpu++) {

      /* Check if we want stats about this proc */
      if (!(*(cpu_bitmap + (cpu >> 3)) & (1 << (cpu & 0x07))))
	 continue;

      printf(_("Average:    %4d"), cpu - 1);

      st_mp_cpu_i = st_mp_cpu[curr]  + cpu;
      st_mp_cpu_j = st_mp_cpu[2] + cpu;

      printf("  %6.2f  %6.2f  %6.2f",
	     SP_VALUE(st_mp_cpu_j->cpu_user,   st_mp_cpu_i->cpu_user,   itv),
	     SP_VALUE(st_mp_cpu_j->cpu_nice,   st_mp_cpu_i->cpu_nice,   itv),
	     SP_VALUE(st_mp_cpu_j->cpu_system, st_mp_cpu_i->cpu_system, itv));

      if (st_mp_cpu_i->cpu_idle < st_mp_cpu_j->cpu_idle)	/* Handle buggy RTC (or kernels?) */
	 printf("    %.2f", 0.0);
      else
	 printf("  %6.2f",
		SP_VALUE(st_mp_cpu_j->cpu_idle, st_mp_cpu_i->cpu_idle, itv));

      printf(" %9.2f\n",
	     S_VALUE(st_mp_cpu_j->irq, st_mp_cpu_i->irq, itv));
   }
}


/*
 * Print statistics
 */
void write_stats(short curr, short dis)
{
   char cur_time[2][14];
   struct mp_stats *st_mp_cpu_i, *st_mp_cpu_j;
   unsigned long itv;
   int cpu;

   /*
    * Get previous timestamp
    * NOTE: loc_time structure must have been init'ed before!
    */
   loc_time.tm_hour = st_mp_tstamp[!curr].hour;
   loc_time.tm_min  = st_mp_tstamp[!curr].minute;
   loc_time.tm_sec  = st_mp_tstamp[!curr].second;
   strftime(cur_time[!curr], 14, "%X  ", &loc_time);

   /* Get current timestamp */
   loc_time.tm_hour = st_mp_tstamp[curr].hour;
   loc_time.tm_min  = st_mp_tstamp[curr].minute;
   loc_time.tm_sec  = st_mp_tstamp[curr].second;
   strftime(cur_time[curr], 14, "%X  ", &loc_time);

   /* Only the first 11 characters are printed */
   cur_time[curr][11] = cur_time[!curr][11] = '\0';

   /* Interval value in jiffies, multiplied by the number of proc */
   itv = st_mp_tstamp[curr].uptime - st_mp_tstamp[!curr].uptime;

   if (!itv)	/* Paranoia checking */
      itv = 1;

   /* Print stats */
   if (dis)
      printf(_("\n%-11s  CPU   %%user   %%nice %%system   %%idle    intr/s\n"), cur_time[!curr]);

   /* Check if we want global stats among all proc */
   if (*cpu_bitmap & 1) {

      printf(_("%-11s  all"), cur_time[curr]);

      printf("  %6.2f  %6.2f  %6.2f",
	     SP_VALUE(st_mp_cpu[!curr]->cpu_user,   st_mp_cpu[curr]->cpu_user,   itv),
	     SP_VALUE(st_mp_cpu[!curr]->cpu_nice,   st_mp_cpu[curr]->cpu_nice,   itv),
	     SP_VALUE(st_mp_cpu[!curr]->cpu_system, st_mp_cpu[curr]->cpu_system, itv));

      if (st_mp_cpu[curr]->cpu_idle < st_mp_cpu[!curr]->cpu_idle)	/* Handle buggy RTC (or kernels?) */
	 printf("    %.2f", 0.0);
      else
	 printf("  %6.2f",
		SP_VALUE(st_mp_cpu[!curr]->cpu_idle, st_mp_cpu[curr]->cpu_idle, itv));
   }

   /* Here, we reduce the interval value to one processor! */
   itv /= (proc_used + 1);
   if (!itv)
      itv = 1;

   if (*cpu_bitmap & 1) {
      printf(" %9.2f\n",
	  S_VALUE(st_mp_cpu[!curr]->irq, st_mp_cpu[curr]->irq, itv));
   }

   for (cpu = 1; cpu <= proc_used + 1; cpu++) {

      /* Check if we want stats about this proc */
      if (!(*(cpu_bitmap + (cpu >> 3)) & (1 << (cpu & 0x07))))
	 continue;

      printf("%-11s %4d", cur_time[curr], cpu - 1);

      st_mp_cpu_i = st_mp_cpu[curr]  + cpu;
      st_mp_cpu_j = st_mp_cpu[!curr] + cpu;

      printf("  %6.2f  %6.2f  %6.2f",
	     SP_VALUE(st_mp_cpu_j->cpu_user,   st_mp_cpu_i->cpu_user,   itv),
	     SP_VALUE(st_mp_cpu_j->cpu_nice,   st_mp_cpu_i->cpu_nice,   itv),
	     SP_VALUE(st_mp_cpu_j->cpu_system, st_mp_cpu_i->cpu_system, itv));

      if (st_mp_cpu_i->cpu_idle < st_mp_cpu_j->cpu_idle)	/* Handle buggy RTC (or kernels?) */
	 printf("    %.2f", 0.0);
      else
	 printf("  %6.2f",
		SP_VALUE(st_mp_cpu_j->cpu_idle, st_mp_cpu_i->cpu_idle, itv));

      printf(" %9.2f\n",
	  S_VALUE(st_mp_cpu_j->irq, st_mp_cpu_i->irq, itv));
   }
}


/*
 * Read stats from /proc/stat
 * (see linux source file linux/fs/proc/array.c)
 */
void read_proc_stat(short curr)
{
   FILE *statfp;
   struct mp_stats *st_mp_cpu_i;
   static char line[80];
   unsigned int cc_user, cc_nice, cc_system;
   unsigned long cc_idle;
   int proc_nb;

   /* Open stat file */
   if ((statfp = fopen(STAT, "r")) == NULL) {
      fprintf(stderr, _("Cannot open %s: %s\n"), STAT, strerror(errno));
      exit(2);
   }

   while (fgets(line, 80, statfp) != NULL) {

      if (!strncmp(line, "cpu ", 4)) {
	 /*
	  * Read the number of jiffies spent in user, nice, system and idle mode among all proc.
	  * CPU usage is not reduced to one processor to avoid rounding problems.
	  */
	 sscanf(line + 5, "%u %u %u %lu",
		&(st_mp_cpu[curr]->cpu_user),   &(st_mp_cpu[curr]->cpu_nice),
		&(st_mp_cpu[curr]->cpu_system), &(st_mp_cpu[curr]->cpu_idle));

	 /*
	  * Compute the uptime of the system in jiffies (1/100ths of a second if HZ=100).
	  * Machine uptime is multiplied by the number of processors here.
	  */
	 st_mp_tstamp[curr].uptime = st_mp_cpu[curr]->cpu_user   + st_mp_cpu[curr]->cpu_nice +
	                             st_mp_cpu[curr]->cpu_system + st_mp_cpu[curr]->cpu_idle;
      }

      else if (!strncmp(line, "cpu", 3)) {
	 /*
	  * Read the number of jiffies spent in user, nice, system and idle mode for current proc.
	  * This is done only on SMP machines.
	  */
	 sscanf(line + 3, "%d %u %u %u %lu", &proc_nb, &cc_user, &cc_nice, &cc_system, &cc_idle);
	 st_mp_cpu_i = st_mp_cpu[curr] + proc_nb + 1;
	 st_mp_cpu_i->cpu_user   = cc_user;
	 st_mp_cpu_i->cpu_nice   = cc_nice;
	 st_mp_cpu_i->cpu_system = cc_system;
	 st_mp_cpu_i->cpu_idle   = cc_idle;
      }

      else if (!strncmp(line, "intr ", 5))
	 /* Read total number of interrupts received since system boot */
	 sscanf(line + 5, "%u", &(st_mp_cpu[curr]->irq));
   }

   /* Close stat file */
   fclose(statfp);
}


/*
 * Read stats from /proc/interrupts
 */
void read_interrupts_stat(short curr)
{
   FILE *irqfp;
   struct mp_stats *st_mp_cpu_i;
   static char line[512];	/* Should depend on the nb of processors */
   unsigned int irq = 0, cpu;

   for (cpu = 0; cpu <= proc_used; cpu++) {
      st_mp_cpu_i = st_mp_cpu[curr] + cpu +1;
      st_mp_cpu_i->irq = 0;
   }

   /* Open interrupts file */
   if ((irqfp = fopen(INTERRUPTS, "r")) != NULL) {

      while (fgets(line, 512, irqfp) != NULL) {

	 if (isdigit(line[2])) {
	
	    for (cpu = 0; cpu <= proc_used; cpu++) {
	       st_mp_cpu_i = st_mp_cpu[curr] + cpu + 1;
	       sscanf(line + 4 + 11 * cpu, " %10u", &irq);
	       st_mp_cpu_i->irq += irq;
	    }
	 }
      }

      /* Close serial file */
      fclose(irqfp);
   }
}


/*
 * Main entry to the program
 */
int main(int argc, char **argv)
{
   int opt = 0, i;
   struct utsname header;
   short curr = 1, dis_hdr = -1, opt_used = 0, dis = 1;
   unsigned long lines = 0;
   int rows = 23;

#ifdef USE_NLS
   /* Init National Language Support */
   init_nls();
#endif

   /* How many processors on this machine ? */
   get_nb_proc_used(&proc_used, ~0);

   /*
    * proc_used: a value of 1 means there are 2 processors (0 and 1).
    * In this case, we have to allocate 3 structures: global, proc0 and proc1.
    */
   salloc_mp_cpu(proc_used + 2);

   while (++opt < argc) {

      if (!strcmp(argv[opt], "-V"))
	 usage(argv[0]);

      else if (!strcmp(argv[opt], "-P")) {
	 /* '-P ALL' can be used on UP machines */
	 if (argv[++opt]) {
	    opt_used = 1;
	    dis_hdr++;
	    if (!strcmp(argv[opt], K_ALL) || !strcmp(argv[opt], "-1")) {
	       if (proc_used)
		  dis_hdr = 9;
	       /* Set bit for every processor. Also indicate to display stats for CPU 'all' */
	       memset(cpu_bitmap, 0xff, ((proc_used + 1 + (proc_used > 0)) >> 3) + 1);
	    }
	    else {
	       if (strspn(argv[opt], DIGITS) != strlen(argv[opt]))
		  usage(argv[0]);
	       i = atoi(argv[opt]);	/* Get cpu number */
	       if (i > proc_used) {
		  fprintf(stderr, _("Not that many processors!\n"));
		  exit(1);
	       }
	       i++;
	       *(cpu_bitmap + (i >> 3)) |= 1 << (i & 0x07);
	    }
	 }
	 else
	    usage(argv[0]);
      }

      else if (!interval) {		/* Get interval */
	 if ((strspn(argv[opt], DIGITS) != strlen(argv[opt])) ||
	     WANT_BOOT_STATS(flags))
	    usage(argv[0]);
	 interval = atol(argv[opt]);
	 if (!interval)
	    flags |= F_BOOT_STATS;
	 else if (interval < 0)
	   usage(argv[0]);
	 count = -1;
      }

      else if (count <= 0) {		/* Get count value */
	 if (strspn(argv[opt], DIGITS) != strlen(argv[opt]))
	    usage(argv[0]);
	 count = atol(argv[opt]);
	 if (count < 1)
	   usage(argv[0]);
      }

      else
	 usage(argv[0]);
   }

   if (!opt_used)
      /* Option -P not used: set bit 0 (global stats among all proc) */
      *cpu_bitmap = 1;
   if (dis_hdr < 0)
      dis_hdr = 0;
   if (!dis_hdr) {
      /* Get window size */
      rows = get_win_height();
      lines = rows;
   }

   /* Get time */
   get_localtime(&loc_time);

   /* Get system name, release number and hostname */
   uname(&header);
   print_gal_header(&loc_time, header.sysname, header.release, header.nodename);

   st_mp_tstamp[0].hour   = loc_time.tm_hour;
   st_mp_tstamp[0].minute = loc_time.tm_min;
   st_mp_tstamp[0].second = loc_time.tm_sec;

   /* Read stats */
   read_proc_stat(0);
   read_interrupts_stat(0);

   if (!interval || WANT_BOOT_STATS(flags)) {
      /* Display since boot time */
      st_mp_tstamp[1].hour   = st_mp_tstamp[0].hour;
      st_mp_tstamp[1].minute = st_mp_tstamp[0].minute;
      st_mp_tstamp[1].second = st_mp_tstamp[0].second;

      st_mp_tstamp[1].uptime = 0;

      memset(st_mp_cpu[1], 0, MP_STATS_SIZE * (proc_used + 2));

      write_stats(0, DISP_HDR);
      exit(0);
   }

   /* Set a handler for SIGALRM */
   alarm_handler(0);

   /* Save the first stats collected. Will be used to compute the average */
   st_mp_tstamp[2].hour   = st_mp_tstamp[0].hour;
   st_mp_tstamp[2].minute = st_mp_tstamp[0].minute;
   st_mp_tstamp[2].second = st_mp_tstamp[0].second;

   st_mp_tstamp[2].uptime = st_mp_tstamp[0].uptime;

   memcpy(st_mp_cpu[2], st_mp_cpu[0], MP_STATS_SIZE * (proc_used + 2));

   pause();

   /* Main loop */
   do {

      /* Resetting the structure is not needed since all the fields will be filled */

      /* Save time */
      get_localtime(&loc_time);

      st_mp_tstamp[curr].hour   = loc_time.tm_hour;
      st_mp_tstamp[curr].minute = loc_time.tm_min;
      st_mp_tstamp[curr].second = loc_time.tm_sec;

      /* Read stats */
      read_proc_stat(curr);
      read_interrupts_stat(curr);

      /* Write stats */
      if (!dis_hdr) {
	 dis = lines / rows;
	 if (dis)
	    lines %= rows;
	 lines++;
      }
      write_stats(curr, dis);

      /* Flush data */
      fflush(stdout);

      if (count > 0)
	 count--;
      if (count) {
	 curr ^= 1;
	 pause();
      }
   }
   while (count);

   /* Write stats average */
   write_stats_avg(curr, dis_hdr);

   return 0;
}
