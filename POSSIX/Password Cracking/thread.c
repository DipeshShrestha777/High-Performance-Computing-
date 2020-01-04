#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <crypt.h>
#include <time.h>

#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <errno.h>
#include <sys/stat.h>
#include <string.h>
#include <time.h>
#include <pthread.h>
#include <math.h>
#include <malloc.h>


int n_passwords = 4;

char *encrypted_passwords[] = {
  "$6$KB$H8s0k9/1RQ783G9gF69Xkn.MI.Dq5Ox0va/dFlkknNjO7trgekVOjTv1BKCb.nm3vqxmtO2mOplhmFkwZXecz0",
  "$6$KB$Z5p6Fe7quS8nMyDnY/vxQhaR2W5Hb/F1OA9rHUtxJyFL078L3sY5yVUHzhCKE/u7gyoS26MlzroxosQits6eH/",
  "$6$KB$cmCCxws2opceXZq71lmWnSNO5/0sAN3zR88i/qGgiWda2Z8LOvIQue2RwamA78gQmQcBffga4ItDZENgNWlZ.0",
  "$6$KB$VaoLCdXouESV71R/qK7zabtCVszVHm0qxN9.0/oGohh4dj1bqMBCBoe.DzpABFNzDEfoizkgIMKZYYzjbzUsm1"
};



void substr(char *dest, char *src, int start, int length){
  memcpy(dest, src + start, length);
  *(dest + length) = '\0';
}



void crack(char *encrypted_passwords){
  pthread_t t1,t2;

  void *kernel_function_1();
  void *kernel_function_2();

  pthread_create(&t1, NULL, kernel_function_1, (void *)encrypted_passwords);
  pthread_create(&t2, NULL, kernel_function_2, (void *)encrypted_passwords);
 
  pthread_join(t1, NULL);
  pthread_join(t2, NULL);
}


void *kernel_function_1(char *salt_and_encrypted){

  int x, y, z;     
  char salt[7];    
  char plain[7];   
  char *enc;      
  int count = 0;   

  substr(salt, salt_and_encrypted, 0, 6);

  for(x='A'; x<='L'; x++){
    for(y='A'; y<='Z'; y++){
      for(z=0; z<=99; z++){
        sprintf(plain, "%c%c%02d", x, y, z); 
        enc = (char *) crypt(plain, salt);
        count++;
        if(strcmp(salt_and_encrypted, enc) == 0){
          printf("#%-8d%s %s\n", count, plain, enc);
        } else {
          printf(" %-8d%s %s\n", count, plain, enc);
        }
      }
    }
  }
  printf("%d solutions explored\n", count);
}

void *kernel_function_2(char *salt_and_encrypted){

  int x, y, z;     
  char salt[7];    
  char plain[7];   
  char *enc;       
  int count = 0;   

  substr(salt, salt_and_encrypted, 0, 6);

  for(x='M'; x<='Z'; x++){
    for(y='A'; y<='Z'; y++){
      for(z=0; z<=99; z++){
        sprintf(plain, "%c%c%02d", x, y, z); 
        enc = (char *) crypt(plain, salt);
        count++;
        if(strcmp(salt_and_encrypted, enc) == 0){
          printf("#%-8d%s %s\n", count, plain, enc);
        } else {
          printf(" %-8d%s %s\n", count, plain, enc);
        }
      }
    }
  }
  printf("%d solutions explored\n", count);
}

int time_difference(struct timespec *start, struct timespec *finish,
                    long long int *difference) {
  long long int ds =  finish->tv_sec - start->tv_sec;
  long long int dn =  finish->tv_nsec - start->tv_nsec;

  if(dn < 0 ) {
    ds--;
    dn += 1000000000;
  }
  *difference = ds * 1000000000 + dn;
  return !(*difference > 0);
}

int main(int argc, char *argv[]){
  int i;
  struct timespec start, finish;  
  long long int time_elapsed;

  clock_gettime(CLOCK_MONOTONIC, &start);
  for(i=0;i<n_passwords;i<i++) {
    crack(encrypted_passwords[i]);
  }

  clock_gettime(CLOCK_MONOTONIC, &finish);
  time_difference(&start, &finish, &time_elapsed);
  printf("Time elapsed was %lldns or %0.9lfs\n", time_elapsed,
         (time_elapsed/1.0e9));

  return 0;
}	
