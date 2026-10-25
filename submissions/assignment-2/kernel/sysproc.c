#include "types.h"
#include "riscv.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "spinlock.h"
#include "proc.h"

uint64
sys_exit(void)
{
  int n;
  argint(0, &n);
  exit(n);
  return 0;  // not reached
}

uint64
sys_getpid(void)
{
  return myproc()->pid;
}

uint64
sys_fork(void)
{
  return fork();
}

uint64
sys_wait(void)
{
  uint64 p;
  argaddr(0, &p);
  return wait(p);
}

uint64
sys_sbrk(void)
{
  uint64 addr;
  int n;

  argint(0, &n);
  addr = myproc()->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

uint64
sys_sleep(void)
{
  int n;
  uint ticks0;

  argint(0, &n);
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(killed(myproc())){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

uint64
sys_kill(void)
{
  int pid;

  argint(0, &pid);
  return kill(pid);
}

// return how many clock tick interrupts have occurred
// since start.
uint64
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}

// char process state
char*
pstate_char(struct proc *p)
{
    acquire(&p->lock);
    switch(p->state){
      case SLEEPING:   release(&p->lock); return "S";
      case RUNNABLE:   release(&p->lock); return "R";
      case RUNNING:    release(&p->lock); return "X"; 
      case ZOMBIE:     release(&p->lock); return "Z";
      default:         release(&p->lock); return "U";    
    }
}

// int process state
int
pstate_int(struct proc *p)
{
    acquire(&p->lock);
    switch(p->state){
      case SLEEPING:   release(&p->lock); return -1;
      case RUNNABLE:   release(&p->lock); return -2;
      case RUNNING:    release(&p->lock); return -3; 
      case ZOMBIE:     release(&p->lock); return -4;
      default:         release(&p->lock); return -5;    
    }
}

uint64
sys_pstate(void)
{
  // print tags that we have to print by the ps command
  printf("PID\tPPID\tState\tRuntime\tOFiles\tName\n");

  uint argc = 0;
  int args[6];

  for(int i = 0; i < 6; i++){
    argint(i, &args[i]);
    if(args[i] != 0) argc++;
  }

  for(int i = 0; i < NPROC; i++){
    struct proc *p = &proc[i];

    acquire(&p->lock);

    if(p->state == UNUSED || p->state == USED) {
      release(&p->lock);
      continue; // not handling process of states other than S, R, X, Z
    }
    release(&p->lock);

    uint runtime = ticks - p->start_time; // Get runtime and open files
    int open_files = 0;

    for (int j = 0; j < NOFILE; j++) {    // count number of open files
        if (p->ofile[j]) {
            open_files++;
        }
    }

    // ps all by itself prints all processes
    if(argc == 0){
      printf("%d\t%d\t%s\t%d:%d.%d\t%d\t%s\n", p->pid, p->parent ? p->parent->pid : 0, pstate_char(p), 
           runtime / 600, (runtime % 600) / 10, (runtime % 600) % 10, open_files, p->name);
    }
    // find processes using pid(running) or pstate(sleeping, ready, zombie)
    else{
      for(int j = 0; j < argc; j++){
        if(p->pid == args[j]){
          printf("%d\t%d\t%s\t%d:%d.%d\t%d\t%s\n", p->pid, p->parent ? p->parent->pid : 0, pstate_char(p), 
           runtime / 600, (runtime % 600) / 10, (runtime % 600) % 10, open_files, p->name);
           break;
        }
        else if(pstate_int(p) == args[j]){
          printf("%d\t%d\t%s\t%d:%d.%d\t%d\t%s\n", p->pid, p->parent ? p->parent->pid : 0, pstate_char(p), 
           runtime / 600, (runtime % 600) / 10, (runtime % 600) % 10, open_files, p->name);
           break;
        }
      }
    }
  }
  return 0;
}
