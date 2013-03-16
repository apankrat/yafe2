#include "renderer.h"

#include <assert.h>
#include <math.h>

/*
 *
 */
typedef vector<size_t> size_t_vec;

/*
 *	(1) Initialization    x = x0
 *	                      For i=1 to WARM_UP:
 *	                         x = rx(1-x)
 *
 *	(2) Derivation        total = 0        
 *	    of the            For i=1 to MAIN_CYCLE:
 *	    exponent            x = rx(1-x) 
 *	                        total = total + Log|r-2rx|/Log 2
 *	                      exponent = total / MAIN_CYCLE 
 */
struct lyapunov_renderer : renderer
{
	lyapunov_renderer();
	~lyapunov_renderer();

	bool init(const string & seq, size_t warm_up, size_t main_cycle);

	bool render_line(const line_f & line, double_vec & out);

	void dispose();

	/*
	 *	the setup
	 */
	string  seq;
	size_t  warm_up;
	size_t  main_cycle;

	/*
	 *	the precomp
	 */
	double_vec  fseq;
	size_t_vec  a_index;
	size_t_vec  b_index;

	size_t      warm_up_fast;
	size_t      total_cycles;
	size_t      total_cycles_fast;
};

/*
 *
 */
lyapunov_renderer::lyapunov_renderer()
{
	warm_up = 0;
	main_cycle = 0;

	warm_up_fast = 0;
	total_cycles = 0;
	total_cycles_fast = 0;
}

lyapunov_renderer::~lyapunov_renderer()
{
}

bool lyapunov_renderer::init(const string & _seq, size_t _warm_up, size_t _main_cycle)
{
	assert(! _seq.empty() && _main_cycle > 0);

	seq = _seq;
	warm_up = _warm_up;
	main_cycle = _main_cycle;

	//
	size_t seq_n = seq.size();

	fseq.resize( seq_n );
	a_index.reserve( seq_n );
	b_index.reserve( seq_n );

	for (size_t i=0, n=seq.size(); i<n; i++)
		if (seq[i] == 'a') a_index.push_back(i);
		else               b_index.push_back(i);

	warm_up_fast = (warm_up / seq_n) * seq_n;

	total_cycles = warm_up + main_cycle;
	total_cycles_fast = (total_cycles / seq_n) * seq_n;
//	main_cycle_full = (main_cycle / seq_n) * seq_n;

	return true;
}

void lyapunov_renderer::dispose()
{
	/*
	 *	$todo - check that not called from 
	 *	render_area->cb->on_rendered_line()
	 */
	delete this;
}

bool lyapunov_renderer::render_line(const line_f & spec, double_vec & out)
{
	size_t i, j, seq_n; 
	size_t width, x;
	double b, a, da;
	double nb_log_b;

	/*
	 *
	 */
	width = out.size();
	assert(width > 1);

	/*
	 *	fill-in B spots
	 */
	b = spec.y;
	for (i=0; i<b_index.size(); i++)
		fseq[ b_index[i] ] = b;

	nb_log_b = b_index.size() * log(fabs(b));

	seq_n = fseq.size();

	da = (spec.x2 - spec.x1) / (width - 1);

	for (x = 0, a = spec.x1; x < width; x++, a += da)
	{
		double f = 0.5;
		double t = 1.0;

		/*
		 *	fill-in A spots
		 */
		for (i=0; i<a_index.size(); i++)
			fseq[ a_index[i] ] = a;

		/*
		 *	warm-up
		 */
		for (i=0; i<warm_up_fast; i += seq_n)
		{
			for (j=0; j<seq_n; j++)
				f *= fseq[j] * (1-f);
		}

		for (j=0; i<warm_up; i++, j++)
		{
			f *= fseq[j] * (1-f);
		}

		/*
		 *	main cycle, prologue - get to the seq boundary
		 */
		for ( ; j<seq_n && i<total_cycles; i++, j++)
		{
			double & r = fseq[j];

			f *= r*(1-f);
			t *= (1-f-f);
		}

		/*
		 *	main cycle, fast part
		 */
		for ( ; i < total_cycles_fast; i += seq_n)
		{
			for (j=0; j<seq_n; j++)
			{
				double & r = fseq[j];

				f *= r*(1-f);
				t *= (1-f-f);
			}				
		}

		/*
		 *	main cycle, epilogue
		 */
		for (j=0; i < total_cycles; i++, j++)
		{
			double & r = fseq[j];

			f *= r*(1-f);
			t *= (1-f-f);
		}

		/*
		 *	tally up
		 */
		out[x] = log(fabs(t)) + a_index.size() * log(fabs(a)) + nb_log_b;
	}

	return true;
}

/*
 *
 */
renderer * new_lyapunov_renderer(const string & seq, size_t warm_up, size_t main_cycle)
{
	lyapunov_renderer * lr;
	
	lr = new lyapunov_renderer();
	assert(lr);

	if (! lr->init(seq, warm_up, main_cycle))
	{
		delete lr;
		return NULL;
	}

	return lr;
}
