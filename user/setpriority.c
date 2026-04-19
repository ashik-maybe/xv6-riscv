#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char **argv)
{
  if(argc != 3){
    fprintf(2, "Usage: setpriority <pid> <priority>\n");
    exit(1);
  }

  int pid = atoi(argv[1]);
  int priority = atoi(argv[2]);

  if(priority < 1 || priority > 5){
    fprintf(2, "Priority must be 1-5\n");
    exit(1);
  }

  if(set_priority(pid, priority) < 0){
    fprintf(2, "Failed to set priority\n");
    exit(1);
  }

  printf("Priority set to %d\n", priority);
  exit(0);
}