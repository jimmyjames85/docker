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

int main(int argc, char **argv) {

     printf("child started\n");
     char buf[1024];

     int fd = socket(AF_INET6, SOCK_STREAM, 0);
     printf("child created socket\n");
     /* if (argc > 1 && strcmp(argv[1], "haxor") == 0) { */

     /* 	  /\* ...and start sending spam *\/ */
     /* } */


}
