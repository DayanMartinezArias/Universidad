#include <stdio.h>

int main (int argc, char* argv[]) {
  char* string = argv[1];
  printf("Hello World\n%c\n", string[1]);
  return 0;
}