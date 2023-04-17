#include "clock.h"

void TimeClock(Time_t *pTime)
{
	pTime->seconds++;

	if (pTime->seconds >= 60)
	{
		pTime->seconds = 0;
		pTime->minutes++;
	}

	if (pTime->minutes >= 60)
	{
		pTime->minutes = 0;
		pTime->hours++;
	}

	if (pTime->hours >= 24)
	{
		pTime->hours = 0;
	}
}