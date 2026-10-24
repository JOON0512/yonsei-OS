#define NPROC        64  // maximum number of processes
#define NCPU          8  // maximum number of CPUs
#define NOFILE       16  // open files per process
#define NFILE       100  // open files per system
#define NINODE       50  // maximum number of active i-nodes
#define NDEV         10  // maximum major device number
#define ROOTDEV       1  // device number of file system root disk
#define MAXARG       32  // max exec arguments
#define MAXOPBLOCKS  10  // max # of blocks any FS op writes
#define LOGSIZE      (MAXOPBLOCKS*3)  // max data blocks in on-disk log
#define NBUF         (MAXOPBLOCKS*3)  // size of disk block cache
#define FSSIZE       2000  // size of file system in blocks
#define MAXPATH      128   // maximum file path name
//mycode :)
#define INIT_TICKETS 5     // Initial number of tickets assigned to new processes
#define MAX_TICKETS 10     // Maximum tickets a process can have
#define MIN_TICKETS 1      // Minimum tickets a process must have
#define LCM 2520          // Least Common Multiple for ticket normalization (LCM of 1-10)
//