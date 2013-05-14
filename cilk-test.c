#include <stdio.h>
#include <cilk/cilk.h> 

int main(){
  int i = 0;
  double x [10000];
  cilk_for (i = 0; i <= 10000; i++)
    x[i] = 1.0;
  printf("done\n");
}
