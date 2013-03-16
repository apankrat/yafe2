#include "sys/timing.h"

#include <windows.h>

static
uint_t tick(uint64_t scale)
{
	static LARGE_INTEGER freq;
	static LARGE_INTEGER zero;

	LARGE_INTEGER now;

	QueryPerformanceCounter(&now);

	if (! freq.QuadPart)
	{
		QueryPerformanceFrequency(&freq);
		zero = now; 
		MemoryBarrier();
	}

	return (uint_t)( scale * (now.QuadPart - zero.QuadPart) / freq.QuadPart );
}

usec_t usec()
{
	usec_t us;
	/* 
	 *	-60000 shifts zero point 60 seconds into the 
	 *	future, helps catching various interesting errors
	 */
	us.raw = tick(1000*1000) - 60*1000*1000;
	return us;
}

msec_t msec() 
{ 
	msec_t ms;

	/* 
	 *	-30000 shifts zero point 30 seconds into the 
	 *	future, helps catching various interesting errors
	 */
	ms.raw = tick(1000) - 30*1000;
	return ms;
}
