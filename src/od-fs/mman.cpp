#include "sysconfig.h"
#include "sysdeps.h"
#include "uae/memory.h"
#include "uae/mman.h"
#include "options.h"
#include "autoconf.h"
#include "gfxboard.h"
#include "cpuboard.h"
#include "rommgr.h"
#include "newcpu.h"

#ifdef __x86_64__
static int os_64bit = 1;
#else
static int os_64bit = 0;
#endif

#ifndef _WIN32

#ifdef HAVE_SYS_PARAM_H
#include <sys/param.h>
#endif
#if defined(__APPLE__)
#include <sys/sysctl.h>
#endif
#include <sys/mman.h>

#define MEM_COMMIT       0x00001000
#define MEM_RESERVE      0x00002000
#define MEM_DECOMMIT         0x4000
#define MEM_RELEASE          0x8000
#define MEM_WRITE_WATCH  0x00200000
#define MEM_TOP_DOWN     0x00100000

#define PAGE_EXECUTE_READWRITE 0x40
#define PAGE_READONLY          0x02
#define PAGE_READWRITE         0x04

typedef void * LPVOID;
typedef size_t SIZE_T;

typedef struct {
	int dwPageSize;
} SYSTEM_INFO;

static void GetSystemInfo(SYSTEM_INFO *si)
{
	si->dwPageSize = sysconf(_SC_PAGESIZE);
}

#define USE_MMAP

#ifdef USE_MMAP
#ifdef MACOSX
#define MAP_ANONYMOUS MAP_ANON
#endif
#endif

static void *VirtualAlloc(void *lpAddress, size_t dwSize, int flAllocationType,
		int flProtect)
{
	write_log("- VirtualAlloc addr=%p size=%zu type=%d prot=%d\n",
			  lpAddress, dwSize, flAllocationType, flProtect);
	if (flAllocationType & MEM_RESERVE) {
		write_log("  MEM_RESERVE\n");
	}
	if (flAllocationType & MEM_COMMIT) {
		write_log("  MEM_COMMIT\n");
	}
	if (flAllocationType & PAGE_READWRITE) {
		write_log("  PAGE_READWRITE\n");
	}
	int prot = 0;
	if (flProtect == PAGE_READWRITE) {
		write_log("  PAGE_READWRITE\n");
		prot = PROT_READ | PROT_WRITE;
	} else if (flProtect == PAGE_READONLY) {
		write_log("  PAGE_READONLY\n");
		prot = PROT_READ;
	} else if (flProtect == PAGE_EXECUTE_READWRITE) {
		write_log("  PAGE_EXECUTE_READWRITE\n");
		prot = PROT_READ | PROT_WRITE | PROT_EXEC;
	} else {
		write_log("  WARNING: unknown protection\n");
	}

	void *memory = NULL;

	if (flAllocationType == MEM_COMMIT && lpAddress == NULL) {
		assert(false);
		memory = malloc(dwSize);
		if (memory == NULL) {
			write_log("memory allocated failed errno %d\n", errno);
		}
		return memory;
	}

	if (flAllocationType & MEM_RESERVE) {
#ifdef USE_MMAP
#ifdef CPU_x86_64
		memory = mmap(lpAddress, dwSize, 0, MAP_PRIVATE | MAP_ANONYMOUS | MAP_32BIT, -1, 0);
#else
		memory = mmap(lpAddress, dwSize, 0, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
#endif
		if (memory == (void *) -1) {
			write_log("mmap failed errno %d\n", errno);
			return NULL;
		}
#else
		memory = malloc(dwSize);
		if (memory == NULL) {
			write_log("memory allocated failed errno %d\n", errno);
			return NULL;
		}
#endif
	}
	else {
		memory = lpAddress;
	}

	if (flAllocationType & MEM_COMMIT) {
#ifdef USE_MMAP
		if (mprotect(memory, dwSize, prot) != 0) {
			write_log("mprotect failed errno %d\n", errno);
		}
#endif
	}

	return memory;
}

static int VirtualProtect(void *lpAddress, int dwSize, int flNewProtect,
						  unsigned int *lpflOldProtect)
{
	write_log("- VirtualProtect addr=%p size=%zu prot=%d\n",
			  lpAddress, dwSize, flNewProtect);
	int prot = 0;
	if (flNewProtect == PAGE_READWRITE) {
		write_log("  PAGE_READWRITE\n");
		prot = PROT_READ | PROT_WRITE;
	} else if (flNewProtect == PAGE_READONLY) {
		write_log("  PAGE_READONLY\n");
		prot = PROT_READ;
	} else {
		write_log("  -- unknown protection --\n");
	}
	if (mprotect(lpAddress, dwSize, prot) != 0) {
		write_log("mprotect failed errno %d\n", errno);
		return 0;
	}
	return 1;
}

static bool VirtualFree(void *lpAddress, size_t dwSize, int dwFreeType)
{
#ifdef MMAP
	int result = 0;
	if (dwFreeType == MEM_DECOMMIT) {
		if (mprotect(lpAddress, dwSize, 0) != 0) {
			write_log("mprotect failed errno %d\n", errno);
			return 0;
		}
	}
	else if (dwFreeType == MEM_RELEASE) {
		result = munmap(lpAddress, dwSize);
		if (result != 0) {
			printf("munmap failed\n");
		}
		return result == 0;
	}
	return 0;
#else
	return 1;
#endif
}

static int GetLastError()
{
	return errno;
}

static int my_getpagesize (void)
{
	return sysconf(_SC_PAGESIZE);
}

#define getpagesize my_getpagesize

#ifdef JIT

void cache_free (uae_u8 *cache)
{
	// FIXME: Must add (address, size) to a list in cache_alloc, so the memory
	// can be correctly released here...
	printf("TODO: free memory with munmap\n");
	//munmap(cache, size);
}

uae_u8 *cache_alloc (int size)
{
	printf("cache_alloc size = %d\n", size);
	size = size < getpagesize() ? getpagesize() : size;

#ifdef CPU_x86_64
	void *cache = mmap(0, size, PROT_READ | PROT_WRITE | PROT_EXEC,
		MAP_PRIVATE | MAP_ANON | MAP_32BIT, -1, 0);
#else
	void *cache = mmap(0, size, PROT_READ | PROT_WRITE | PROT_EXEC,
		MAP_PRIVATE | MAP_ANON, -1, 0);
#endif
	if (cache == MAP_FAILED) {
		write_log ("Cache_Alloc of %d failed. ERR=%d\n", size, errno);
	}
	return (uae_u8 *) cache;
}

#endif /* JIT */

#endif /* !WIN32 */

/* Prevent od-win32/win32.h from being included */
#define __WIN32_H__

#include "../od-win32/mman.cpp"
