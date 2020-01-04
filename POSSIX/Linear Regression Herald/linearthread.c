#include <stdio.h>
#include <math.h>
#include <time.h>
#include <pthread.h>


int i;
  double bm = 1.3;
  double bc = 10;
  double be;
  double dm[8];
  double dc[8];
  double e[8];
  double step = 0.01;
  double best_error = 999999999;
  int best_error_i;
  int minimum_found = 0;
  double om[] = {0,1,1, 1, 0,-1,-1,-1};
  double oc[] = {1,1,0,-1,-1,-1, 0, 1};

typedef struct point_t {
  double x;
  double y;
} point_t;

int n_data = 1000;
point_t data[];

double residual_error(double x, double y, double m, double c) {
  double e = (m * x) + c - y;
  return e * e;
}

double rms_error(double m, double c) {
  int i;
  double mean;
  double error_sum = 0;
  
  for(i=0; i<n_data; i++) {
    error_sum += residual_error(data[i].x, data[i].y, m, c);  
  }
  
  mean = error_sum / n_data;
  
  return sqrt(mean);
}
int time_difference(struct timespec *start, struct timespec *finish, long long int *difference)
{
  long long int ds =  finish->tv_sec - start->tv_sec; 
  long long int dn =  finish->tv_nsec - start->tv_nsec; 

  if(dn < 0 ) {
    ds--;
    dn += 1000000000; 
  } 
  *difference = ds * 1000000000 + dn;
  return !(*difference > 0);
}

void *linear_regression_thread(void *args){

  int *a = args;
  int i = *a;

  dm[i] = bm +(om[i] * step);
      dc[i] = bc + (oc[i] * step);
  e[i] = rms_error(dm[i], dc[i]);
  if(e[i] < best_error) {
  best_error = e[i];
  best_error_i = i;
  pthread_exit(NULL);
  }
}  

int main() {
   struct timespec start, finish;   
  long long int time_elapsed;
  clock_gettime(CLOCK_MONOTONIC, &start);

  int i;
  pthread_t p_threads[0];
  
  be = rms_error(bm, bc);

  while(!minimum_found) {
    for(i=0;i<8;i++) {
      pthread_create(&p_threads[i], NULL, (void*)linear_regression_thread, &i);
      pthread_join(p_threads[i], NULL); 
    }

    if(best_error < be) {
      be = best_error;
      bm = dm[best_error_i];
      bc = dc[best_error_i];
    } else {
      minimum_found = 1;
    }
  }
  printf("minimum m,c is %lf,%lf with error %lf\n", bm, bc, be);
  clock_gettime(CLOCK_MONOTONIC, &finish);
  time_difference(&start, &finish, &time_elapsed);
  printf("Time elapsed was %lldns or %0.9lfs\n", time_elapsed, 
         (time_elapsed/1.0e9)); 
  
  return 0;
}

point_t data[] = {
  {70.49,130.80},{69.15,120.96},{73.09,128.52},{86.47,121.82},
  {79.33,135.78},{82.97,146.94},{78.52,137.85},{73.39,136.96},
  {75.61,114.34},{79.52,146.23},{76.70,133.10},{65.33,122.13},
  {79.50,129.38},{85.38,157.06},{72.33,133.95},{35.91,75.40},
  {80.77,143.67},{52.68,89.78},{36.13,77.22},{83.90,142.29},
  {59.04,106.05},{71.12,130.59},{32.06,71.46},{40.77,93.83},
  {46.91,81.58},{94.85,156.72},{12.06,43.28},{ 0.71,30.59},
  {21.39,61.08},{88.48,157.99},{50.25,118.75},{31.08,96.31},
  {58.31,100.58},{27.50,76.70},{10.69,38.32},{95.66,150.32},
  {15.97,68.61},{80.36,139.70},{71.97,130.69},{ 7.44,39.10},
  {74.91,138.56},{ 5.87,38.76},{38.96,94.86},{ 8.20,38.92},
  {57.23,103.56},{29.79,82.30},{64.80,112.26},{20.84,60.04},
  {82.50,136.68},{12.69,56.76},{ 7.73,41.58},{79.38,150.25},
  {99.73,182.45},{64.77,107.66},{38.09,78.58},{75.72,143.27},
  {93.61,171.88},{46.59,116.39},{31.18,76.09},{89.36,167.09},
  {57.97,100.04},{30.65,59.97},{57.40,107.18},{78.58,141.45},
  {28.35,66.02},{72.36,132.13},{12.30,50.43},{42.29,86.56},
  {42.23,74.52},{91.38,147.51},{35.52,95.32},{75.56,144.53},
  {92.61,164.53},{21.57,81.55},{81.52,146.53},{62.55,108.13},
  {45.86,94.30},{27.38,56.80},{98.47,164.28},{79.60,132.17},
  {49.49,101.96},{99.59,171.38},{91.11,158.91},{52.61,99.92},
  {51.00,105.16},{46.34,97.15},{60.72,90.68},{53.70,116.41},
  {16.92,71.76},{75.14,129.96},{56.44,114.93},{63.62,122.81},
  {87.88,145.18},{93.38,150.65},{24.15,70.67},{70.89,125.06},
  {57.82,120.58},{27.41,77.74},{35.24,76.88},{75.73,146.45},
  {72.88,135.76},{56.38,118.97},{ 9.35,53.94},{ 6.65,43.07},
  {92.78,145.83},{60.23,103.33},{82.59,138.65},{82.59,147.20},
  { 5.38,47.28},{ 9.85,58.44},{45.16,104.55},{20.85,57.53},
  {93.27,159.89},{56.15,94.30},{34.84,84.45},{49.78,105.58},
  {24.81,73.02},{12.36,45.98},{51.76,116.24},{14.80,50.91},
  {58.76,114.94},{ 4.65,34.34},{74.99,126.14},{45.12,99.17},
  {75.16,120.98},{20.96,59.97},{62.03,120.94},{29.02,57.61},
  {50.10,109.93},{25.70,67.21},{53.37,121.01},{69.83,106.86},
  {98.40,167.39},{62.79,103.27},{73.46,129.78},{94.55,146.92},
  {71.17,128.64},{ 9.89,61.96},{71.64,99.54},{ 4.08,39.81},
  {86.30,139.84},{57.12,119.67},{23.03,69.35},{13.05,50.76},
  {87.04,165.95},{ 6.30,42.04},{46.82,111.49},{29.52,74.86},
  {57.05,102.30},{44.34,107.69},{85.27,135.83},{10.74,47.09},
  {45.03,90.95},{35.25,98.93},{49.98,101.17},{62.33,110.82},
  {83.44,128.79},{49.31,80.08},{98.90,162.01},{73.81,113.52},
  {17.62,50.47},{63.13,116.08},{13.46,51.40},{27.67,70.81},
  { 5.54,38.33},{83.67,145.25},{49.77,93.82},{44.96,117.71},
  {90.09,165.72},{29.28,76.51},{ 2.22,36.30},{ 6.36,32.50},
  {96.08,156.79},{62.72,119.35},{ 2.24,32.55},{85.64,154.80},
  {76.93,148.22},{ 3.74,32.51},{80.07,129.92},{ 7.57,47.93},
  { 9.00,52.97},{72.69,123.67},{72.76,149.85},{45.74,103.78},
  {66.00,121.69},{59.09,124.34},{ 4.60,47.04},{69.41,133.60},
  { 9.09,63.43},{45.74,103.93},{56.63,111.44},{76.34,136.06},
  {97.77,165.84},{80.20,147.33},{16.71,43.05},{13.95,65.55},
  {96.06,172.59},{ 5.44,48.49},{12.06,58.95},{36.61,73.82},
  {29.29,68.38},{37.42,90.24},{50.49,95.83},{47.45,110.52},
  {40.96,92.52},{38.24,78.03},{55.77,105.52},{42.17,93.48},
  {40.53,82.97},{22.55,73.32},{20.23,65.30},{ 1.88,54.37},
  { 6.84,45.82},{73.05,130.63},{83.94,158.91},{90.96,161.00},
  {10.94,41.62},{78.06,146.69},{45.97,112.26},{79.55,133.24},
  { 0.25,28.96},{98.80,171.38},{95.77,176.06},{ 0.82,31.52},
  {40.03,96.99},{22.06,81.54},{21.25,57.42},{26.64,60.34},
  { 3.87,29.19},{79.36,135.16},{88.96,145.04},{90.13,146.75},
  {42.63,85.20},{68.11,135.21},{94.64,163.26},{31.01,76.30},
  {78.95,132.19},{89.73,153.83},{24.83,65.97},{18.69,50.48},
  {45.29,74.50},{ 2.68,37.48},{75.57,134.16},{37.04,97.58},
  {53.59,88.16},{66.96,141.13},{ 8.31,61.03},{ 4.53,57.53},
  {41.66,97.85},{72.11,132.50},{71.86,138.16},{72.87,121.14},
  {87.34,143.48},{95.03,141.95},{85.67,167.74},{83.99,148.48},
  {84.18,136.06},{59.05,110.64},{75.45,148.87},{10.60,48.52},
  {81.85,144.43},{29.44,76.63},{10.76,52.81},{46.80,103.01},
  {10.39,47.90},{35.43,82.97},{11.96,52.36},{41.33,74.35},
  {34.32,100.25},{90.90,160.50},{89.02,144.90},{32.94,76.30},
  {35.15,67.46},{49.81,105.50},{58.97,114.68},{61.15,85.18},
  {53.52,105.59},{80.53,136.80},{ 8.25,57.08},{88.87,142.03},
  {63.05,116.95},{50.19,110.48},{32.94,72.63},{75.91,128.73},
  {98.59,162.18},{ 6.66,53.32},{66.68,116.98},{46.53,95.66},
  {80.56,157.54},{ 4.04,39.16},{84.05,142.18},{ 8.82,51.97},
  {94.05,154.96},{14.54,71.12},{51.22,96.16},{53.00,102.82},
  {65.99,122.14},{80.88,146.18},{ 2.80,28.08},{62.66,127.25},
  {81.08,135.03},{11.56,58.03},{46.56,85.84},{48.72,108.46},
  {34.11,77.80},{25.72,69.80},{91.39,149.30},{54.37,99.88},
  {46.36,107.12},{35.15,75.41},{57.05,105.71},{41.58,83.45},
  {22.83,62.83},{23.79,76.76},{53.68,97.62},{40.51,85.35},
  {50.73,98.67},{64.43,125.08},{92.16,155.77},{56.76,110.60},
  {20.14,61.83},{72.47,131.58},{78.09,141.29},{ 3.64,32.93},
  { 6.73,65.31},{30.47,75.97},{44.28,85.69},{96.01,159.51},
  {10.44,53.52},{45.94,100.93},{35.94,92.95},{79.84,148.83},
  {42.10,83.36},{48.75,107.21},{66.31,129.78},{ 8.26,45.71},
  {19.01,57.81},{12.89,52.33},{34.53,79.61},{57.75,104.38},
  {47.06,97.03},{41.79,96.89},{21.96,67.98},{29.73,75.70},
  { 6.13,26.46},{13.22,76.73},{66.13,103.21},{18.58,62.21},
  {30.37,42.98},{20.71,54.73},{63.10,130.17},{52.73,105.14},
  {38.51,89.64},{10.37,52.75},{13.14,53.41},{23.17,57.08},
  {96.43,158.01},{71.44,124.79},{38.76,93.08},{50.23,112.39},
  {84.90,144.26},{37.25,88.01},{ 6.49,42.67},{64.16,100.96},
  { 1.50,44.62},{29.76,60.69},{67.03,111.56},{31.42,70.82},
  {85.35,142.18},{59.23,107.16},{64.07,104.07},{90.84,151.87},
  {75.77,135.87},{59.10,99.71},{88.05,157.91},{25.65,70.84},
  {95.05,154.88},{65.83,127.48},{90.27,149.50},{15.15,43.70},
  {83.34,130.41},{53.43,118.60},{68.00,121.65},{95.21,160.78},
  {74.74,150.54},{42.66,79.34},{25.30,53.27},{25.99,85.53},
  {43.21,90.96},{13.26,49.37},{41.67,77.87},{47.19,88.81},
  {90.32,166.44},{19.76,60.65},{81.70,130.94},{88.04,138.64},
  {32.64,87.28},{52.97,94.55},{ 6.54,44.90},{12.62,62.43},
  { 1.99,60.37},{24.10,66.02},{39.88,97.35},{85.36,149.25},
  {89.75,149.66},{85.48,135.83},{64.70,115.71},{99.39,155.02},
  {66.68,111.85},{88.72,146.10},{24.89,49.71},{97.13,163.31},
  { 8.04,42.95},{72.37,123.91},{ 6.28,47.33},{ 9.04,36.47},
  {27.92,52.38},{76.89,125.98},{86.88,149.43},{62.49,113.11},
  {75.93,136.43},{39.81,88.85},{37.76,78.30},{48.62,105.76},
  {11.06,47.21},{21.55,64.30},{18.34,65.05},{60.29,94.85},
  {97.17,144.12},{55.76,94.79},{74.14,112.63},{50.67,102.12},
  {78.33,147.34},{87.66,139.50},{95.28,142.74},{15.79,60.86},
  {51.55,86.75},{66.70,117.50},{16.42,74.91},{96.96,173.36},
  {72.45,117.10},{82.60,150.17},{67.67,130.28},{ 1.46,40.37},
  {65.45,118.92},{80.27,139.24},{88.31,144.56},{77.83,139.24},
  {16.50,61.86},{ 3.68,43.30},{86.42,146.72},{82.20,144.05},
  {60.26,122.63},{35.91,79.84},{ 6.38,36.90},{61.15,133.48},
  {75.59,130.90},{66.25,122.05},{39.81,65.67},{22.03,72.67},
  {49.81,97.68},{42.75,72.90},{79.72,135.65},{14.02,43.73},
  {50.97,113.88},{25.92,75.34},{71.34,131.50},{90.16,159.00},
  {90.00,173.48},{ 5.93,32.51},{93.47,164.66},{80.15,137.14},
  {96.50,161.72},{ 5.22,36.44},{59.09,127.71},{67.61,142.50},
  {37.95,72.89},{36.28,80.51},{ 1.75,37.11},{32.50,81.37},
  {68.29,110.92},{ 3.19,38.92},{10.42,47.43},{23.38,68.25},
  { 1.24,39.85},{95.36,147.53},{14.70,39.25},{16.27,49.69},
  {78.54,121.12},{20.68,61.07},{89.24,153.20},{37.41,92.99},
  {31.54,72.54},{ 9.04,36.12},{71.16,157.70},{40.54,101.05},
  {87.40,146.76},{40.03,64.89},{65.93,106.88},{51.99,91.49},
  {30.11,69.89},{ 4.20,29.63},{72.94,121.91},{84.03,140.15},
  {18.21,65.22},{22.75,72.70},{ 5.03,62.94},{84.19,121.12},
  {49.73,109.18},{50.97,96.52},{17.84,61.19},{22.23,63.79},
  {98.64,161.96},{47.67,98.78},{95.89,164.82},{17.60,40.57},
  {19.55,60.32},{39.65,100.65},{78.04,145.50},{21.25,57.30},
  {75.44,132.79},{20.74,51.29},{99.76,167.69},{24.02,68.15},
  {83.83,144.62},{28.83,79.70},{81.39,140.50},{54.20,114.42},
  {65.66,114.08},{38.43,82.74},{45.69,81.81},{30.16,71.89},
  { 5.60,54.27},{83.32,146.93},{11.91,37.69},{72.86,145.12},
  {94.26,157.64},{77.50,145.38},{28.53,70.20},{62.64,144.67},
  {46.98,87.65},{17.94,66.43},{94.83,154.61},{70.00,115.57},
  {81.49,146.60},{53.42,112.37},{73.41,122.83},{28.85,77.99},
  {61.51,103.53},{ 9.43,45.86},{61.79,112.81},{22.91,62.04},
  {18.97,73.47},{71.89,125.20},{21.33,49.19},{60.95,107.95},
  {50.48,100.19},{44.09,102.06},{90.72,162.02},{54.67,95.87},
  {80.13,146.92},{19.49,64.20},{22.27,51.03},{65.80,125.90},
  {84.97,142.32},{61.33,129.41},{81.98,151.09},{41.93,94.51},
  {69.72,122.51},{20.44,59.72},{52.94,92.47},{53.87,108.83},
  {66.10,131.38},{53.89,118.39},{90.61,141.08},{ 1.48,43.49},
  {55.65,104.78},{15.90,60.60},{46.88,105.80},{64.44,112.85},
  {52.33,117.11},{85.09,153.46},{73.22,115.56},{ 3.81,49.12},
  {10.66,30.84},{23.27,55.05},{48.66,109.58},{23.29,50.31},
  {88.92,150.73},{26.52,73.72},{65.10,115.43},{17.14,69.33},
  {90.44,164.86},{40.92,92.54},{29.13,54.22},{36.01,92.08},
  {62.79,95.56},{21.66,69.26},{41.24,83.40},{22.49,75.67},
  {60.91,120.06},{94.45,165.13},{13.20,56.43},{59.92,90.43},
  {39.19,80.79},{76.59,139.35},{36.67,81.34},{11.06,32.61},
  {88.81,151.04},{44.19,86.43},{98.74,170.51},{14.20,57.27},
  { 0.12,34.46},{80.95,146.47},{80.91,137.87},{41.60,89.96},
  {74.73,146.95},{10.15,34.76},{99.40,156.65},{ 2.58,40.48},
  {97.86,172.37},{78.82,139.27},{58.57,109.60},{96.57,169.35},
  {79.00,152.23},{39.99,94.14},{66.95,126.50},{59.33,105.83},
  {13.71,60.63},{45.88,100.72},{ 5.73,42.26},{73.24,138.38},
  {18.70,59.33},{44.16,103.88},{18.93,63.40},{ 8.89,56.46},
  {64.87,119.64},{59.27,128.50},{65.70,125.98},{31.45,76.90},
  {47.62,106.65},{55.24,102.65},{66.98,129.90},{67.20,120.15},
  {82.89,160.45},{87.63,156.09},{86.84,154.94},{49.71,106.31},
  {81.13,141.18},{83.95,148.70},{24.82,68.16},{ 6.29,36.96},
  {45.53,100.22},{54.86,118.40},{20.11,73.23},{36.27,77.63},
  {34.99,87.72},{82.93,147.98},{15.79,47.57},{16.52,38.24},
  {41.72,91.70},{88.28,162.99},{41.99,86.34},{19.14,71.88},
  {46.82,92.30},{63.26,119.18},{95.62,168.26},{16.65,53.28},
  {37.05,97.31},{23.12,52.65},{94.77,164.76},{92.08,141.33},
  {73.24,117.79},{26.84,57.89},{79.50,144.53},{ 4.19,28.60},
  {72.43,135.74},{53.96,102.81},{34.51,71.36},{ 8.26,36.34},
  {70.16,133.65},{58.46,96.95},{95.49,147.11},{61.54,129.50},
  {53.80,99.09},{20.07,70.56},{92.32,161.17},{77.15,131.94},
  {13.48,47.35},{98.88,169.61},{54.80,84.26},{29.52,77.65},
  {46.78,81.14},{50.98,100.42},{34.22,71.59},{92.79,162.00},
  {41.44,107.55},{65.00,105.16},{25.10,75.73},{ 5.68,47.49},
  {55.63,122.32},{59.70,105.98},{ 0.83,18.38},{93.49,170.66},
  {74.24,125.12},{21.73,56.04},{69.00,129.79},{74.33,131.77},
  {87.29,162.96},{49.45,108.64},{39.85,95.13},{65.94,128.56},
  {96.42,164.68},{75.72,135.47},{74.00,128.64},{22.69,79.03},
  {16.49,49.83},{51.75,105.92},{18.35,39.89},{12.11,47.51},
  {11.40,53.07},{42.69,75.97},{34.09,90.98},{89.58,138.92},
  {61.38,116.03},{15.99,53.12},{51.36,98.31},{ 8.43,41.23},
  {99.48,160.38},{28.02,72.25},{18.56,67.65},{20.40,75.66},
  { 9.16,51.61},{16.99,69.22},{16.63,62.37},{98.07,172.62},
  { 2.13,37.96},{34.13,90.66},{46.26,90.77},{91.73,155.07},
  {38.47,84.49},{62.03,123.28},{22.39,52.12},{32.11,73.83},
  {90.83,141.57},{55.57,125.48},{31.37,72.59},{74.83,150.41},
  {84.81,158.26},{68.49,137.47},{28.18,66.69},{30.45,95.28},
  {35.25,85.16},{68.88,111.04},{69.27,138.32},{99.21,173.21},
  {12.99,44.58},{33.35,93.76},{51.33,90.40},{61.72,112.41},
  {59.57,115.05},{68.79,118.10},{43.68,103.93},{28.34,72.36},
  {65.11,117.06},{80.55,143.77},{19.12,65.14},{19.35,73.32},
  { 5.25,43.43},{61.76,111.72},{72.75,138.36},{57.36,101.49},
  {49.69,113.93},{86.72,139.78},{87.23,144.77},{82.63,140.81},
  {86.35,146.08},{85.91,147.89},{98.85,174.96},{92.35,159.01},
  {25.75,70.99},{39.70,81.86},{ 3.86,33.06},{61.49,112.99},
  {55.07,136.48},{70.31,120.27},{74.20,122.49},{76.62,139.04},
  {59.92,107.95},{67.72,150.15},{90.39,159.74},{56.12,99.84},
  {25.27,65.76},{47.30,88.16},{87.88,125.22},{66.52,121.60},
  {56.18,105.57},{23.84,65.30},{47.42,103.93},{14.72,54.60},
  {55.42,93.79},{72.59,123.12},{97.52,153.87},{57.87,111.74},
  {16.32,67.04},{61.16,108.39},{10.41,59.99},{21.46,50.16},
  {88.81,161.65},{87.42,146.69},{58.95,125.92},{76.51,138.78},
  { 9.07,60.01},{23.03,70.96},{ 0.74,43.37},{94.22,142.83},
  {39.50,74.27},{ 9.36,54.88},{39.38,108.91},{47.11,97.19},
  { 8.02,27.25},{ 2.14,30.21},{ 2.24,47.15},{28.53,75.91},
  {53.16,116.06},{67.95,131.53},{39.90,96.56},{ 4.89,46.30},
  {96.71,151.69},{52.71,86.57},{72.33,127.71},{57.81,113.64},
  {20.66,50.51},{60.82,122.96},{52.86,93.88},{14.65,64.75},
  {74.36,132.70},{46.84,81.11},{ 3.79,32.15},{39.85,87.00},
  {42.20,88.52},{78.22,130.13},{93.58,152.52},{57.03,92.98},
  {26.96,71.50},{ 3.42,36.65},{ 2.61,34.84},{88.96,150.75},
  {92.04,157.28},{51.04,108.19},{59.44,120.82},{55.34,95.53},
  {41.00,96.36},{59.79,131.01},{30.89,63.48},{43.47,90.16},
  {18.84,72.90},{42.70,78.42},{44.85,90.97},{41.23,99.03},
  {16.14,52.82},{10.22,69.66},{86.11,150.33},{43.47,96.18},
  {97.45,180.39},{31.67,77.81},{75.57,130.89},{16.87,45.23},
  { 6.68,42.93},{11.99,46.31},{93.15,165.13},{25.97,61.79},
  { 1.98,52.17},{50.93,91.84},{19.96,38.01},{51.04,110.55},
  { 2.94,44.35},{38.64,78.52},{87.43,142.52},{67.31,141.90},
  {97.56,162.61},{23.24,58.72},{88.40,126.06},{97.41,152.38},
  { 8.99,60.09},{62.95,121.42},{39.19,78.97},{68.34,124.26},
  {67.92,126.91},{18.55,59.65},{ 0.52,42.03},{63.22,127.39},
  {61.12,108.44},{38.83,76.44},{75.92,123.50},{24.70,61.13},
  {34.53,63.04},{30.55,69.85},{93.81,158.14},{17.02,58.94},
  {39.86,86.69},{13.91,43.15},{43.07,80.31},{14.22,52.39},
  {28.01,64.04},{17.66,51.30},{64.87,127.50},{68.69,129.09},
  { 3.99,46.66},{27.77,79.85},{82.46,133.97},{11.77,51.57},
  { 3.29,42.13},{28.30,80.83},{56.98,102.61},{41.17,97.33},
  {50.10,94.36},{89.95,144.63},{13.52,43.10},{38.27,106.86},
  {29.52,59.80},{78.72,146.92},{34.18,96.12},{85.06,152.50},
  {79.77,122.94},{36.97,84.69},{16.15,48.64},{80.74,110.63},
  {73.75,133.85},{98.49,171.85},{22.60,60.53},{49.58,112.72},
  {35.70,75.85},{55.94,117.99},{21.88,51.47},{14.56,45.53},
  {12.98,48.68},{61.74,108.64},{84.13,156.45},{10.53,63.37},
  {67.73,117.73},{28.39,78.10},{83.21,138.03},{76.86,135.79},
  {67.45,121.59},{54.79,102.89},{87.09,145.27},{78.89,141.41},
  {93.95,154.16},{82.44,149.57},{46.98,99.33},{52.73,110.86},
  {74.92,127.56},{18.70,67.47},{28.05,67.85},{17.31,50.26},
  {51.58,107.92},{ 6.23,51.92},{ 3.91,30.74},{69.02,125.15},
  {80.46,138.83},{35.14,80.49},{92.95,163.01},{ 8.26,53.66},
  {39.88,96.76},{55.01,105.77},{55.70,105.97},{ 7.84,49.25},
  { 7.46,32.19},{ 6.66,43.31},{82.11,133.48},{87.68,144.55},
  { 9.06,45.72},{50.11,90.64},{85.47,162.49},{53.97,96.09},
  { 3.95,43.61},{70.93,114.76},{63.70,121.28},{12.35,41.48},
  {61.28,108.55},{36.19,71.01},{ 5.82,46.69},{31.71,88.30},
  {70.95,121.80},{28.23,69.52},{ 7.46,38.60},{85.07,137.40},
  {38.88,85.77},{41.81,81.44},{ 9.77,46.36},{84.85,146.87},
  {49.52,113.65},{58.38,108.35},{19.87,65.23},{71.50,130.83},
  {71.13,127.30},{80.05,139.42},{27.85,76.60},{37.16,76.01}
};
