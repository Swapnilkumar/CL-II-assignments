#include <stdio.h>
#include <string.h>

//This functions returns matched param's value in a buffer

char* get_param_val(char* buffer, char *param_name)
{
  char* match;
  match = strstr (buffer, param_name);
  if (match == NULL)
    return NULL;
  
  return match;
}

void get_cpu_params ()
{
  FILE* fp;
  char buffer[1024];
  size_t bytes_read;
  float clock_speed;
  int cache_size;
  char modelname[100];

  /* Read the entire contents of /proc/cpuinfo into the buffer. */
  fp = fopen ("/proc/cpuinfo", "r");
  bytes_read = fread (buffer, 1, sizeof (buffer), fp);
  fclose (fp);

  /* Return if read failed or if buffer isn't big enough. */
  if (bytes_read == 0 || bytes_read == sizeof (buffer))
    return ;
  
  buffer[bytes_read] = '\0';
  
  sscanf (get_param_val(buffer,"cpu MHz"), "cpu MHz : %f", &clock_speed);
  printf ("CPU clock speed: %f MHz\n",clock_speed);
  
  sscanf (get_param_val(buffer,"cache size"), "cache size : %d", &cache_size);
  printf ("CPU cache size: %d KB\n",cache_size);
  
}
int main ()
{
  get_cpu_params ();
  return 0;
}


