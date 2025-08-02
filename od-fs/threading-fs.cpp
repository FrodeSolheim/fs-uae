#include "sysconfig.h"
#include "sysdeps.h"

#include "threaddep/thread.h"
#include "uae.h"

#include <SDL3/SDL.h>

// void uae_sem_init (uae_sem_t * event, int manual_reset, int initial_state)
int uae_sem_init (uae_sem_t * event, int manual_reset, int initial_state)
{
#warning uae_sem is not completely compatible (?) with WinUAE version...
	if(*event) {
		write_log(_T("WARNING: uae_sem_init used with existing event (?)\n"));
#if 0
		if (initial_state)
			SetEvent (*event);
		else
			ResetEvent (*event);
	} else {
		// *event = CreateEvent (NULL, manual_reset, initial_state, NULL);
		SDL_Semaphore *sem = SDL_CreateSemaphore(initial_state);
#endif
	}
	if (manual_reset) {
		write_log(_T("WARNING: uae_sem_init used with manual_reset\n"));
	}
	SDL_Semaphore *sem = SDL_CreateSemaphore(initial_state);
	*event = sem;
	return (sem == NULL);
}

void uae_sem_wait (uae_sem_t * event)
{
	SDL_WaitSemaphore((SDL_Semaphore *) *event);
}

void uae_sem_unpost (uae_sem_t * event)
{
	write_log(_T("WARNING: uae_sem_unpost used\n"));
	// ResetEvent(*event);
}

void uae_sem_post (uae_sem_t * event)
{
	SDL_SignalSemaphore((SDL_Semaphore *) *event);
}

int uae_sem_trywait_delay(uae_sem_t * event, int millis)
{
	bool result = SDL_WaitSemaphoreTimeout((SDL_Semaphore *) *event, millis);
	if (result) {
		return 0;
	}
	return -1;
}

int uae_sem_trywait (uae_sem_t * event)
{
	return uae_sem_trywait_delay(event, 0);
}

void uae_sem_destroy (uae_sem_t * event)
{
	if (*event) {
		SDL_DestroySemaphore((SDL_Semaphore *) *event);
		*event = NULL;
	}
}

uae_thread_id uae_thread_get_id(void)
{
	// FIXME: uae_thread_t and uae_thread_id_t should be different types!!!
	// This is not very nice - thread IDs as returned by  uae_thread_get_id is
	// not at all the same as the "thread ids" (pointers) used to manage
	// threads. It works as long as no code assume these types are the same.
	return (uae_thread_id) SDL_GetCurrentThreadID();
}

// An intermediate thread function is needed because the return type of
// uae_thread_function is different.

struct thparms
{
	void (*f)(void *);
	void *arg;
};

static int thread_init (void *f)
{
	struct thparms *thp = (struct thparms*)f;
	void (*fp)(void *) = thp->f;
	void *arg = thp->arg;

	xfree (f);

	fp(arg);
	return 0;
}

void uae_end_thread (uae_thread_id *tid)
{
#ifdef FSUAE
	if (tid) {
		// fs_thread_free(*thread);
		// SDL_WaitThread(*tid, NULL);
		SDL_DetachThread((SDL_Thread *) *tid);
		*tid = (uae_thread_id) NULL;
	}
#else
	if (tid) {
		CloseHandle (*tid);
		*tid = NULL;
	}
#endif
}

int uae_start_thread (const TCHAR *name, void (*f)(void *), void *arg, uae_thread_id *tid)
{
	int result = 1;
	struct thparms *thp;

	if (name != NULL) {
		write_log("uae_start_tread \"%s\" function %p arg %p\n", name, f, arg);
	} else {
		write_log("uae_start_tread (anonymous) function %p arg %p\n", f, arg);
	}

	thp = xmalloc (struct thparms, 1);
	thp->f = f;
	thp->arg = arg;
	SDL_Thread *thread = SDL_CreateThread(thread_init, name, thp);
	if (thread == NULL) {
		result = 0;
		write_log("ERROR creating thread\n");
	}

	if (tid) {
		*tid = (uae_thread_id) thread;
	} else if (thread) {
		SDL_DetachThread(thread);
		// FIXME: Check if this is a common occurence
		// write_log("WARNING: uae_start_thread with tid argument\n");
		// fs_thread_free(thread_id);
	}
	return result;
}

int uae_start_thread_fast (void (*f)(void *), void *arg, uae_thread_id *tid)
{
	int v = uae_start_thread (NULL, f, arg, tid);
	#warning Not adjusting thread priority at the moment
#if 0
	if (*tid) {
		if (!AVTask) {
			SetThreadPriority (*tid, THREAD_PRIORITY_HIGHEST);
		} else if (pAvSetMmThreadPriority) {
			pAvSetMmThreadPriority(AVTask, AVRT_PRIORITY_HIGH);
		}
	}
#endif
	return v;
}

void uae_set_thread_priority (uae_thread_id *tid, int pri)
{
	SDL_SetCurrentThreadPriority(SDL_THREAD_PRIORITY_HIGH);
#if 0
	if (!AVTask) {
		if (!SetThreadPriority (GetCurrentThread(), THREAD_PRIORITY_HIGHEST))
			SetThreadPriority (GetCurrentThread(), THREAD_PRIORITY_ABOVE_NORMAL);
	} else if (pAvSetMmThreadPriority) {
		pAvSetMmThreadPriority(AVTask, AVRT_PRIORITY_HIGH);
	}
#endif
}

void uae_wait_thread(uae_thread_id thread)
{
	SDL_WaitThread((SDL_Thread *) thread, NULL);
}

static SDL_ThreadID g_main_thread_id;
static SDL_ThreadID g_emulation_thread_id;

void uae_register_main_thread(void)
{
	g_main_thread_id = SDL_ThreadID();
}

bool uae_is_main_thread(void)
{
	return SDL_ThreadID() == g_main_thread_id;
}

bool is_mainthread(void)
{
	// Was a bit of misunderstanding here, is_mainthread should return true
	// if it is the *main emulation thread*. Fixed now. FIXME: Doublecheck
	// that "mainthread" in WinUAE is infact the emulation thread.
	return uae_is_emulation_thread();
}

void uae_register_emulation_thread(void)
{
	g_emulation_thread_id = SDL_GetCurrentThreadID();
}

bool uae_is_emulation_thread(void)
{
	return SDL_GetCurrentThreadID() == g_emulation_thread_id;
}
