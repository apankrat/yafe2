#include "render_area.h"
#include "sys/threading.h"

#include <assert.h>
	
/*
 *	with N threads, 
 *	cb->on_rendered_line() issued by the actual rendering thread
 */
struct render_job_passthru
{
	static void * render_thread_proxy(void * ptr)
	{
		((render_job_passthru*)ptr)->render_thread();
		return NULL;
	}

	void render_thread()
	{
		double_vec out;
		line_f     line;
		size_t     y;

		line.x1 = area.x1;
		line.x2 = area.x2;

		out.resize(w);

		lock();
		while (! cancel && next_y < h)
		{
			y = next_y++;
			unlock();

			line.y = area.y1 + (area.y2 - area.y1) * y / (h - 1);

			if (out.size() != w)    /* the callback swapped the buffer out */
				out.resize(w); 

			r->render_line(line, out);

			if (! cb->on_rendered_line(out, y))
			{
				cancel = true;
				return;
			}

			lock();
		}
		unlock();
	}

	bool run(renderer * _r, const rect_f & _area, size_t _w, size_t _h, render_area_cb * _cb, size_t _threads)
	{
		vector<thread_t> threads;

		assert(_r && _w > 1 && _h > 1 && _cb && _threads > 0);

		r = _r;
		w = _w;
		h = _w;
		area = _area;
		cb = _cb;

		mutex_create(&access);
		cancel = false;
		next_y = 0;

		for (size_t i = 0; i < _threads-1; i++)
		{
			thread_t t;
			thread_create(&t, render_thread_proxy, this);
			threads.push_back(t);
		}

		render_thread();

		for (size_t i = 0; i < threads.size(); i++)
			thread_join(threads[i], NULL);

		return !cancel;
	}

	void lock()
	{
		mutex_acquire(access);
	}

	void unlock()
	{
		mutex_release(access);
	}

	/*
	 *
	 */
	renderer * r;
	rect_f     area;
	size_t     w, h;

	render_area_cb * cb;

	mutex_t access;
	bool     cancel;
	size_t   next_y;
};

/*
 *
 */
bool render_area_passthru(renderer * r, const rect_f & area, size_t w, size_t h, render_area_cb * cb, size_t threads)
{
	assert(threads > 0);

	return render_job_passthru().run(r, area, w, h, cb, threads);
}
