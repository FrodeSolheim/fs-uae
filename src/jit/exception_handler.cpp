/*************************************************************************
* Handling mistaken direct memory access                                *
*************************************************************************/


#ifdef NATMEM_OFFSET
#ifdef _WIN32 // %%% BRIAN KING WAS HERE %%%
#include <winbase.h>
#else
#ifndef __USE_GNU
#define __USE_GNU
#endif
#include <sys/ucontext.h>
#endif
#include <signal.h>

#define SIG_READ 1
#define SIG_WRITE 2

static int in_handler=0;
static uae_u8 *veccode;

#ifdef _WIN32

#if defined(CPU_64_BIT)
#define ctxPC (pContext->Rip)
#else
#define ctxPC (pContext->Eip)
#endif

int EvalException (LPEXCEPTION_POINTERS blah, int n_except)
{
	PEXCEPTION_RECORD pExceptRecord = NULL;
	PCONTEXT          pContext = NULL;

	uae_u8* i = NULL;
	uae_u32 addr = 0;
	int r=-1;
	int size=4;
	int dir=-1;
	int len=0;

	if (n_except != STATUS_ACCESS_VIOLATION || !canbang || currprefs.cachesize == 0)
		return EXCEPTION_CONTINUE_SEARCH;

	pExceptRecord = blah->ExceptionRecord;
	pContext = blah->ContextRecord;

	if (pContext)
		i = (uae_u8 *)ctxPC;
	if (pExceptRecord)
		addr = (uae_u32)(pExceptRecord->ExceptionInformation[1]);
#ifdef JIT_DEBUG
	write_log (_T("JIT: fault address is 0x%x at 0x%x\n"),addr,i);
#endif
	if (!canbang || !currprefs.cachesize)
		return EXCEPTION_CONTINUE_SEARCH;

	if (in_handler)
		write_log (_T("JIT: Argh --- Am already in a handler. Shouldn't happen!\n"));

	if (canbang && i>=compiled_code && i<=current_compile_p) {
		if (*i==0x66) {
			i++;
			size=2;
			len++;
		}

		switch(i[0]) {
		case 0x8a:
			if ((i[1]&0xc0)==0x80) {
				r=(i[1]>>3)&7;
				dir=SIG_READ;
				size=1;
				len+=6;
				break;
			}
			break;
		case 0x88:
			if ((i[1]&0xc0)==0x80) {
				r=(i[1]>>3)&7;
				dir=SIG_WRITE;
				size=1;
				len+=6;
				break;
			}
			break;
		case 0x8b:
			switch(i[1]&0xc0) {
			case 0x80:
				r=(i[1]>>3)&7;
				dir=SIG_READ;
				len+=6;
				break;
			case 0x40:
				r=(i[1]>>3)&7;
				dir=SIG_READ;
				len+=3;
				break;
			case 0x00:
				r=(i[1]>>3)&7;
				dir=SIG_READ;
				len+=2;
				break;
			default:
				break;
			}
			break;
		case 0x89:
			switch(i[1]&0xc0) {
			case 0x80:
				r=(i[1]>>3)&7;
				dir=SIG_WRITE;
				len+=6;
				break;
			case 0x40:
				r=(i[1]>>3)&7;
				dir=SIG_WRITE;
				len+=3;
				break;
			case 0x00:
				r=(i[1]>>3)&7;
				dir=SIG_WRITE;
				len+=2;
				break;
			}
			break;
		}
	}

	if (r!=-1) {
		void* pr=NULL;
#ifdef JIT_DEBUG
		write_log (_T("JIT: register was %d, direction was %d, size was %d\n"),r,dir,size);
#endif

		switch(r) {
#if defined(CPU_64_BIT)
		case 0: pr=&(pContext->Rax); break;
		case 1: pr=&(pContext->Rcx); break;
		case 2: pr=&(pContext->Rdx); break;
		case 3: pr=&(pContext->Rbx); break;
		case 4: pr=(size>1)?NULL:(((uae_u8*)&(pContext->Rax))+1); break;
		case 5: pr=(size>1)?
			(void*)(&(pContext->Rbp)):
			(void*)(((uae_u8*)&(pContext->Rcx))+1); break;
		case 6: pr=(size>1)?
			(void*)(&(pContext->Rsi)):
			(void*)(((uae_u8*)&(pContext->Rdx))+1); break;
		case 7: pr=(size>1)?
			(void*)(&(pContext->Rdi)):
			(void*)(((uae_u8*)&(pContext->Rbx))+1); break;
#else
		case 0: pr=&(pContext->Eax); break;
		case 1: pr=&(pContext->Ecx); break;
		case 2: pr=&(pContext->Edx); break;
		case 3: pr=&(pContext->Ebx); break;
		case 4: pr=(size>1)?NULL:(((uae_u8*)&(pContext->Eax))+1); break;
		case 5: pr=(size>1)?
			(void*)(&(pContext->Ebp)):
			(void*)(((uae_u8*)&(pContext->Ecx))+1); break;
		case 6: pr=(size>1)?
			(void*)(&(pContext->Esi)):
			(void*)(((uae_u8*)&(pContext->Edx))+1); break;
		case 7: pr=(size>1)?
			(void*)(&(pContext->Edi)):
			(void*)(((uae_u8*)&(pContext->Ebx))+1); break;
#endif
		default: abort();
		}
		if (pr) {
			blockinfo* bi;

			if (currprefs.comp_oldsegv) {
				addr-=(uae_u32)NATMEM_OFFSET;

#ifdef JIT_DEBUG
				if ((addr>=0x10000000 && addr<0x40000000) ||
					(addr>=0x50000000)) {
						write_log (_T("JIT: Suspicious address 0x%x in SEGV handler.\n"),addr);
				}
#endif
				if (dir==SIG_READ) {
					switch (size) {
					case 1: *((uae_u8*)pr)=get_byte (addr); break;
					case 2: *((uae_u16*)pr)=swap16(get_word (addr)); break;
					case 4: *((uae_u32*)pr)=swap32(get_long (addr)); break;
					default: abort();
					}
				}
				else { /* write */
					switch (size) {
					case 1: put_byte (addr,*((uae_u8*)pr)); break;
					case 2: put_word (addr,swap16(*((uae_u16*)pr))); break;
					case 4: put_long (addr,swap32(*((uae_u32*)pr))); break;
					default: abort();
					}
				}
#ifdef JIT_DEBUG
				write_log (_T("JIT: Handled one access!\n"));
#endif
				fflush(stdout);
				segvcount++;
				ctxPC+=len;
			}
			else {
				void* tmp=target;
				int i;
				uae_u8 vecbuf[5];

				addr-=(uae_u32)NATMEM_OFFSET;

#ifdef JIT_DEBUG
				if ((addr>=0x10000000 && addr<0x40000000) ||
					(addr>=0x50000000)) {
						write_log (_T("JIT: Suspicious address 0x%x in SEGV handler.\n"),addr);
				}
#endif

				target=(uae_u8*)ctxPC;
				for (i=0;i<5;i++)
					vecbuf[i]=target[i];
				emit_byte(0xe9);
				emit_long((uae_u32)veccode-(uae_u32)target-4);
#ifdef JIT_DEBUG

				write_log (_T("JIT: Create jump to %p\n"),veccode);
				write_log (_T("JIT: Handled one access!\n"));
#endif
				segvcount++;

				target=veccode;

				if (dir==SIG_READ) {
					switch(size) {
					case 1: raw_mov_b_ri(r,get_byte (addr)); break;
					case 2: raw_mov_w_ri(r,swap16(get_word (addr))); break;
					case 4: raw_mov_l_ri(r,swap32(get_long (addr))); break;
					default: abort();
					}
				}
				else { /* write */
					switch(size) {
					case 1: put_byte (addr,*((uae_u8*)pr)); break;
					case 2: put_word (addr,swap16(*((uae_u16*)pr))); break;
					case 4: put_long (addr,swap32(*((uae_u32*)pr))); break;
					default: abort();
					}
				}
				for (i=0;i<5;i++)
					raw_mov_b_mi(ctxPC+i,vecbuf[i]);
				raw_mov_l_mi((uae_u32)&in_handler,0);
				emit_byte(0xe9);
				emit_long(ctxPC+len-(uae_u32)target-4);
				in_handler=1;
				target=(uae_u8*)tmp;
			}
			bi=active;
			while (bi) {
				if (bi->handler &&
						(uae_u8*)bi->direct_handler<=i &&
						(uae_u8*)bi->nexthandler>i) {
#ifdef JIT_DEBUG
					write_log (_T("JIT: deleted trigger (%p<%p<%p) %p\n"),
							bi->handler,
							i,
							bi->nexthandler,
							bi->pc_p);
#endif
					invalidate_block(bi);
					raise_in_cl_list(bi);
					set_special(0);
					return EXCEPTION_CONTINUE_EXECUTION;
				}
				bi=bi->next;
			}
			/* Not found in the active list. Might be a rom routine that
			is in the dormant list */
			bi=dormant;
			while (bi) {
				if (bi->handler &&
						(uae_u8*)bi->direct_handler<=i &&
						(uae_u8*)bi->nexthandler>i) {
#ifdef JIT_DEBUG
					write_log (_T("JIT: deleted trigger (%p<%p<%p) %p\n"),
							bi->handler,
							i,
							bi->nexthandler,
							bi->pc_p);
#endif
					invalidate_block(bi);
					raise_in_cl_list(bi);
					set_special(0);
					return EXCEPTION_CONTINUE_EXECUTION;
				}
				bi=bi->next;
			}
#ifdef JIT_DEBUG
			write_log (_T("JIT: Huh? Could not find trigger!\n"));
#endif
			return EXCEPTION_CONTINUE_EXECUTION;
		}
	}
	write_log (_T("JIT: Can't handle access %08X!\n"), i);
#if 0
	if (i)
	{
		int j;

		for (j=0;j<10;j++) {
			write_log (_T("JIT: instruction byte %2d is 0x%02x\n"),j,i[j]);
		}
	}
	write_log (_T("Please send the above info (starting at \"fault address\") to\n"));
	write_log (_T("bmeyer@csse.monash.edu.au\n"));
	write_log (_T("This shouldn't happen ;-)\n"));
#endif
	return EXCEPTION_CONTINUE_SEARCH;
}
#else

#if defined(__APPLE__) && __DARWIN_UNIX03
#define CONTEXT_MEMBER(x) __##x
#elif defined(__FreeBSD__)
#define CONTEXT_MEMBER(x) sc_##x
#else
#define CONTEXT_MEMBER(x) x
#endif

#ifdef __APPLE__
static void vec(int x, siginfo_t *info, ucontext_t *uap)
{
	_STRUCT_X86_THREAD_STATE32 sc = uap->uc_mcontext->CONTEXT_MEMBER(ss);
	uae_u32 addr = 0;
#else
static void vec(int x, struct sigcontext sc)
{
#endif
    uae_u8* i=(uae_u8*)sc.CONTEXT_MEMBER(eip);
#ifdef __APPLE__
	if (i >= compiled_code) {
			unsigned int j;
			write_log ("JIT_APPLE: can't handle access!\n");
			for (j = 0 ; j < 10; j++)
				write_log ("JIT: instruction byte %2d is %02x\n", i, j[i]);
	} else {
		write_log ("Caught illegal access to (unknown) at eip=%08x\n", i);
	}
   exit (EXIT_FAILURE);
#elif defined(__FreeBSD__)
	//uae_u32 addr=sc.cr2;
	uae_u32 addr=sc.CONTEXT_MEMBER(spare2); // UJ: @@@@: ????:
#else
	uae_u32 addr=sc.cr2;
#endif
	int r=-1;
	int size=4;
	int dir=-1;
	int len=0;
	int j;

	write_log (_T("JIT: fault address is %08x at %08x\n"),addr,i);
	if (!canbang)
		write_log (_T("JIT: Not happy! Canbang is 0 in SIGSEGV handler!\n"));
	if (in_handler)
		write_log (_T("JIT: Argh --- Am already in a handler. Shouldn't happen!\n"));

    /*
     * Decode access opcode
     */
    if (canbang && i>=compiled_code && i<=current_compile_p) {
        if (*i==0x66) {
            i++;
            size=2;
            len++;
        }

		switch(i[0]) {
		case 0x8a:
			if ((i[1]&0xc0)==0x80) {
				r=(i[1]>>3)&7;
				dir=SIG_READ;
				size=1;
				len+=6;
				break;
			}
			break;
		case 0x88:
			if ((i[1]&0xc0)==0x80) {
				r=(i[1]>>3)&7;
				dir=SIG_WRITE;
				size=1;
				len+=6;
				break;
			}
			break;

		case 0x8b:
			switch(i[1]&0xc0) {
			case 0x80:
				r=(i[1]>>3)&7;
				dir=SIG_READ;
				len+=6;
				break;
			case 0x40:
				r=(i[1]>>3)&7;
				dir=SIG_READ;
				len+=3;
				break;
			case 0x00:
				r=(i[1]>>3)&7;
				dir=SIG_READ;
				len+=2;
				break;
			default:
				break;
			}
			break;

		case 0x89:
			switch(i[1]&0xc0) {
			case 0x80:
				r=(i[1]>>3)&7;
				dir=SIG_WRITE;
				len+=6;
				break;
			case 0x40:
				r=(i[1]>>3)&7;
				dir=SIG_WRITE;
				len+=3;
				break;
			case 0x00:
				r=(i[1]>>3)&7;
				dir=SIG_WRITE;
				len+=2;
				break;
			}
			break;
		}
	}

	if (r!=-1) {
		void* pr=NULL;
		write_log (_T("JIT: register was %d, direction was %d, size was %d\n"),r,dir,size);

		switch(r) {
		case 0: pr = &(sc.CONTEXT_MEMBER(eax)); break;
		case 1: pr = &(sc.CONTEXT_MEMBER(ecx)); break;
		case 2: pr = &(sc.CONTEXT_MEMBER(edx)); break;
		case 3: pr = &(sc.CONTEXT_MEMBER(ebx)); break;
		case 4: pr = (size>1) ? NULL:(((uae_u8*)&(sc.CONTEXT_MEMBER(eax)))+1); break;
		case 5: pr=(size>1)?
			(void*)(&(sc.CONTEXT_MEMBER(ebp))):
			(void*)(((uae_u8*)&(sc.CONTEXT_MEMBER(ecx)))+1); break;
		case 6: pr=(size>1)?
			(void*)(&(sc.CONTEXT_MEMBER(esi))):
			(void*)(((uae_u8*)&(sc.CONTEXT_MEMBER(edx)))+1); break;
		case 7: pr=(size>1)?
			(void*)(&(sc.CONTEXT_MEMBER(edi))):
			(void*)(((uae_u8*)&(sc.CONTEXT_MEMBER(ebx)))+1); break;
		default: abort();
		}
		if (pr) {
			blockinfo* bi;

			if (currprefs.comp_oldsegv) {
#ifdef FSUAE
// FIXME: check why (uae_u8 was here originally...?)
				addr -= (uae_u32)NATMEM_OFFSET;
#else
				addr -= (uae_u8)NATMEM_OFFSET;
#endif

				if ((addr>=0x10000000 && addr<0x40000000) ||
					(addr>=0x50000000)) {
						write_log (_T("JIT: Suspicious address in %x SEGV handler.\n"),addr);
				}
				if (dir==SIG_READ) {
					switch(size) {
					case 1: *((uae_u8*)pr)=get_byte (addr); break;
					case 2: *((uae_u16*)pr)=get_word (addr); break;
					case 4: *((uae_u32*)pr)=get_long (addr); break;
					default: abort();
					}
				}
				else { /* write */
					switch(size) {
					case 1: put_byte (addr,*((uae_u8*)pr)); break;
					case 2: put_word (addr,*((uae_u16*)pr)); break;
					case 4: put_long (addr,*((uae_u32*)pr)); break;
					default: abort();
					}
				}
				write_log ("JIT: Handled one access!\n");
				fflush(stdout);
				segvcount++;
				sc.CONTEXT_MEMBER(eip) += len;
			}
			else {
				uint8_t* tmp=target;
				int i;
				uae_u8 vecbuf[5];

#ifdef FSUAE
// FIXME: check why (uae_u8 was here originally...?)
				addr -= (uae_u32)NATMEM_OFFSET;
#else
				addr-=(uae_u8)NATMEM_OFFSET;
#endif

				if ((addr>=0x10000000 && addr<0x40000000) ||
					(addr>=0x50000000)) {
						write_log (_T("JIT: Suspicious address 0x%x in SEGV handler.\n"),addr);
				}

				target = (uae_u8*)sc.CONTEXT_MEMBER(eip);
				for (i=0;i<5;i++)
					vecbuf[i]=target[i];
				emit_byte(0xe9);
				emit_long((uae_u32)veccode-(uae_u32)target-4);
				write_log (_T("JIT: Create jump to %p\n"),veccode);

				write_log (_T("JIT: Handled one access!\n"));
				segvcount++;

				target=veccode;

				if (dir==SIG_READ) {
					switch(size) {
					case 1: raw_mov_b_ri(r,get_byte (addr)); break;
					case 2: raw_mov_w_ri(r,get_word (addr)); break;
					case 4: raw_mov_l_ri(r,get_long (addr)); break;
					default: abort();
					}
				}
				else { /* write */
					switch(size) {
					case 1: put_byte (addr,*((uae_u8*)pr)); break;
					case 2: put_word (addr,*((uae_u16*)pr)); break;
					case 4: put_long (addr,*((uae_u32*)pr)); break;
					default: abort();
					}
				}
				for (i=0;i<5;i++)
					raw_mov_b_mi(sc.CONTEXT_MEMBER(eip)+i,vecbuf[i]);
				raw_mov_l_mi((uae_u32)&in_handler,0);
				emit_byte(0xe9);
				emit_long(sc.CONTEXT_MEMBER(eip)+len-(uae_u32)target-4);
				in_handler=1;
				target=tmp;
			}
			bi=active;
			while (bi) {
				if (bi->handler &&
					(uae_u8*)bi->direct_handler<=i &&
					(uae_u8*)bi->nexthandler>i) {
						write_log (_T("JIT: deleted trigger (%p<%p<%p) %p\n"),
							bi->handler,
							i,
							bi->nexthandler,
							bi->pc_p);
						invalidate_block(bi);
						raise_in_cl_list(bi);
						set_special(0);
						return;
				}
				bi=bi->next;
			}
			/* Not found in the active list. Might be a rom routine that
			is in the dormant list */
			bi=dormant;
			while (bi) {
				if (bi->handler &&
					(uae_u8*)bi->direct_handler<=i &&
					(uae_u8*)bi->nexthandler>i) {
						write_log (_T("JIT: deleted trigger (%p<%p<%p) %p\n"),
							bi->handler,
							i,
							bi->nexthandler,
							bi->pc_p);
						invalidate_block(bi);
						raise_in_cl_list(bi);
						set_special(0);
						return;
				}
				bi=bi->next;
			}
			write_log (_T("JIT: Huh? Could not find trigger!\n"));
			return;
		}
	}
	write_log (_T("JIT: Can't handle access!\n"));
	for (j=0;j<10;j++) {
		write_log (_T("JIT: instruction byte %2d is %02x\n"),j,i[j]);
	}
#if 0
	write_log (_T("Please send the above info (starting at \"fault address\") to\n")
		"bmeyer@csse.monash.edu.au\n"
		"This shouldn't happen ;-)\n");
	fflush(stdout);
#endif
	signal(SIGSEGV,SIG_DFL);  /* returning here will cause a "real" SEGV */
}
#endif
#endif
