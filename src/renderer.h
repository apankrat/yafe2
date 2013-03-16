#ifndef _YAFE2_RENDERER_H_
#define _YAFE2_RENDERER_H_

#include "types.h"

/*
 *
 */
struct line_f
{
	double x1, x2;
	double y;
};

typedef vector<double> double_vec;

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
