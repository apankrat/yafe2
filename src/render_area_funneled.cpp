#include "render_area.h"
#include "sys/threading.h"

#include <assert.h>
	
/*
 *	with N threads,
 *	all rendered lines accumulated in a single buffer and reported
 *	back by the render_area_funneled() thread
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
		vector<thread_t> threads;
		double_vec out;
		size_t done_y;

		assert(_r && _w > 1 && _h > 1 && cb && _threads > 0);

		r = _r;
		w = _w;
		h = _w;
		area = _area;

		mutex_create(&access);
		cancel = false;
		next_y = 0;

		done.clear();
		cache.clear();

		for (size_t i = 0; i < _threads; i++)
		{
			thread_t t;
			thread_create(&t, render_thread_proxy, this);
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

			if (out.size() == w)
			{
				lock();
				cache.push_back( double_vec() );
				swap(out, cache.back());
				unlock();
			}
			else
			{
				out.clear();
			}
		}

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

	mutex_t access;
	bool     cancel;
	size_t   next_y;

	map<size_t, double_vec>  done;
	vector<double_vec>       cache;
};

/*
 *
 */
bool render_area_funneled(renderer * r, const rect_f & area, size_t w, size_t h, render_area_cb * cb, size_t threads)
{
	assert(threads > 0);
	return render_job_funneled().run(r, area, w, h, cb, threads);
}
