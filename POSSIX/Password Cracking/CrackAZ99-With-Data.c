#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <crypt.h>
#include <time.h>

int n_passwords = 4;

char *encrypted_passwords[] = {
  "$6$KB$dSPepSSmoYmb2CX7NHkVpJmOn6ohIO105u8ZzOEK14lYyCwHGr6sskjOXY1YkAmSVJwOqjbpegXmD8nC/Cusk1",
  "$6$KB$Fm6h/CAxqUSxnmW.eA0fYUb9.HyMQ9iSynESqaV1BMOZ/VytPPhIKREdahNUGy3vZ2nW4ln3A9bZ3dUJJbYp5.",
  "$6$KB$XuMILs4aLPZ8Q9Rpr92NUoosyBuzvAlog78GkgaxbtJXoZX3jCfPqhIadJUHCIyumXaPVEfVWCXb4NFLLWT.T1",
  "$6$KB$VL.gwO9GAIFTbIg4rS3Ovj.up1nBgojr03.vSqZVhq98LSoPioPq2bQK9a0jw4bN2MY4Os8QCSEgTJnUaDj83."
};


void substr(char *dest, char *src, int start, int length){
  memcpy(dest, src + start, length);
  *(dest + length) = '\0';
}

void crack(char *salt_and_encrypted){
  int a, b, c,d;     
  char salt[7];    
  char plain[7];  
  char *enc;       
  int count = 0;   

  substr(salt, salt_and_encrypted, 0, 6);

  for(a='A'; a<='Z'; a++){
    for(b='A'; b<='Z'; b++){
for(d='A'; d<='Z'; d++){
      for(c=0; c<=99; c++){
        sprintf(plain, "%c%c%c%02d", a, b,d,c); 
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
  }
  printf("%d solutions explored\n", count);
}

int time_difference(struct timespec *start,
                    struct timespec *finish,
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
