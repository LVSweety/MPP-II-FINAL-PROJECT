#include "countdown.h"

void TimeCountdown(Time_t *pTime)
{
	pTime->miliseconds--;

	if (pTime->miliseconds < 0)
	{
		pTime->miliseconds = 100;
		pTime->seconds--;
	}

	if (pTime->seconds < 0)
	{
		pTime->seconds = 59;
		pTime->minutes--;
	}

	if (pTime->minutes < 0)
	{
		pTime->minutes = 59;
		pTime->hours--;
	}

	if (pTime->hours < 0)
	{
		pTime->miliseconds = 0;
		pTime->seconds = 0;
		pTime->minutes = 0;
		pTime->hours = 0;
	}
}