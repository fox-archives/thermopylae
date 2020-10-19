#ifndef UTIL
#define UTIL

void ensure_root();
typedef struct SortedArgs Struct;
struct SortedArgs {
  char** our_args;
  char** their_args;

  unsigned int our_args_length;
  unsigned int their_args_length;
};
struct SortedArgs get_sargs(int argc, char** argv);
int subcommand_is(char** argv, int arr_len, char* str);
int contains(char** arr, int arr_len, char* str);

#endif
