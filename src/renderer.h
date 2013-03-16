#ifndef _YAFE2_RENDERER_H_
#define _YAFE2_RENDERER_H_

#include "types.h"

/*
 *	types
 */
struct line_f
{
	double x1, x2;
	double y;
};

typedef vector<double> double_vec;

/*
struct rgba_t
{
	uint32_t raw;
};


/*
 *	double -> color, aka palette mapping
 *
struct rasterizer
{
	__interface(rasterizer);

	virtual bool rasterize_line(uint w, double * val, rgba_t * col) = 0;
};
*/

/*
 *
 */
struct renderer
{
	__interface(renderer);

	virtual bool render_line(const line_f & line, double_vec & out) = 0;

	virtual void dispose() = 0;
};

renderer * new_lyapunov_renderer(const string & seq, size_t warm_up_cycles, size_t main_cycles);

#endif
