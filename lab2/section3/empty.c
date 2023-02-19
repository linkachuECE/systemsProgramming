#include <stdio.h>
#include <unistd.h>

/*
 *      AUTHOR: Ethan Braun
 *      DATE CREATED: 02/15/23
 *      DESCRIPTION: The purpose of this lab is to program is to continually print with one-second intervals between
 *      CONTRIBUTORS: None. The code is identical to what I downloaded from D2L
 */

void main(){
  for(;;)
  {
    printf("Endless print \n");
    sleep(1);
  }

}
