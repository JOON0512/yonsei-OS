#include "kernel/types.h"
#include "kernel/fcntl.h"
#include "user/user.h"

#define buf_size    128     // Max length of user input
#define max_args    16      // Max number of arguments

// function prototypes
int runcmd(char *cmd);                // Run a command.
void parsecmd(char *cmd, char **argv); // parse cmd into args.
void runpcmd(char **cmd);              // run command w/ pipe.
void runrcmd(char *cmd);              // run single command, handle redirection.

// run command function.
// parsing and handling pipes, sequences, background exec.
int runcmd(char *cmd){
    int rc; // for fork() syscall
    // Empty command ignored: current char is \0(null-terminator)
    if(!*cmd) return 1;
    // Skip leading white space(s).
    while(*cmd == ' ')cmd++;
    // Remove trailing white space(s).
    for(char *c = cmd + strlen(cmd) - 1; *c == ' '; c--) *c = 0;
    if(!strcmp(cmd, "exit"))                 // exit command
        return 0;                            
    else if(!strncmp(cmd, "cd ", 3)){        // cd command
        if(chdir(cmd + 3) < 0) fprintf(2, "Cannot cd %s\n", cmd+3); // directory path starts from 4th char.
        return 1; //continue shell loop
    }
    // handle command sequences (;)
    int isscmd = 0; // isscmd: is sequence command ~init
    for(char* seq = cmd; *seq; seq++){ // pointer seq is init to the same address of cmd.
    //for loop iterates over command string, looking for ';' char.
        if(*seq == ';'){
            *seq = 0;    // split command by ';'
            runcmd(cmd); // recursive function call. sequence before ';' will be executed.
            return runcmd(seq + 1); // sequence after ';' will be executed.
        }
    }

    if(!isscmd){
        int isbcmd = 0; // isbcmd: is background command ~init
        rc = fork();
        if(rc < 0){ // fork failed. maybe because of resource issues?
            fprintf(2, "sfork failed\n"); exit(1);
        } else if(rc > 0) { // parent process, rc == PID.
            char* bg;
            for(bg = cmd; *bg; bg++){
                if(*bg == '&'){
                    *bg = 0; // replace '&' with null-terminator
                    isbcmd = 1; // mark isbcmd as 1. 
                    break;
                }
            }
            if(!isbcmd) wait(0); // if there is no background command, parent proc waits for child proc.
            return 1;
        } else{ // rc == 0, current process is child process.
            char* bg;
            for(bg = cmd; *bg; bg++){ // runs the same loop to check for background commands.
                if(*bg == '&'){
                    *bg = 0;
                    isbcmd = 1;
                    break;
                }
            }
        }
    }
    // handle pipe (|).
    runpcmd(&cmd);
    // handle redirection (>), redirect if needed.
    runrcmd(cmd);
    return 1; // continue shell loop.
}

// parse cmd into arg
void parsecmd(char *cmd, char **argv){
    int argc = 0;

    // Skip leading white space(s).
    while(*cmd == ' ')cmd++;
    // Remove trailing white space(s).
    
    for(char *c = cmd + strlen(cmd) - 1; *c == ' '; c--) *c = 0;
    while (*cmd != 0){
        while (*cmd == ' ') *cmd++ = 0;         // skip spaces
        
        argv[argc++] = cmd;                     // mark start of argument
        
        while (*cmd != ' ' && *cmd != 0) cmd++; // Move to next space or end of command

        if(argc >= max_args) break;
    }
    argv[argc] = 0;    // NULL-terminate arg array
}

void runpcmd(char** cmd){
    // a pipe creates two file dexcriptors, fd[0] for reading, and fd[1] for writing.
    int fd[2]; // file descriptors for the pipe.
    int rc;
    char* pipeidx; // pointer to the location of the pipe '|' in the command.
    int ispcmd = 0; // ispcmd: is pipe command ~init

    for(pipeidx = *cmd; *pipeidx; pipeidx++){
        if(*pipeidx == '|'){
            *pipeidx = 0; // split command into two parts at '|'. same way as ';'.
            ispcmd = 1; // mark that a pipe was found.
            break;
        }
    }
    // setting up the pipe
    if(ispcmd){ // function creates a pipe using the pipe(fd) system call.
        if(pipe(fd) < 0){
            fprintf(2, "pipe failed\n");
            exit(1);
        }
        rc = fork();
        if(rc < 0){
            fprintf(2, "fork failed\n");
            exit(1);
        }
        else if(rc == 0){ // child process
            close(1); // close stdout
            dup(fd[1]); // redirect stdout to pipe's write end
            close(fd[0]); close(fd[1]); // close pipe's read end & write end fd[1].
            runpcmd(cmd); // execute left command
        } else{ // parent process
            close(0); // closes stdin
            dup(fd[0]); // lowest available file descriptor now points to fd[0].
            close(fd[0]); close(fd[1]); // close unused read & write end of pipe.
            *cmd = pipeidx + 1; // move pointer to right side of command(after '|').
            runpcmd(cmd); // execute right command
        }
    }
    return;
}

// run single command, handling redirection if needed
void runrcmd(char *cmd){
    char *argv[max_args]; // array to store the parsed command and it's arguments.
    char* file; // holds the file name of where the output should be redirected.
    int isrcmd = 0; 
    //fprintf(2, "%s\n", cmd);
    // handle output redirection (>)
    for(char *redir = cmd; *redir != 0; redir++){
        if(*redir == '>'){
            *redir = 0;  // split command @ '>'
            file = redir + 1; // sets file to point to string after '>'.(name of file where file output goes)
            while(*file == ' '){
                file += 1; // skip over spaces btw '>' and file name
            }
            for(char *c = file + strlen(file) - 1; *c == ' '; c--) *c = 0;
            isrcmd = 1; // mark that redirection is needed
            break;
        }
    }

    if(isrcmd){ // if command should be redirected
        //fprintf(2, "%s\n", file);
        int fd = open(file, O_WRONLY | O_CREATE | O_TRUNC );
        // open(): open file in write mode(O_WRONLY), if file doesn't exist create(O_CREATE), start with empty file(O_TRUNC)
        close(1);
        dup(fd); // duplicate file descriptor fd to stdout (fd 1)
        parsecmd(cmd, argv); // parses the cmd(command before '>') into an array of arguments argv, which will be passed to the exec() call.
        exec(argv[0], argv); // exec replaces current proc w/ new proc(argv[0]).
    }
    else{ // if ne redirection is needed
        parsecmd(cmd, argv); 
        exec(argv[0], argv);
    }
}

// Read a shell input. ***readcmd should not be modified***
char* readcmd(char *buf){
    // Read an input from stdin.
    fprintf(1, "$ ");
    memset(buf, 0, buf_size);
    char *cmd = gets(buf, buf_size);
    // Chop off the trailing '\n'.
    if(cmd)cmd[strlen(cmd)-1] = 0;

    return cmd;
}

int main(int argc, char **argv) {  // ***main also should not be modified***
    int fd = 0;
    char *cmd = 0;
    char buf[buf_size];
  
    // Ensure three file descriptors are open.
    while((fd = open("console", O_RDWR)) >= 0) {
        if(fd >= 3) { close(fd); break; }
    }
  
    fprintf(1, "EEE3535 Operating Systems: starting ysh\n");
  
    // Read and run input commands.
    while((cmd = readcmd(buf)) && runcmd(cmd)) ;
  
    fprintf(1, "EEE3535 Operating Systems: closing ysh\n");
    exit(0);
}