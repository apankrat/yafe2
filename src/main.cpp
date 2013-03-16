#include "renderer.h"
#include "render_area.h"

#include "sys/timing.h"

struct lyapunov : render_area_cb
{
	bool on_rendered_line(double_vec & line, size_t y)
	{
		printf("%4u\b\b\b\b", y);
		return true;
	}
};

int wmain(int argc, wchar_t ** argv)
{
	renderer * lyap = new_lyapunov_renderer("abbabbabababaaa", 500, 1000);
	lyapunov   cb;
	usec_t     t0, t1;

	rect_f area = { 0.0, 4.0, 0.0, 4.0 };

	for (size_t threads = 1; threads <= 16; threads++)
	{
		printf("%2u threads \t", threads);
		
		t0 = usec();
		render_area_funneled(lyap, area, 1024, 1024, &cb, threads);
		t1 = usec();

		printf("%5.3f ms (funneled) \t", (t1 - t0)/1000.);

		t0 = usec();
		render_area_passthru(lyap, area, 1024, 1024, &cb, threads);
		t1 = usec();

		printf("%5.3f ms (passthru)\n", (t1 - t0)/1000.);
	}

	lyap->dispose();

	return 0;
}
