#include "render_area.h"

#include <assert.h>
	
/*
 *	line-by-line with a single thread
 */
bool render_area(renderer * r, const rect_f & area, size_t w, size_t h, render_area_cb * cb)
{
	line_f     line;
	double_vec out;

	assert(h > 1); /* use render_line() otherwise */

	line.x1 = area.x1;
	line.x2 = area.x2;

	out.resize(w);

	for (size_t y=0; y<h; y++)
	{
		line.y = area.y1 + (area.y2 - area.y1) * y / (h - 1);

		if (! r->render_line(line, out))
			return false;

		if (! cb->on_rendered_line(out, y))
			return false;
	}
	return true;
}
