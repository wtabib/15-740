#include <stdio.h>
//#include <string.h>
#include "strcat_opt.h"


int main() {
  char mystring[10];
  char mystring2[10];
  mystring[7] = '\0';
  mystring[8] = 'b';
  mystring[9] = '\0';

  mystring2[0] = 'c';
  mystring2[1] = 'd';
  mystring2[2] = '\0';


  //__GI_strcat(mystring+7, mystring2);
  strcat_opt(mystring+7, mystring2);
  printf("%s\n", mystring+7);
}
 
