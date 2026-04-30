#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

char*
state_name(int s)
{
  switch(s) {
    case 0: return "UNUSED";
    case 1: return "USED";
    case 2: return "SLEEPING";
    case 3: return "RUNNABLE";
    case 4: return "RUNNING";
    case 5: return "ZOMBIE";
    default: return "?";
  }
}

int
main(int argc, char **argv)
{
  struct pinfo info[64];
  int n = getpinfo(info);

  printf("PID   PRIORITY  STATE       CPU_TICKS\n");
  printf("----------------------------------------\n");
  for(int i = 0; i < n; i++){
    printf("%d     %d         %s         %d\n",
           info[i].pid,
           info[i].priority,
           state_name(info[i].state),
           info[i].ticks);
  }
  exit(0);
}