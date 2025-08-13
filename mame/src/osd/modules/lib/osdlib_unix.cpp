// license:BSD-3-Clause
// copyright-holders:Olivier Galibert, R. Belmont
//============================================================
//
//  sdlos_*.c - OS specific low level code
//
//  SDLMAME by Olivier Galibert and R. Belmont
//
//============================================================

// MAME headers
#include "osdcore.h"
#include "osdlib.h"

#ifdef FSUAE
#include <SDL3/SDL.h>
#else
#include <SDL2/SDL.h>
#endif

#include <csignal>
#include <cstdio>
#include <cstdlib>
#include <iomanip>
#include <memory>

#ifdef FSUAE
#else
#include <dlfcn.h>
#include <sys/mman.h>
#endif
#include <sys/types.h>
#include <unistd.h>


//============================================================
//  osd_getenv
//============================================================

const char* osd_getenv(const char* name)
{
	return getenv(name);
}

//============================================================
//  osd_setenv
//============================================================

#ifdef FSUAE
#else
int osd_setenv(const char* name, const char* value, int overwrite)
{
	return setenv(name, value, overwrite);
}
#endif

//============================================================
//  osd_process_kill
//============================================================

#ifdef FSUAE
#else
void osd_process_kill()
{
	kill(getpid(), SIGKILL);
}
#endif

//============================================================
//  osd_break_into_debugger
//============================================================

void osd_break_into_debugger(const char* message)
{
#ifdef MAME_DEBUG
	printf("MAME exception: %s\n", message);
	printf("Attempting to fall into debugger\n");
	kill(getpid(), SIGTRAP);
#else
	printf("Ignoring MAME exception: %s\n", message);
#endif
}

#ifdef SDLMAME_ANDROID
std::string osd_get_clipboard_text()
{
	return std::string();
}
#else
//============================================================
//  osd_get_clipboard_text
//============================================================

std::string osd_get_clipboard_text()
{
	std::string result;

	if (SDL_HasClipboardText())
	{
		char* temp = SDL_GetClipboardText();
		result.assign(temp);
		SDL_free(temp);
	}
	return result;
}

#endif

//============================================================
//  osd_getpid
//============================================================

int osd_getpid()
{
	return getpid();
}


namespace osd {

	namespace {

		class dynamic_module_posix_impl : public dynamic_module
		{
		public:
			dynamic_module_posix_impl(std::vector<std::string>&& libraries) : m_libraries(std::move(libraries))
			{
			}

#ifdef FSUAE
#else
			virtual ~dynamic_module_posix_impl() override
			{
				if (m_module)
					dlclose(m_module);
			}
#endif

		protected:
#ifdef FSUAE
#else
			virtual generic_fptr_t get_symbol_address(char const* symbol) override
			{
				/*
				 * given a list of libraries, if a first symbol is successfully loaded from
				 * one of them, all additional symbols will be loaded from the same library
				 */
				if (m_module)
					return reinterpret_cast<generic_fptr_t>(dlsym(m_module, symbol));

				for (auto const& library : m_libraries)
				{
					void* const module = dlopen(library.c_str(), RTLD_LAZY);

					if (module != nullptr)
					{
						generic_fptr_t const function = reinterpret_cast<generic_fptr_t>(dlsym(module, symbol));

						if (function)
						{
							m_module = module;
							return function;
						}
						else
						{
							dlclose(module);
						}
					}
				}

				return nullptr;
			}
#endif

		private:
			std::vector<std::string> m_libraries;
			void* m_module = nullptr;
		};

	} // anonymous namespace


	bool invalidate_instruction_cache(void const* start, std::size_t size)
	{
#if !defined(SDLMAME_EMSCRIPTEN)
		char const* const begin(reinterpret_cast<char const*>(start));
		char const* const end(begin + size);
		__builtin___clear_cache(const_cast<char*>(begin), const_cast<char*>(end));
#endif
		return true;
	}

#ifdef FSUAE
#else

	void* virtual_memory_allocation::do_alloc(std::initializer_list<std::size_t> blocks, unsigned intent, std::size_t& size, std::size_t& page_size)
	{
		long const p(sysconf(_SC_PAGE_SIZE));
		if (0 >= p)
			return nullptr;
		std::size_t s(0);
		for (std::size_t b : blocks)
			s += (b + p - 1) / p;
		s *= p;
		if (!s)
			return nullptr;
#if defined __NetBSD__
		int req((NONE == intent) ? PROT_NONE : 0);
		if (intent & READ)
			req |= PROT_READ;
		if (intent & WRITE)
			req |= PROT_WRITE;
		if (intent & EXECUTE)
			req |= PROT_EXEC;
		int const prot(PROT_MPROTECT(req));
#else
		int const prot(PROT_NONE);
#endif
#if defined(SDLMAME_BSD) || defined(SDLMAME_MACOSX) || defined(SDLMAME_EMSCRIPTEN)
		int const fd(-1);
#else
		// TODO: portable applications are supposed to use -1 for anonymous mappings - detect whatever requires 0 specifically
		int const fd(0);
#endif
		void* const result(mmap(nullptr, s, prot, MAP_ANON | MAP_SHARED, fd, 0));
		if (result == (void*)-1)
			return nullptr;
		size = s;
		page_size = p;
		return result;
	}

	void virtual_memory_allocation::do_free(void* start, std::size_t size)
	{
		munmap(reinterpret_cast<char*>(start), size);
	}

	bool virtual_memory_allocation::do_set_access(void* start, std::size_t size, unsigned access)
	{
		int prot((NONE == access) ? PROT_NONE : 0);
		if (access & READ)
			prot |= PROT_READ;
		if (access & WRITE)
			prot |= PROT_WRITE;
		if (access & EXECUTE)
			prot |= PROT_EXEC;
		return mprotect(reinterpret_cast<char*>(start), size, prot) == 0;
	}

	dynamic_module::ptr dynamic_module::open(std::vector<std::string>&& names)
	{
		return std::make_unique<dynamic_module_posix_impl>(std::move(names));
	}
#endif

} // namespace osd
