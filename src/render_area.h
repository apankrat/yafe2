#ifndef _RENDER_AREA_H_
#define _RENDER_AREA_H_

#include "renderer.h"

/*
 *
 */
struct rect_f
{
	double x1, x2;
	double y1, y2;
};

struct render_area_cb
{
	__interface(render_area_cb);

	virtual bool on_rendered_line(double_vec & line, size_t y) = 0;
};

bool render_area_simple  (renderer * r, const rect_f & area, size_t w, size_t h, render_area_cb * cb);
bool render_area_funneled(renderer * r, const rect_f & area, size_t w, size_t h, render_area_cb * cb, size_t threads);
bool render_area_passthru(renderer * r, const rect_f & area, size_t w, size_t h, render_area_cb * cb, size_t threads);

#endif
