#include <time.h>
#include <stdlib.h>
/**
 * 
 *Confere o Checksum do frame informado
 *
 *
 */

int verify_checksum(char frame[],int size,int posH, int posL)
{
 
 int tempCks = 0;

 for(int i = 0; i < size; i++)
 {

  if(i == posH)
    continue;
  if(i == posL)
    continue;
  
   tempCks += frame[i];
 }
 
 tempCks = 0x10000 - tempCks;

 tempCks = tempCks & 0x00FFFF;


 if(frame[posH] == ((0xFF00 & tempCks ) >> 8 ) && (frame[posL] ==   (tempCks &  0x00FF)))
  return 0;
 else
  return tempCks;
}


int calculate_checksum(char frame[], int size, int posH, int posL)
{

 int tempCks = 0;

 for(int i = 0; i < size; i++)
 {
  if(i == posH)
   continue;
  if(i == posL)
   continue;
 
   tempCks += frame[i];
 }

  tempCks = 0x10000 - tempCks;

  tempCks = tempCks & 0x00FFFF;
  
return tempCks;
}

char aleat_bytes(){
 srand(time(NULL));
 int r = rand(); 
  return r;

}
