#include "utils.h"

// perform a while loop for the specified duration (seconds)
void SleepFor(clock_t sec)
{
	clock_t start_time = clock();
	clock_t end_time = sec * 1000 + start_time;
		while(clock() != end_time){}
}