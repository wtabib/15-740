#include <stdio.h>
#include <string.h>
#include "strcat_opt.h"


int main() {
  char mystring[100];
  char mystring2[100];
  char *pattern = "abcdefghijklmnop";
  memcpy(mystring, pattern, strlen(pattern)+1);
  /*mystring[6] = 'e';
  mystring[7] = 'a';
  mystring[8] = 'b';
  mystring[9] = '\0';*/

  /*mystring2[0] = 'c';
  mystring2[1] = 'd';
  mystring2[2] = '\0';*/
  memcpy(mystring2, pattern, strlen(pattern)+1);


  //__GI_strcat(mystring+7, mystring2);
  strcat_opt(mystring, mystring2);
  printf("%s\n", mystring);
}
 
