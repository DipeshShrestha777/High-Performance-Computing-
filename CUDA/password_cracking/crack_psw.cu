#include <stdio.h>
#include <cuda_runtime_api.h>
#include <time.h>

__device__ int is_a_match(char *attempt) {
  char password1[] = "DV78";
  char password2[] = "ER87";
  char password3[] = "GS58";
  char password4[] = "TA88";

  char *d = attempt;
  char *i = attempt;
  char *p = attempt;
  char *s = attempt;
  char *psw1 = password1;
  char *psw2 = password2;
  char *psw3 = password3;
  char *psw4 = password4;

  while(*d == *psw1) {
   if(*d == '\0')
    {
    printf("Found password: %s\n",password1);
      break;
    }

    d++;
    psw1++;
  }
    
  while(*i == *psw2) {
   if(*i == '\0')
    {
    printf("Found password: %s\n",password2);
      break;
}

    i++;
    psw2++;
  }

  while(*p == *psw3) {
   if(*p == '\0')
    {
    printf("Found password: %s\n",password3);
      break;
    }

    p++;
    psw3++;
  }

  while(*s == *psw4) {
   if(*s == '\0')
    {
    printf("Found password: %s\n",password4);
      return 1;
    }

    s++;
    psw4++;
  }
  return 0;

}
/****************************************************************************
  The kernel function assume that there will be only one thread and uses
  nested loops to generate all possible passwords and test whether they match
  the hidden password.
*****************************************************************************/

__global__ void  kernel() {
char e,h;
 
  char password[5];
  password[4] = '\0';

int i = blockIdx.x+65;
int j = threadIdx.x+65;
char firstValue = i;
char secondValue = j;
    
password[0] = firstValue;
password[1] = secondValue;
    for(e='0'; e<='9'; e++){
      for(h='0'; h<='9'; h++){
            password[2] = e;
            password[3] = h;
          if(is_a_match(password)) {
        //printf("Success");
          }
             else {
         //printf("tried: %s\n", password);          
            }
          }
        } 
      
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


int main() {

  struct  timespec start, finish;
  long long int time_elapsed;
  clock_gettime(CLOCK_MONOTONIC, &start);

kernel <<<26,26>>>();
  cudaThreadSynchronize();

  clock_gettime(CLOCK_MONOTONIC, &finish);
  time_difference(&start, &finish, &time_elapsed);
  printf("Time elapsed was %lldns or %0.9lfs\n", time_elapsed, (time_elapsed/1.0e9));
  return 0;
}






