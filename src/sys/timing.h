#ifndef _YAFE2_TIMING_H_
#define _YAFE2_TIMING_H_

#include "..\types.h"

#include <assert.h>
#include <time.h>

/*	
 *	tick_t represents a timestamp on a sub-second scale. It's a base
 *	class for both milli- and microsecond time interval measurement.
 *
 *	The zero point of the scale is undefined and may vary between 
 *	application runs. Therefore tick_t values should be used only 
 *	for measuring *intervals*.
 *
 *	tick_t is guaranteed to be at least 32 bits long. This limits 
 *	maximum interval to (2^31-1) ticks - a bit over 24 days for the 
 *	millisecond scale and 35 minutes for the microsecond scale.
 * 
 *	Wrapping of the tick_t value is handled transparently by trimming
 *	maximum measurable interval to 31 bits. This means that if A has
 *	a value of 0x03 and B is 0xFFFFFFF7, then the (A - B) interval is 
 *	12 even though A is numerically smaller than B. Same idea is used
 *	when *comparing* A and B. 
 *
 *	See Knuth for details.
 */
template <int _>
struct tick_t
{
	uint_t raw;

	/*
	 *	comparison
	 */
	bool operator < (const tick_t<_> & x) const
	{
		return ((int)raw - (int)x.raw) < 0;
	}

	bool operator <= (const tick_t<_> & x) const
	{
		return ((int)raw - (int)x.raw) <= 0;
	}

	/*
	 *	difference -> interval
	 */
	uint_t operator - (const tick_t<_> & x) const
	{
		assert( (int)raw - (int)x.raw >= 0 );
		return (raw - x.raw);
	}

	/*
	 *	adjustments
	 */
	void operator ++ (int)
	{
		raw++;
	}

	tick_t<_> operator + (int delta)
	{
		tick_t<_> r;
		r.raw = raw + delta;
		return r;
	}
};

/*
 *	API
 */
typedef tick_t<0> msec_t;
typedef tick_t<1> usec_t;

msec_t msec();
usec_t usec();

#endif
