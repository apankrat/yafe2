#include "render_area.h"
#include "sys/windows/threading.h"

#include <assert.h>
	
/*
 *
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

/*
 *
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
		vector<xthread_t> threads;

		assert(_r && _w > 1 && _h > 1 && _cb && _threads > 0);

		r = _r;
		w = _w;
		h = _w;
		area = _area;
		cb = _cb;

		xmutex_create(&access);
		cancel = false;
		next_y = 0;

		for (size_t i = 0; i < _threads-1; i++)
		{
			xthread_t t;
			xthread_create(&t, render_thread_proxy, this);
			threads.push_back(t);
		}

		render_thread();

		for (size_t i = 0; i < threads.size(); i++)
			xthread_join(threads[i], NULL);

		return !cancel;
	}

	void lock()
	{
		xmutex_acquire(access);
	}

	void unlock()
	{
		xmutex_release(access);
	}

	/*
	 *
	 */
	renderer * r;
	rect_f     area;
	size_t     w, h;

	render_area_cb * cb;

	xmutex_t access;
	bool     cancel;
	size_t   next_y;
};

/*
 *
 */
struct render_job_funneled
{
	static void * render_thread_proxy(void * ptr)
	{
		((render_job_funneled*)ptr)->render_thread();
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

			/*
			 *	render
			 */
			line.y = area.y1 + (area.y2 - area.y1) * y / (h - 1);
			r->render_line(line, out);

			/*
			 *	upload the result
			 */
			lock();
			swap(done[y], out);

			/*
			 *	grab new buffer
			 */
			if (cache.size())
			{
				swap(cache.back(), out);
				cache.pop_back();
			}
			else
			{
				out.resize(w);
			}
		}
		unlock();
	}

	bool run(renderer * _r, const rect_f & _area, size_t _w, size_t _h, render_area_cb * cb, size_t _threads)
	{
		vector<xthread_t> threads;
		double_vec out;
		size_t done_y;

		assert(_r && _w > 1 && _h > 1 && cb && _threads > 0);

		r = _r;
		w = _w;
		h = _w;
		area = _area;

		xmutex_create(&access);
		cancel = false;
		next_y = 0;

		done.clear();
		cache.clear();

		for (size_t i = 0; i < _threads; i++)
		{
			xthread_t t;
			xthread_create(&t, render_thread_proxy, this);
			threads.push_back(t);
		}

		for (done_y = 0; done_y < h; )
		{
			lock();

			if (done.empty() || done.begin()->first != done_y)
			{
				unlock();
				yield();
				continue;
			}

			swap(out, done.begin()->second);
			done.erase(done.begin());
			unlock();

			if (! cb->on_rendered_line(out, done_y))
			{
				cancel = true;
				break;
			}

			done_y++;

			lock();
			cache.push_back( double_vec() );
			swap(out, cache.back());
			unlock();
		}

		for (size_t i = 0; i < threads.size(); i++)
			xthread_join(threads[i], NULL);

		return !cancel;
	}

	void lock()
	{
		xmutex_acquire(access);
	}

	void unlock()
	{
		xmutex_release(access);
	}

	/*
	 *
	 */
	renderer * r;
	rect_f     area;
	size_t     w, h;

	xmutex_t access;
	bool     cancel;
	size_t   next_y;

	map<size_t, double_vec>  done;
	vector<double_vec>       cache;
};

bool render_area(renderer * r, const rect_f & area, size_t w, size_t h, render_area_cb * cb, size_t threads, bool same_thread_cb)
{
	if (threads == 0)
		return render_area(r, area, w, h, cb);

	if (same_thread_cb)
		return render_job_funneled().run(r, area, w, h, cb, threads);

	return render_job_passthru().run(r, area, w, h, cb, threads);
}
