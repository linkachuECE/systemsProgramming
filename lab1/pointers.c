#include <stdio.h>

int main(){

  int i;
  char char_array[5] = {'a', 'b', 'c', 'd', 'e'};
  int int_array[5] = {1, 2, 3, 4, 5};

  char *char_pointer;
  int *int_pointer;

  char_pointer = (char*)int_array;
  int_pointer = (int*)char_array;

  for(i=0; i < 5; i++) {
    printf("[Integer pointer] points to %p, which contains the char '%c'\n", int_pointer, (char)*int_pointer);
    int_pointer = (int*)(((long)int_pointer) + sizeof(char));
  }

  for(i=0; i < 5; i++) {
    printf("[Char pointer] points to %p, which contains the integer '%d'\n", char_pointer, (int)*char_pointer);
    char_pointer = char_pointer + sizeof(int);
  }
}
