#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "user/sid.h"

void print_help(const char *s) {
  fprintf(2, "%s [on/off]\n", s);
  exit(1);
}

int main(int argc, char **argv) {
  if(argc != 2) { print_help(argv[0]); }

       if(!strcmp(argv[1], "on")) { sdbg(1); }
  else if(!strcmp(argv[1], "off")){ sdbg(0); }
  else { print_help(argv[0]); }

  exit(0);
}

