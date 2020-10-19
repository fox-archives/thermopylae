#define _POSIX_C_SOURCE >= 200809L
#define _GNU_SOURCE
#include <sys/types.h>
#include <sys/wait.h>
#include <sched.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sched.h>
#include <unistd.h>
#include <error.h>
#include <errno.h>
#include <stdbool.h>
#include "util.h"

static int child_exec(void *stuff);
static int child_exec(void *stuff) {
  struct clone_args {
    char **argv;
  };

  struct clone_args *args = (struct clone_args *)stuff;

  int ret = execvp(args->argv[0], args->argv);
  if (ret != 0){
    fprintf(stderr, "failed to execvp argments %s\n",
    strerror(errno));
    exit(-1);
  }

	exit(EXIT_FAILURE);
}

void doIt(struct SortedArgs sargs);
void doIt(struct SortedArgs sargs) {
    int clone_flags = SIGCHLD | CLONE_NEWNS | CLONE_NEWUTS;
#define STACKSIZE (1024*1024)
  static char child_stack[STACKSIZE];
  pid_t pid = clone(child_exec, child_stack + STACKSIZE, clone_flags, &sargs.their_args);
  if (pid < 0) {
    perror("clone");
    exit(EXIT_FAILURE);
  }

  int i = waitpid(pid, NULL, 0);
  if (i == -1) {
    perror("waitpid");
    exit(EXIT_FAILURE);
  }
}


int main(int argc, char** argv) {
  ensure_root();
  struct SortedArgs sargs = get_sargs(argc, argv);

  if (getenv("DEBUG")) {
    for (int i = 0; i < sargs.our_args_length; i++) {
      printf("OUR   [%i] %s\n", i, sargs.our_args[i]);
    }

    for (int i = 0; i < sargs.their_args_length; i++) {
      printf("THEIR [%i] %s\n", i, sargs.their_args[i]);
    }
  }

  if (contains(sargs.our_args, sargs.our_args_length, "--help")) {
    char* help_text = "Thermopylae\n"
    "A broken container runtime\n"
    "\n"
    "Usage: thermopylae [FLAGS] [COMMAND] [OPTIONS] -- [EXEC] \n"
    "\n"
    "Commands:\n"
    "run\n"
    "    runs the thing (unimplemented)\n"
    "\n"
    "Flags:\n"
    "--help\n"
    "    Prints the help menu\n";
    puts(help_text);
  }

  if (subcommand_is(sargs.our_args, sargs.our_args_length, "run")) {
    if (sargs.their_args_length == 0) {
      puts("No program specified. Exiting");
      exit(0);
    }

    doIt(sargs);
    puts("Done.");
  } else {
    puts("No subcommand recognized. Exiting");
    exit(EXIT_FAILURE);
  }

  return EXIT_SUCCESS;
}
