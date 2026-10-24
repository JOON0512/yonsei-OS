// Test that fork fails gracefully.
// Tiny executable so that the limit can be filling the proc table.

#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

#define N  1000

void
print(const char *s)
{
  write(1, s, strlen(s));
}

void
forktest(int longtest)
{
  int n, pid;
  unsigned r = 1;

  print("fork test\n");

  for(n=0; n<N; n++, r <<= (n & 0x1)){
    pid = fork();
    if(pid < 0)
      break;
    if(pid == 0) {
      if(longtest) for(int i = 0; i < r; i++) ;
      exit(0);
    }
  }

  if(n == N){
    print("fork claimed to work N times!\n");
    exit(1);
  }

  for(; n > 0; n--){
    if(wait(0) < 0){
      print("wait stopped early\n");
      exit(1);
    }
  }

  if(wait(0) != -1){
    print("wait got too many\n");
    exit(1);
  }

  print("fork test OK\n");
}

int
main(int argc, char **argv)
{
  forktest((argc == 2) && !strcmp(argv[1], "long"));
  exit(0);
}
