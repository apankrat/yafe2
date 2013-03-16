#include "sys/threading.h"

#include <windows.h>
#include <assert.h>

/*
 *	pthread-ish threading api
 */
struct thread_ctx
{
	void * (* thread_start)(void *);
	void * thread_arg;
	void * thread_rc;

	HANDLE handle;
};

static DWORD WINAPI thread_launcher(void * ptr)
{
	thread_ctx * ctx = (thread_ctx *)ptr;
	ctx->thread_rc = ctx->thread_start(ctx->thread_arg);
	return 0;
}

int thread_create(thread_t * thread, void * (* start_routine)(void * ), void * arg)
{
	thread_ctx * ctx;
	
	ctx = new thread_ctx();
	ctx->thread_start = start_routine;
	ctx->thread_arg = arg;
	ctx->handle = CreateThread(NULL, 0, thread_launcher, ctx, 0, NULL);

	if (! ctx->handle)
	{
		delete ctx;
		return -1;
	}

	*thread = ctx;
	return 0;
}

int thread_join(thread_t ctx, void ** rc_ptr)
{
	assert(ctx);

	WaitForSingleObject(ctx->handle, -1);
	
	if (rc_ptr)
		*rc_ptr = ctx->thread_rc;

	return thread_detach(ctx);
}

int thread_detach(thread_t ctx)
{
	assert(ctx);

	CloseHandle(ctx->handle);
	delete ctx;
	return 0;
}

/*
 *
 */
struct mutex_ctx
{
	CRITICAL_SECTION cs;
};

int mutex_create(mutex_t * mutex)
{
	mutex_ctx * ctx = new mutex_ctx;
	InitializeCriticalSection(&ctx->cs);

	*mutex = ctx;
	return 0;
}

int mutex_delete(mutex_t ctx)
{
	assert(ctx);

	DeleteCriticalSection(&ctx->cs);
	delete ctx;
	return 0;
}

int mutex_acquire(mutex_t ctx)
{
	assert(ctx);
	EnterCriticalSection(&ctx->cs);
	return 0;
}

int mutex_release(mutex_t ctx)
{
	assert(ctx);
	LeaveCriticalSection(&ctx->cs);
	return 0;
}

/*
 *
 */
void yield()
{
	Sleep(0);
}
