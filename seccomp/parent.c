#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

#include <sys/types.h>
#include <sys/prctl.h>
#include <sys/syscall.h>
#include <sys/socket.h>

#include <linux/filter.h>
#include <linux/seccomp.h>
#include <linux/audit.h>

#define ArchField offsetof(struct seccomp_data, arch)

#define Allow(syscall)						\
     BPF_JUMP(BPF_JMP+BPF_JEQ+BPF_K, SYS_##syscall, 0, 1),	\
	  BPF_STMT(BPF_RET+BPF_K, SECCOMP_RET_ALLOW)

struct sock_filter filter[] = {
     /* validate arch */
     BPF_STMT(BPF_LD+BPF_W+BPF_ABS, ArchField),
     BPF_JUMP( BPF_JMP+BPF_JEQ+BPF_K, AUDIT_ARCH_X86_64, 1, 0),
     BPF_STMT(BPF_RET+BPF_K, SECCOMP_RET_KILL),

     /* load syscall */
     BPF_STMT(BPF_LD+BPF_W+BPF_ABS, offsetof(struct seccomp_data, nr)),

     /* list of allowed syscalls */
     Allow(exit_group),  /* exits a processs */
     Allow(brk),     /* for malloc(), inside libc */
     Allow(mmap),        /* also for malloc() */
     Allow(munmap),      /* for free(), inside libc */
     Allow(write),       /* called by printf */
     Allow(fstat),       /* called by printf */

     /* and if we don't match above, die */
     BPF_STMT(BPF_RET+BPF_K, SECCOMP_RET_KILL),
};

struct sock_fprog filterprog = {
     .len = sizeof(filter)/sizeof(filter[0]),
     .filter = filter
};


int startSeccomp(){
     /* set up the restricted environment */

     int err = prctl(PR_SET_NO_NEW_PRIVS, 1, 0, 0, 0);
     if (err) {
	  return err;
	  /* printf("Could not start seccomp:\n"); */
	  /* perror("Could not start seccomp:"); */
	  /* exit(1); */
     }

     printf("started seccomp\n");
     err = prctl(PR_SET_SECCOMP, SECCOMP_MODE_FILTER, &filterprog);
     if (err == -1) {
	  return err;
	  /* printf("Could not start seccomp:\n"); */
	  /* perror("Could not start seccomp:"); */
	  /* exit(1); */
     }

     printf("set permission: %d\n", err);
     return err;
}

int main(int argc, char **argv) {


     char buf[1024];

     pid_t pID = fork();
     if (pID > 0){ //parent
	  printf("parent here child pid is: %d\n", pID);
     } else if (pID < 0) {
	  printf("fork failed\n");
	  return -127;
     } else {  //child
	  startSeccomp();
	  printf("executing child proccess\n");
	  execl("/usr/bin/ls", "/usr/bin/ls", "-l","-a","-h", (char*) 0);
	  /* execl("/root/child", "/root/child", (char*) 0); */
     }


     return 0;




}
