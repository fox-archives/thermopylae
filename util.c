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

void ensure_root() {
  uid_t euid = geteuid();
  if (euid != 0) {
    fputs("Please run this program as root. Exiting\n", stdout);
    exit(EXIT_FAILURE);
  }
}

struct SortedArgs get_sargs(int argc, char** argv) {
  char **our_args = NULL;
  char **their_args = NULL;

  unsigned int our_args_length = 0;
  unsigned int their_args_length = 0;

  int double_hyphen_found = false;
  for(int i = 0; i < argc; i++) {
      if(strcmp(argv[i], "--") == 0) {
          double_hyphen_found = true;
          our_args = &argv[0];
          our_args_length = i;

          their_args = &argv[i + 1];
          their_args_length = argc - (i + 1);

          break;
      }
  }

  if (!double_hyphen_found) {
    our_args = &argv[0];
    our_args_length = argc;
  }

  struct SortedArgs sargs = {
    our_args = our_args,
    their_args = their_args,
    our_args_length = our_args_length,
    their_args_length = their_args_length
  };

  return sargs;
}

int subcommand_is(char** argv, int arr_len, char* str) {
  if (arr_len <= 1) {
    return false;
  }

  if (strcmp(argv[1], str) == 0) {
    return true;
  }

  return false;
}

int contains(char** arr, int arr_len, char* str) {
  for(int i = 0; i < arr_len; i++) {
    if (strcmp(arr[i], str) == 0) {
      return true;
    }
  }

  return false;
}
