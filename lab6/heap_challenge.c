#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <sys/types.h>

struct data {
  char name[64];
};

struct fp {
  int (*fp)();
};

void bad_code()
{
  printf("You managed to guide the execution of an application! Good job!!\n");
}

void print_function()
{
  printf("This is a regular print :( Try again\n");
}

int main(int argc, char **argv)
{
  struct data *pointer1;
  struct fp *pointer2;

  pointer1 = malloc(sizeof(struct data));
  pointer2 = malloc(sizeof(struct fp));
  pointer2->fp = print_function;

  printf("data is at %p, fp is at %p\n", pointer1, pointer2);

  strcpy(pointer1->name, argv[1]);
  
  // DELETE THESE
  printf("%p\n",bad_code);
  printf("%p\n", pointer2->fp);

// 0x00000000000011a9

  pointer2->fp();

}
