#include <stdlib.h>
#include <stdio.h>

static void* return_arg(void* p);
int frame3 ( void )
{
  int *a = malloc(15 * sizeof(int)); //Extended the buffer so the line "n = a[10]" is a legal read
  int i = 0;
  //Initialized values of a
  for(i = 0; i < 15; i++){
    a[i] = 0;
  }

  int n = a[10];

  if (a[5] == 42) {
    printf("hello from frame3().  The answer is 42.\n");
  } else {
    printf("hello from frame3().  The answer is not 42.\n");
  }

  n = a[  a[0] & 7  ];

  free(a);
  //Got rid of an illegal free that free'd a again
  //Got rid of a free on a non-malloc'd int

  a = malloc(99 * sizeof(int));
  free(a); //Have to free a

  return n;
}

int frame2 ( void )
{
  return frame3() - 1;
}

int frame1 ( void )
{
  return frame2() + 1;
}

int main ( void )
{
  return frame1() - 1;
}

static void* return_arg(void* p)
{
   return p;
}

