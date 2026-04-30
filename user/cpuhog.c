#include "kernel/types.h"
#include "user/user.h"

int
main(int argc, char **argv)
{
  int pri = 3;  // default priority
  
  if(argc > 1){
    pri = atoi(argv[1]);
  }
  
  set_priority(getpid(), pri);
  
  printf("cpuhog pid=%d priority=%d starting\n", getpid(), pri);
  
  // Infinite CPU loop
  volatile int x = 0;
  while(1) {
    x++;  // Burn CPU
  }
  
  exit(0);
}
