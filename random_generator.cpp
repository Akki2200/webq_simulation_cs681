#include <cstdlib>
#include <iostream>
#include <cfloat>
#include <cmath>
// #include "lcgrand.h"
// #include <time.h>
using namespace std;

// float global_clock = 0.0f;
// float global_clock_end_time = 500.0f;

float generate_next_event_time(float next_peak, float next_trough, float global_clock){

	float rate = 1.0f;

	float next_event_time = 0.0f;

	if(global_clock < next_peak)
		next_event_time = (rand()/(float)RAND_MAX * (next_peak - global_clock));
	else if (global_clock == next_peak) //(global_clock - next_peak < 100*FLT_EPSILON)
		next_event_time = rand()/(float)RAND_MAX;
	else
		next_event_time = (rand()/(float)RAND_MAX * (global_clock - next_peak));

	// srand(time(NULL));
	
	return next_event_time;
}

float expon(float mean)  /* Exponential variate generation function. */
{
    /* Return an exponential random variate with mean "mean". */

    return -mean * log((rand()/(float)RAND_MAX));
}

// int main(){

	
// 	while(global_clock < global_clock_end_time){
// 		float peak_time= global_clock + rand() % 100;
// 		float trough_time= global_clock + peak_time + rand() % 100;

// 		while(global_clock < trough_time){
		
// 		global_clock += generate_next_event_time(peak_time,trough_time,global_clock);
// 		cout<<global_clock<<endl;

// 			}
// 		}

// 	return 0;
// }