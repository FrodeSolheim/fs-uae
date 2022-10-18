#include "sysconfig.h"
#ifdef _WIN32
#include <winsock2.h>
#include <windows.h>
#include <ws2tcpip.h>
#else
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#endif

#include "sysdeps.h"

#include <thread>
#include <vector>
#include <array>

#include "options.h"
#include "memory.h"
#include "newcpu.h"
#include "debug.h"
#include "inputdevice.h"
#include "uae.h"
#include "debugmem.h"
#include "custom.h"
#include "xwin.h" // xcolnr
#include "drawing.h" // color_entry
#include "savestate.h"
#include "fsemu-log.h"
#include "fsemu-action.h"
#include "fsemu-video.h"
#include "fs/conf.h"

#define OPTION_REMOTE_DEBUGGER_START_TIMER "remote_debugger"
#define OPTION_REMOTE_DEBUGGER_TRIGGER "remote_debugger_trigger"
#define OPTION_REMOTE_DEBUGGER_PORT "remote_debugger_port"
#define DEFAULT_PORT 2345

#ifdef _WIN32
	#define sock_err WSAGetLastError()
#else
	#define sock_err errno
	#define closesocket ::close
#endif

#ifndef _countof
#define _countof(array) (sizeof(array) / sizeof(array[0]))
#endif

#ifndef max
#define max(a,b) (((a)>(b))?(a):(b))
#endif
#ifndef min
#define min(a,b) (((a)<(b))?(a):(b))
#endif

#ifndef SOCKET_ERROR
#define SOCKET_ERROR -1
#endif
#ifndef SOCKADDR_INET
#define SOCKADDR_INET -1
#endif

#define barto_log(format, ...) fsemu_log_info(format, ##__VA_ARGS__)

// from newcpu.cpp
/*static*/ extern int baseclock;

// from custom.cpp
/*static*/ extern struct color_entry current_colors;

// from debug.cpp
extern uae_u8 *get_real_address_debug(uaecptr addr);
extern void initialize_memwatch(int mode);
extern void memwatch_setup();
/*static*/ extern int trace_mode;
/*static*/ extern uae_u32 trace_param[2];
/*static*/ extern uaecptr processptr;
/*static*/ extern uae_char *processname;
/*static*/ extern int memwatch_triggered;
/*static*/ extern struct memwatch_node mwhit;
extern int debug_illegal;
extern uae_u64 debug_illegal_mask;

// from fsvideo.cpp
extern fsemu_video_frame_t *uae_fsvideo_getframe();

#include "barto_gdbserver.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

// VS2022: Test or Release/FullRelease config 
// -s input.config=1 -s input.1.keyboard.0.button.41.GRAVE=SPC_SINGLESTEP.0    -s use_gui=no -s quickstart=a500,1 -s debugging_features=gdbserver -s filesystem=rw,dh0:c:\Users\Chuck\Documents\Visual_Studio_Code\amiga-debug\bin\dh0
// c:\Users\Chuck\Documents\Visual_Studio_Code\amiga-debug\bin\opt\bin> m68k-amiga-elf-gdb.exe -ex "set debug remote 1" -ex "target remote :2345" -ex "monitor profile xxx" ..\..\..\template\a.mingw.elf

namespace barto_gdbserver {
	bool is_connected();
	bool data_available();
	void disconnect();

	// Options:
	bool enabled = false;
	int port = DEFAULT_PORT;
	int time_out;
	char *debugging_trigger;

	static bool in_handle_packet = false;
	struct tracker {
		tracker() { backup = in_handle_packet; in_handle_packet = true; }
		~tracker() { in_handle_packet = backup; }
	private: 
		bool backup;
	};

	static std::string string_replace_all(const std::string& str, const std::string& search, const std::string& replace) {
		std::string copy(str);
		size_t start = 0;
		for(;;) {
			auto p = copy.find(search, start);
			if(p == std::string::npos)
				break;

			copy.replace(p, search.size(), replace);
			start = p + replace.size();
		}
		return copy;
	}

	/*
	static std::string string_to_utf8(LPCWSTR string) {
		int len = WideCharToMultiByte(CP_UTF8, 0, string, -1, nullptr, 0, nullptr, nullptr);
		std::unique_ptr<char[]> buffer(new char[len]);
		WideCharToMultiByte(CP_UTF8, 0, string, -1, buffer.get(), len, nullptr, nullptr);
		wcstombs(buffer.get(), string, len);
		return std::string(buffer.get());
	}
	*/

	static constexpr char hex[]{ "0123456789abcdef" };
	static std::string hex8(uint8_t v) {
		std::string ret;
		ret += hex[v >> 4];
		ret += hex[v & 0xf];
		return ret;
	}
	static std::string hex32(uint32_t v) {
		std::string ret;
		for(int i = 28; i >= 0; i -= 4)
			ret += hex[(v >> i) & 0xf];
		return ret;
	}

	static std::string from_hex(const std::string& s) {
		std::string ret;
		for(size_t i = 0, len = s.length() & ~1; i < len; i += 2) {
			uint8_t v{};
			if(s[i] >= '0' && s[i] <= '9')
				v |= (s[i] - '0') << 4;
			else if(s[i] >= 'a' && s[i] <= 'f')
				v |= (s[i] - 'a' + 10) << 4;
			else if(s[i] >= 'A' && s[i] <= 'F')
				v |= (s[i] - 'A' + 10) << 4;
			if(s[i + 1] >= '0' && s[i + 1] <= '9')
				v |= (s[i + 1] - '0');
			else if(s[i + 1] >= 'a' && s[i + 1] <= 'f')
				v |= (s[i + 1] - 'a' + 10);
			else if(s[i + 1] >= 'A' && s[i + 1] <= 'F')
				v |= (s[i + 1] - 'A' + 10);
			ret += (char)v;
		}
		return ret;
	}

	static std::string to_hex(const std::string& s) {
		std::string ret;
		for(size_t i = 0, len = s.length(); i < len; i++) {
			uint8_t v = s[i];
			ret += hex[v >> 4];
			ret += hex[v & 0xf];
		}
		return ret;
	}

/*	#pragma comment(lib, "Bcrypt.lib")
	#ifndef NT_SUCCESS
		#define NT_SUCCESS(Status) ((NTSTATUS)(Status) >= 0)
	#endif
	std::array<uint8_t, 32> sha256(const void* addr, size_t size) {
		std::array<uint8_t, 32> hash{};

		BCRYPT_ALG_HANDLE AlgHandle = nullptr;
		BCRYPT_HASH_HANDLE HashHandle = nullptr;
		if(NT_SUCCESS(BCryptOpenAlgorithmProvider(&AlgHandle, BCRYPT_SHA256_ALGORITHM, nullptr, BCRYPT_HASH_REUSABLE_FLAG))) {
			DWORD HashLength = 0;
			DWORD ResultLength = 0;
			if(NT_SUCCESS(BCryptGetProperty(AlgHandle, BCRYPT_HASH_LENGTH, (PBYTE)&HashLength, sizeof(HashLength), &ResultLength, 0)) && HashLength == hash.size()) {
				if(NT_SUCCESS(BCryptCreateHash(AlgHandle, &HashHandle, nullptr, 0, nullptr, 0, 0))) {
					(void)BCryptHashData(HashHandle, kickmem_bank.baseaddr, kickmem_bank.allocated_size, 0);
					(void)BCryptFinishHash(HashHandle, hash.data(), (ULONG)hash.size(), 0);
					BCryptDestroyHash(HashHandle);
				}
			}
			BCryptCloseAlgorithmProvider(AlgHandle, 0);
		}

		return hash;
	}

	std::array<uint8_t, 16> sha1(const void* addr, size_t size) {
		std::array<uint8_t, 16> hash{};

		BCRYPT_ALG_HANDLE AlgHandle = nullptr;
		BCRYPT_HASH_HANDLE HashHandle = nullptr;
		if(NT_SUCCESS(BCryptOpenAlgorithmProvider(&AlgHandle, BCRYPT_SHA1_ALGORITHM, nullptr, BCRYPT_HASH_REUSABLE_FLAG))) {
			DWORD HashLength = 0;
			DWORD ResultLength = 0;
			if(NT_SUCCESS(BCryptGetProperty(AlgHandle, BCRYPT_HASH_LENGTH, (PBYTE)&HashLength, sizeof(HashLength), &ResultLength, 0)) && HashLength == hash.size()) {
				if(NT_SUCCESS(BCryptCreateHash(AlgHandle, &HashHandle, nullptr, 0, nullptr, 0, 0))) {
					(void)BCryptHashData(HashHandle, kickmem_bank.baseaddr, kickmem_bank.allocated_size, 0);
					(void)BCryptFinishHash(HashHandle, hash.data(), (ULONG)hash.size(), 0);
					BCryptDestroyHash(HashHandle);
				}
			}
			BCryptCloseAlgorithmProvider(AlgHandle, 0);
		}

		return hash;
	}
*/

	std::thread connect_thread;
	SOCKET gdbsocket{ INVALID_SOCKET };
	SOCKET gdbconn{ INVALID_SOCKET };
	char socketaddr[sizeof SOCKADDR_INET];
	bool useAck{ true };
	uint32_t baseText{};
	uint32_t sizeText{};
	uint32_t systemStackLower{}, systemStackUpper{};
	uint32_t stackLower{}, stackUpper{};
	std::vector<uint32_t> sections; // base for every section
	std::string profile_outname;
	int profile_num_frames{};
	int profile_frame_count{};
	std::unique_ptr<cpu_profiler_unwind[]> profile_unwind{};

	enum class state {
		inited,
		connected,
		debugging,
		profile,
		profiling,
	};

	state debugger_state{ state::inited };

	bool is_connected() {
		if(gdbsocket == INVALID_SOCKET)
			return false;
		if(gdbconn == INVALID_SOCKET) {
			struct timeval tv;
			fd_set fd;
			tv.tv_sec = 0;
			tv.tv_usec = 0;
			FD_ZERO(&fd);
			FD_SET(gdbsocket, &fd);
			if(select(gdbsocket + 1, &fd, NULL, NULL, &tv)) {
				unsigned int hostSize = sizeof(struct sockaddr_in);
				gdbconn = accept(gdbsocket, (struct sockaddr*)socketaddr, (socklen_t*)&hostSize);
				if(gdbconn != INVALID_SOCKET)
					barto_log("connection accepted\n");
			}
		}
		return gdbconn != INVALID_SOCKET;
	}

	bool data_available() {
		if(is_connected()) {
			struct timeval tv;
			fd_set fd;
			tv.tv_sec = 0;
			tv.tv_usec = 0;
			FD_ZERO(&fd);
			FD_SET(gdbconn, &fd);
			int err = select(gdbconn + 1, &fd, nullptr, nullptr, &tv);
			if(err == SOCKET_ERROR) {
				disconnect();
				return false;
			}
			if(err > 0)
				return true;
		}
		return false;
	}

	bool listen() {
		barto_log("GDBSERVER: listen()\n");

		assert(debugger_state == state::inited);

		#ifdef _WIN32
		WSADATA wsaData = { 0 };
		if(WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
			barto_log(_T("GDBSERVER: can't open winsock, error %d\n"), sock_err);
			return false;
		}
		#endif

		int err;
		const int one = 1;
		const struct linger linger_1s = { 1, 1 };
		constexpr auto name = _T("127.0.0.1");

		sockaddr_in serv_addr;
		serv_addr.sin_family=AF_INET;
		serv_addr.sin_addr.s_addr = inet_addr(name);
		serv_addr.sin_port = htons(port);

		gdbsocket = socket(AF_INET, SOCK_STREAM, 0);
		if(gdbsocket == INVALID_SOCKET) {
			barto_log(_T("GDBSERVER: socket() failed\n"), sock_err);
			return false;
		}
		if(::bind(gdbsocket, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
			barto_log(_T("GDBSERVER: bind() failed\n"), sock_err);
			return false;
		}
		if(::listen(gdbsocket, 1) < 0) {
			barto_log(_T("GDBSERVER: listen() failed\n"), sock_err);
			return false;
		}
		if(setsockopt(gdbsocket, SOL_SOCKET, SO_LINGER, (char*)&linger_1s, sizeof linger_1s) < 0) {
			barto_log(_T("GDBSERVER: setsockopt(SO_LINGER) failed\n"), sock_err);
			return false;
		}
		if(setsockopt(gdbsocket, SOL_SOCKET, SO_REUSEADDR, (char*)&one, sizeof one) < 0) {
			barto_log(_T("GDBSERVER: setsockopt(SO_REUSEADDR) failed\n"), sock_err);
			return false;
		}

		barto_log("GDBSERVER: listen() succeeded on %s:%d\n", name, port);
		return true;
	}

	bool init() {
		// Read options:
		if(fs_config_exists(OPTION_REMOTE_DEBUGGER_START_TIMER)) {
			enabled = true;
			time_out = fs_config_get_int(OPTION_REMOTE_DEBUGGER_START_TIMER);
		}
		if(fs_config_exists(OPTION_REMOTE_DEBUGGER_TRIGGER)) {
			debugging_trigger = fs_config_get_string(OPTION_REMOTE_DEBUGGER_TRIGGER);
		}
		if(fs_config_exists(OPTION_REMOTE_DEBUGGER_PORT)) {
			port = fs_config_get_int(OPTION_REMOTE_DEBUGGER_PORT);
		}

		if(enabled) {
			barto_log(_T("GDBSERVER: enabled (start_timer: %d trigger: %s port: %d)\n"), time_out, debugging_trigger, port);
			close();

			fsemu_action_process_command_in_main(FSEMU_ACTION_WARP, 1);

			// disable console
			static TCHAR empty[2] = { 0 };
			setconsolemode(empty, 1);

			activate_debugger();
			initialize_memwatch(0);

			if(debugging_trigger) {
				// from debug.cpp@process_breakpoint()
				processptr = 0;
				xfree(processname);
				processname = nullptr;
				processname = debugging_trigger;
				trace_mode = TRACE_CHECKONLY;
			} else {
				// savestate debugging
				baseText = 0;
				sizeText = 0x7fff'ffff;
			}

			// call as early as possible to avoid delays with GDB having to retry to connect...
			listen();
		}

		return true;
	}

	void close() {
		barto_log(_T("GDBSERVER: close()\n"));
		if(gdbconn != INVALID_SOCKET)
			closesocket(gdbconn);
		gdbconn = INVALID_SOCKET;
		if(gdbsocket != INVALID_SOCKET)
			closesocket(gdbsocket);
		gdbsocket = INVALID_SOCKET;
		#ifdef _WIN32
		WSACleanup();
		#endif
	}

	void disconnect() {
		if(gdbconn == INVALID_SOCKET)
			return;
		closesocket(gdbconn);
		gdbconn = INVALID_SOCKET;
		barto_log(_T("GDBSERVER: disconnect\n"));
	}

	// from binutils-gdb/gdb/m68k-tdep.c
/*	static const char* m68k_register_names[] = {
		"d0", "d1", "d2", "d3", "d4", "d5", "d6", "d7",
		"a0", "a1", "a2", "a3", "a4", "a5", "a6", "sp", //BARTO
		"sr", "pc", //BARTO
		"fp0", "fp1", "fp2", "fp3", "fp4", "fp5", "fp6", "fp7",
		"fpcontrol", "fpstatus", "fpiaddr"
	}*/
	enum regnames {
		D0, D1, D2, D3, D4, D5, D6, D7,
		A0, A1, A2, A3, A4, A5, A6, A7,
		SR, PC
	};

	static std::string get_register(int reg) {
		uint32_t regvalue{};
		// need to byteswap because GDB expects 68k big-endian
		switch(reg) {
		case SR: 
			regvalue = regs.sr; 
			break;
		case PC: 
			regvalue = M68K_GETPC; 
			break;
		case D0: case D1: case D2: case D3: case D4: case D5: case D6: case D7:
			regvalue = m68k_dreg(regs, reg - D0);
			break;
		case A0: case A1: case A2: case A3: case A4: case A5: case A6: case A7:
			regvalue = m68k_areg(regs, reg - A0);
			break;
		default:
			return "xxxxxxxx";
		}
		return hex32(regvalue);
	}

	static std::string get_registers() {
		barto_log("PC=%x\n", M68K_GETPC);
		std::string ret;
		for(int reg = 0; reg < 18; reg++)
			ret += get_register(reg);
		return ret;
	}

	void print_breakpoints() {
		barto_log("GDBSERVER: Breakpoints:\n");
		for(auto& bpn : bpnodes) {
			if(bpn.enabled) {
				barto_log("GDBSERVER: - %d, 0x%x, 0x%x\n", bpn.type, bpn.value1, bpn.value2);
			}
		}
	}

	void print_watchpoints() {
		barto_log("GDBSERVER: Watchpoints:\n");
		for(auto& mwn : mwnodes) {
			if(mwn.size) {
				barto_log("GDBSERVER: - 0x%x, 0x%x\n", mwn.addr, mwn.size);
			}
		}
	}

	void send_ack(const std::string& ack) {
		if(useAck && !ack.empty()) {
			barto_log("GDBSERVER: <- %s\n", ack.c_str());
			int result = send(gdbconn, ack.data(), (int)ack.length(), 0);
			if(result == SOCKET_ERROR)
				barto_log(_T("GDBSERVER: error sending ack: %d\n"), sock_err);
		}
	}

	void send_response(std::string response) {
		tracker _;
		if(!response.empty()) {
			barto_log("GDBSERVER: <- %s\n", response.substr(1).c_str());
			uint8_t cksum{};
			for(size_t i = 1; i < response.length(); i++)
				cksum += response[i];
			response += '#';
			response += hex[cksum >> 4];
			response += hex[cksum & 0xf];
			int result = send(gdbconn, response.data(), (int)response.length(), 0);
			if(result == SOCKET_ERROR)
				barto_log(_T("GDBSERVER: error sending data: %d\n"), sock_err);
		}
	}

	void handle_packet() {
		tracker _;
		if(data_available()) {
			char buf[512];
			auto result = recv(gdbconn, buf, sizeof(buf) - 1, 0);
			if(result > 0) {
				buf[result] = '\0';
				barto_log("GDBSERVER: received %d bytes: >>%s<<\n", result, buf);
				std::string request{ buf }, ack{}, response;
				while(!request.empty() && (request[0] == '+' || request[0] == '-')) {
					if(request[0] == '+') {
						request = request.substr(1);
					} else if(request[0] == '-') {
						barto_log("GDBSERVER: client non-ack'd our last packet\n");
						request = request.substr(1);
					}
				}
				if(!request.empty() && request[0] == 0x03) {
					// Ctrl+C
					ack = "+";
					response = "$";
					response += "S05"; // SIGTRAP
					debugger_state = state::debugging;
					activate_debugger();
				} else if(!request.empty() && request[0] == '$') {
					ack = "-";
					auto end = request.find('#');
					if(end != std::string::npos) {
						uint8_t cksum{};
						for(size_t i = 1; i < end; i++)
							cksum += request[i];
						if(request.length() >= end + 2) {
							if(tolower(request[end + 1]) == hex[cksum >> 4] && tolower(request[end + 2]) == hex[cksum & 0xf]) {
								request = request.substr(1, end - 1);
								barto_log("GDBSERVER: -> %s\n", request.c_str());
								ack = "+";
								response = "$";
								if(request.substr(0, strlen("qSupported")) == "qSupported") {
									response += "PacketSize=512;BreakpointCommands+;swbreak+;hwbreak+;QStartNoAckMode+;vContSupported+;";
								} else if(request.substr(0, strlen("qAttached")) == "qAttached") {
									response += "1";
								} else if(request.substr(0, strlen("qTStatus")) == "qTStatus") {
									response += "T0";
								} else if(request.substr(0, strlen("QStartNoAckMode")) == "QStartNoAckMode") {
									send_ack(ack);
									useAck = false;
									response += "OK";
								} else if(request.substr(0, strlen("qfThreadInfo")) == "qfThreadInfo") {
									response += "m1";
								} else if(request.substr(0, strlen("qsThreadInfo")) == "qsThreadInfo") {
									response += "l";
								} else if(request.substr(0, strlen("qC")) == "qC") {
									response += "QC1";
								} else if(request.substr(0, strlen("qOffsets")) == "qOffsets") {
									auto BADDR = [](auto bptr) { return bptr << 2; };
									auto BSTR = [](auto bstr) { return std::string(reinterpret_cast<char*>(bstr) + 1, bstr[0]); };
									// from debug.cpp@show_exec_tasks
									auto execbase = get_long_debug(4);
									auto ThisTask = get_long_debug(execbase + 276);
									response += "E01";
									if(ThisTask) {
										auto ln_Name = reinterpret_cast<char*>(get_real_address_debug(get_long_debug(ThisTask + 10)));
										barto_log("GDBSERVER: ln_Name = %s\n", ln_Name);
										auto ln_Type = get_byte_debug(ThisTask + 8);
										bool process = ln_Type == 13; // NT_PROCESS
										sections.clear();
										if(process) {
											constexpr auto sizeofLN = 14;
											// not correct when started from CLI
											auto tc_SPLower = get_long_debug(ThisTask + sizeofLN + 44);
											auto tc_SPUpper = get_long_debug(ThisTask + sizeofLN + 48) - 2;
											stackLower = tc_SPLower;
											stackUpper = tc_SPUpper;
											//auto pr_StackBase = BADDR(get_long_debug(ThisTask + 144));
											//stackUpper = pr_StackBase;

											systemStackLower = get_long_debug(execbase + 58);
											systemStackUpper = get_long_debug(execbase + 54);
											auto pr_SegList = BADDR(get_long_debug(ThisTask + 128));
											// not correct when started from CLI
											auto numSegLists = get_long_debug(pr_SegList + 0);
											auto segList = BADDR(get_long_debug(pr_SegList + 12)); // from debug.cpp@debug()
											auto pr_CLI = BADDR(get_long_debug(ThisTask + 172));
											int pr_TaskNum = get_long_debug(ThisTask + 140);
											if(pr_CLI && pr_TaskNum) {
												auto cli_CommandName = BSTR(get_real_address_debug(BADDR(get_long_debug(pr_CLI + 16))));
												barto_log("GDBSERVER: cli_CommandName = %s\n", cli_CommandName.c_str());
												segList = BADDR(get_long_debug(pr_CLI + 60));
												// don't know how to get the real stack except reading current stack pointer
												auto pr_StackSize = get_long_debug(ThisTask + 132);
												stackUpper = m68k_areg(regs, A7 - A0);
												stackLower = stackUpper - pr_StackSize;
											}
											baseText = 0;
											for(int i = 0; segList; i++) {
												auto size = get_long_debug(segList - 4) - 4;
												auto base = segList + 4;
												if(i == 0) {
													baseText = base;
													sizeText = size;
												}
												if(i == 0)
													response = "$";
												else
													response += ";";
												// this is non-standard (we report addresses of all segments), works only with modified gdb
												response += hex32(base);
												sections.push_back(base);
												barto_log("GDBSERVER:   base=%x; size=%x\n", base, size);
												segList = BADDR(get_long_debug(segList));
											}
										}
									}
								} else if(request.substr(0, strlen("qRcmd,")) == "qRcmd,") {
									// "monitor" command. used for profiling
									auto cmd = from_hex(request.substr(strlen("qRcmd,")));
									barto_log("GDBSERVER:   monitor %s\n", cmd.c_str());
									// syntax: monitor profile <num_frames> <unwind_file> <out_file>
									if(cmd.substr(0, strlen("profile")) == "profile") {
										auto s = cmd.substr(strlen("profile "));
										std::string profile_unwindname;
										profile_num_frames = 0;
										profile_outname.clear();

										// get num_frames
										while(s[0] >= '0' && s[0] <= '9') {
											profile_num_frames = profile_num_frames * 10 + s[0] - '0';
											s = s.substr(1);
										}
										profile_num_frames = max(1, min(100, profile_num_frames));
										s = s.substr(1); // skip space

										// get profile_unwindname
										if(s.substr(0, 1) == "\"") {
											auto last = s.find('\"', 1);
											if(last != std::string::npos) {
												profile_unwindname = s.substr(1, last - 1);
												s = s.substr(last + 1);
											} else {
												s.clear();
											}
										} else {
											auto last = s.find(' ', 1);
											if(last != std::string::npos) {
												profile_unwindname = s.substr(0, last);
												s = s.substr(last + 1);
											} else {
												s.clear();
											}
										}

										s = s.substr(1); // skip space

										// get profile_outname
										if(s.substr(0, 1) == "\"") {
											auto last = s.find('\"', 1);
											if(last != std::string::npos) {
												profile_outname = s.substr(1, last - 1);
												s = s.substr(last + 1);
											} else {
												s.clear();
											}
										} else {
											profile_outname = s.substr(1);
										}

										profile_unwind.reset();
										if(!profile_unwindname.empty()) {
											if(auto f = fopen(profile_unwindname.c_str(), "rb")) {
												profile_unwind = std::make_unique<cpu_profiler_unwind[]>(sizeText >> 1);
												fread(profile_unwind.get(), sizeof(cpu_profiler_unwind), sizeText >> 1, f);
												fclose(f);
											}
										}

										if(!profile_outname.empty()) {
											send_ack(ack);
											profile_frame_count = 0;
											debugger_state = state::profile;
											deactivate_debugger();
											return; // response is sent when profile is finished (vsync)
										}
									} else if(cmd == "reset" && debugging_trigger) {
										savestate_quick(0, 0); // restore state saved at process entry
										barto_debug_resources_count = 0;
										response += "OK";
									} else {
										// unknown monitor command
										response += "E01";
									}
								} else if(request.substr(0, strlen("vCont?")) == "vCont?") {
									response += "vCont;c;C;s;S;t;r";
								} else if(request.substr(0, strlen("vCont;")) == "vCont;") {
									auto actions = request.substr(strlen("vCont;"));
									while(!actions.empty()) {
										std::string action;
										// split actions by ';'
										auto semi = actions.find(';');
										if(semi != std::string::npos) {
											action = actions.substr(0, semi);
											actions = actions.substr(semi + 1);
										} else {
											action = actions;
											actions.clear();
										}
										// thread specified by ':'
										auto colon = action.find(':');
										if(colon != std::string::npos) {
											// ignore thread ID
											action = action.substr(0, colon);
										}

										// hmm.. what to do with multiple actions?!

										if(action == "s") { // single-step
											// step over - GDB does this in a different way
											//auto pc = M68K_GETPC;
											//decltype(pc) nextpc;
											//m68k_disasm(pc, &nextpc, pc, 1);
											//trace_mode = TRACE_MATCH_PC;
											//trace_param[0] = nextpc;

											// step in
											trace_param[0] = 1;
											trace_mode = TRACE_SKIP_INS;

											exception_debugging = 1;
											debugger_state = state::connected;
											send_ack(ack);
											return;
										} else if(action == "c") { // continue
											debugger_state = state::connected;
											deactivate_debugger();
											// none work...
											//SetWindowPos(AMonitors[0].hAmigaWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOSIZE); // bring window to top
											//BringWindowToTop(AMonitors[0].hAmigaWnd);
											//SetForegroundWindow(AMonitors[0].hAmigaWnd);
											//setmouseactive(0, 2);
											send_ack(ack);
											return;
										} else if(action[0] == 'r') { // keep stepping in range
											auto comma = action.find(',', 3);
											if(comma != std::string::npos) {
												uaecptr start = strtoul(action.data() + 1, nullptr, 16);
												uaecptr end = strtoul(action.data() + comma + 1, nullptr, 16);
												trace_mode = TRACE_NRANGE_PC;
												trace_param[0] = start;
												trace_param[1] = end;
												debugger_state = state::connected;
												send_ack(ack);
												return;
											}
										} else {
											barto_log("GDBSERVER: unknown vCont action: %s\n", action.c_str());
										}
									}
								} else if(request[0] == 'H') {
									response += "OK";
								} else if(request[0] == 'T') {
									response += "OK";
/*								} else if(request.substr(0, strlen("vRun")) == "vRun") {
									debugger_state = state::wait_for_process;
									activate_debugger();
									send_ack(ack);
									return;
*/								} else if(request[0] == 'D') { // detach
									response += "OK";
/*								} else if(request[0] == '!') { // enable extended mode
									response += "OK";
*/								} else if(request[0] == '?') { // reason for stopping
									response += "S05"; // SIGTRAP
								} else if(request[0] == 's') { // single-step
									assert(!"should have used vCont;s");
								} else if(request[0] == 'c') { // continue
									assert(!"should have used vCont;c");
								} else if(request[0] == 'k') { // kill
									uae_quit();
									deactivate_debugger();
									return;
								} else if(request.substr(0, 2) == "Z0") { // set software breakpoint
									auto comma = request.find(',', strlen("Z0"));
									if(comma != std::string::npos) {
										uaecptr adr = strtoul(request.data() + strlen("Z0,"), nullptr, 16);
										if(adr == 0xffffffff) {
											// step out of kickstart
											trace_mode = TRACE_RANGE_PC;
											trace_param[0] = 0;
											trace_param[1] = 0xF80000;
											response += "OK";
										} else {
											for(auto& bpn : bpnodes) {
												if(bpn.enabled)
													continue;
												bpn.value1 = adr;
												bpn.type = BREAKPOINT_REG_PC;
												bpn.oper = BREAKPOINT_CMP_EQUAL;
												bpn.enabled = 1;
												trace_mode = 0;
												print_breakpoints();
												response += "OK";
												break;
											}
											// TODO: error when too many breakpoints!
										}
									} else
										response += "E01";
								} else if(request.substr(0, 2) == "z0") { // clear software breakpoint
									auto comma = request.find(',', strlen("z0"));
									if(comma != std::string::npos) {
										uaecptr adr = strtoul(request.data() + strlen("z0,"), nullptr, 16);
										if(adr == 0xffffffff) {
											response += "OK";
										} else {
											for(auto& bpn : bpnodes) {
												if(bpn.enabled && bpn.value1 == adr) {
													bpn.enabled = 0;
													trace_mode = 0;
													print_breakpoints();
													response += "OK";
													break;
												}
											}
											// TODO: error when breakpoint not found
										}
									} else
										response += "E01";
								} else if(request.substr(0, 2) == "Z2" || request.substr(0, 2) == "Z3" || request.substr(0, 2) == "Z4") { // Z2: write watchpoint, Z3: read watchpoint, Z4: access watchpoint
									int rwi = 0;
									if(request[1] == '2')
										rwi = 2; // write
									else if(request[1] == '3')
										rwi = 1; // read
									else
										rwi = 1 | 2; // read + write
									auto comma = request.find(',', strlen("Z2"));
									auto comma2 = request.find(',', strlen("Z2,"));
									if(comma != std::string::npos && comma2 != std::string::npos) {
										uaecptr adr = strtoul(request.data() + strlen("Z2,"), nullptr, 16);
										int size = strtoul(request.data() + comma2 + 1, nullptr, 16);
										barto_log("GDBSERVER: write watchpoint at 0x%x, size 0x%x\n", adr, size);
										for(auto& mwn : mwnodes) {
											if(mwn.size)
												continue;
											mwn.addr = adr;
											mwn.size = size;
											mwn.rwi = rwi;
											// defaults from debug.cpp@memwatch()
											mwn.val_enabled = 0;
											mwn.val_mask = 0xffffffff;
											mwn.val = 0;
											mwn.access_mask = MW_MASK_ALL;
											mwn.reg = 0xffffffff;
											mwn.frozen = 0;
											mwn.modval_written = 0;
											mwn.mustchange = 0;
											// mwn.bus_error = 0;
											mwn.reportonly = false;
											mwn.nobreak = false;
											print_watchpoints();
											response += "OK";
											break;
										}
										memwatch_setup();
										// TODO: error when too many watchpoints!
									} else
										response += "E01";
								} else if(request.substr(0, 2) == "z2" || request.substr(0, 2) == "z3" || request.substr(0, 2) == "z4") { // Z2: clear write watchpoint, Z3: clear read watchpoint, Z4: clear access watchpoint
									auto comma = request.find(',', strlen("z2"));
									if(comma != std::string::npos) {
										uaecptr adr = strtoul(request.data() + strlen("z2,"), nullptr, 16);
										for(auto& mwn : mwnodes) {
											if(mwn.size && mwn.addr == adr) {
												mwn.size = 0;
												trace_mode = 0;
												print_watchpoints();
												response += "OK";
												break;
											}
											// TODO: error when watchpoint not found
										}
										memwatch_setup();
									} else
										response += "E01";
								} else if(request[0] == 'g') { // get registers
									response += get_registers();
								} else if(request[0] == 'p') { // get register
									response += get_register(strtoul(request.data() + 1, nullptr, 16));
								} else if(request[0] == 'm') { // read memory
									auto comma = request.find(',');
									if(comma != std::string::npos) {
										std::string mem;
										uaecptr adr = strtoul(request.data() + strlen("m"), nullptr, 16);
										int len = strtoul(request.data() + comma + 1, nullptr, 16);
										barto_log("GDBSERVER: want 0x%x bytes at 0x%x\n", len, adr);
										while(len-- > 0) {
											auto debug_read_memory_8_no_custom = [](uaecptr addr) -> int {
												addrbank* ad;
												ad = &get_mem_bank(addr);
												if(ad && ad != &custom_bank)
													return ad->bget(addr);
												return -1;
											};

											auto data = debug_read_memory_8_no_custom(adr);
											if(data == -1) {
												barto_log("GDBSERVER: error reading memory at 0x%x\n", len, adr);
												response += "E01";
												mem.clear();
												break;
											}
											data &= 0xff; // custom_bget seems to have a problem?
											mem += hex[data >> 4];
											mem += hex[data & 0xf];
											adr++;
										}
										response += mem;
									} else
										response += "E01";
								}
							} else
								barto_log("GDBSERVER: packet checksum mismatch: got %c%c, want %c%c\n", tolower(request[end + 1]), tolower(request[end + 2]), hex[cksum >> 4], hex[cksum & 0xf]);
						} else
							barto_log("GDBSERVER: packet checksum missing\n");
					} else
						barto_log("GDBSERVER: packet end marker '#' not found\n");
				}

				send_ack(ack);
				send_response(response);
			} else if(result == 0) {
				disconnect();
			} else {
				barto_log(_T("GDBSERVER: error receiving data: %d\n"), sock_err);
				disconnect();
			}
		}
		if(!is_connected()) {
			debugger_state = state::inited;
			close();
			deactivate_debugger();
		}
	}

	// called during pause_emulation
	void vsync() {
		if(!enabled) // "gdbserver"
			return;

		// continue emulation if receiving debug commands
		if(debugger_state == state::connected && data_available()) {
			fsemu_action_process_command_in_main(FSEMU_ACTION_PAUSE, 0);
			// handle_packet will be called in next call to vsync_pre
		}
	}

	void vsync_pre() {
		if(!enabled) // "gdbserver"
			return;

		static uae_u32 profile_start_cycles{};
		static size_t profile_custom_regs_size{};
		static uae_u8* profile_custom_regs{}; // at start of profile 
		static FILE* profile_outfile{};

		if(debugger_state == state::profile) {
start_profile:
			// start profiling
			barto_log("PRF: %d/%d\n", profile_frame_count + 1, profile_num_frames);
			if(profile_frame_count == 0) {
				profile_outfile = fopen(profile_outname.c_str(), "wb");
				if(!profile_outfile) {
					send_response("$E01");
					debugger_state = state::debugging;
					activate_debugger();
					return;
				}
				int section_count = (int)sections.size();
				fwrite(&profile_num_frames, sizeof(int), 1, profile_outfile);
				fwrite(&section_count, sizeof(int), 1, profile_outfile);
				fwrite(sections.data(), sizeof(uint32_t), section_count, profile_outfile);
				fwrite(&systemStackLower, sizeof(uint32_t), 1, profile_outfile);
				fwrite(&systemStackUpper, sizeof(uint32_t), 1, profile_outfile);
				fwrite(&stackLower, sizeof(uint32_t), 1, profile_outfile);
				fwrite(&stackUpper, sizeof(uint32_t), 1, profile_outfile);

				// store chipmem
				auto profile_chipmem_size = chipmem_bank.reserved_size;
				auto profile_chipmem = std::make_unique<uint8_t[]>(profile_chipmem_size);
				memcpy(profile_chipmem.get(), chipmem_bank.baseaddr, profile_chipmem_size);

				// store bogomem
				auto profile_bogomem_size = bogomem_bank.reserved_size;
				auto profile_bogomem = std::make_unique<uint8_t[]>(profile_bogomem_size);
				memcpy(profile_bogomem.get(), bogomem_bank.baseaddr, profile_bogomem_size);

				// kickstart
				// from memory.cpp@save_rom()
				auto kick_start = 0xf80000;
				auto kick_real_start = kickmem_bank.baseaddr;
				auto kick_size = kickmem_bank.reserved_size;
				// 256KB or 512KB ROM?
				int i;
				for(i = 0; i < kick_size / 2 - 4; i++) {
					if(get_long_debug(i + kick_start) != get_long_debug(i + kick_start + kick_size / 2))
						break;
				}
				if(i == kick_size / 2 - 4) {
					kick_size /= 2;
					kick_start += ROM_SIZE_256;
				}

				fwrite(&kick_size, sizeof(kick_size), 1, profile_outfile);
				fwrite(kick_real_start, 1, kick_size, profile_outfile);

				// memory
				fwrite(&profile_chipmem_size, sizeof(profile_chipmem_size), 1, profile_outfile);
				fwrite(profile_chipmem.get(), 1, profile_chipmem_size, profile_outfile);
				fwrite(&profile_bogomem_size, sizeof(profile_bogomem_size), 1, profile_outfile);
				fwrite(profile_bogomem.get(), 1, profile_bogomem_size, profile_outfile);

				// CPU information
				fwrite(&baseclock, sizeof(int), 1, profile_outfile);
				fwrite(&cpucycleunit, sizeof(int), 1, profile_outfile);
			}

			// store custom registers
			profile_custom_regs = save_custom(&profile_custom_regs_size, 0, TRUE);

			// reset idle
			if(barto_debug_idle_count > 0) {
				barto_debug_idle[0] = barto_debug_idle[barto_debug_idle_count - 1] & 0x80000000;
				barto_debug_idle_count = 1;
			}

			// start profiler
			start_cpu_profiler(baseText, baseText + sizeText, profile_unwind.get());
			debug_dma = 1;
			profile_start_cycles = static_cast<uae_u32>(get_cycles() / cpucycleunit);
			//barto_log("GDBSERVER: Start CPU Profiler @ %u cycles\n", get_cycles() / cpucycleunit);
			debugger_state = state::profiling;
		} else if(debugger_state == state::profiling) {
			profile_frame_count++;
			// end profiling
			stop_cpu_profiler();
			debug_dma = 0;
			uae_u32 profile_end_cycles = static_cast<uae_u32>(get_cycles() / cpucycleunit);
			//barto_log("GDBSERVER: Stop CPU Profiler @ %u cycles => %u cycles\n", profile_end_cycles, profile_end_cycles - profile_start_cycles);

			// process dma records
			static constexpr int NR_DMA_REC_HPOS_IN = 256, NR_DMA_REC_VPOS_IN = 1000;
			static constexpr int NR_DMA_REC_HPOS_OUT = 227, NR_DMA_REC_VPOS_OUT = 313;
			auto dma_in = get_dma_records();
			auto dma_out = std::make_unique<dma_rec[]>(NR_DMA_REC_HPOS_OUT * NR_DMA_REC_VPOS_OUT);
			for(size_t y = 0; y < NR_DMA_REC_VPOS_OUT; y++) {
				for(size_t x = 0; x < NR_DMA_REC_HPOS_OUT; x++) {
					dma_out[y * NR_DMA_REC_HPOS_OUT + x] = dma_in[y * NR_DMA_REC_HPOS_IN + x];
				}
			}

			int profile_cycles = profile_end_cycles - profile_start_cycles;

			// calculate idle cycles
			int idle_cycles = 0;
			int last_idle = 0;
			for(int i = 0; i < barto_debug_idle_count; i++) {
				auto this_idle = barto_debug_idle[i];
				if((last_idle & 0x80000000) && !(this_idle & 0x80000000)) { // idle->busy
					idle_cycles += (this_idle & 0x7fffffff) - max(profile_start_cycles, (last_idle & 0x7fffffff));
				}

				if((this_idle ^ last_idle) & 0x80000000)
					last_idle = this_idle;
			}
			if(last_idle & 0x80000000)
				idle_cycles += profile_end_cycles - max(profile_start_cycles, (last_idle & 0x7fffffff));
			//barto_log("GDBSERVER: idle_cycles: %d\n", idle_cycles);

			// Custom Regs
			int custom_len = (int)profile_custom_regs_size;
			fwrite(&custom_len, sizeof(int), 1, profile_outfile);
			fwrite(profile_custom_regs, 1, custom_len, profile_outfile);
			free(profile_custom_regs);
			profile_custom_regs = nullptr;

			// DMA
			int dmarec_size = sizeof(dma_rec);
			int dmarec_count = NR_DMA_REC_HPOS_OUT * NR_DMA_REC_VPOS_OUT;
			fwrite(&dmarec_size, sizeof(int), 1, profile_outfile);
			fwrite(&dmarec_count, sizeof(int), 1, profile_outfile);
			fwrite(dma_out.get(), sizeof(dma_rec), NR_DMA_REC_HPOS_OUT * NR_DMA_REC_VPOS_OUT, profile_outfile);

			// resources
			int resource_size = sizeof(barto_debug_resource);
			int resource_count = barto_debug_resources_count;
			fwrite(&resource_size, sizeof(int), 1, profile_outfile);
			fwrite(&resource_count, sizeof(int), 1, profile_outfile);
			fwrite(barto_debug_resources, resource_size, resource_count, profile_outfile);

			fwrite(&profile_cycles, sizeof(int), 1, profile_outfile);
			fwrite(&idle_cycles, sizeof(int), 1, profile_outfile);

			// profiles
			int profile_count = get_cpu_profiler_output_count();
			fwrite(&profile_count, sizeof(int), 1, profile_outfile);
			fwrite(get_cpu_profiler_output(), sizeof(uae_u32), profile_count, profile_outfile);

			// write screenshot
			redraw_frame();
			auto frame = uae_fsvideo_getframe();

			// need to flip bits and swap rgb channels
			int w = frame->width;
			int h = frame->height;
			uint8_t *bi_bits = frame->buffer;
			auto bits = std::make_unique<uint8_t[]>(w * 3 * h);
			for(int y = 0; y < h; y++) {
				for(int x = 0; x < w; x++) {
					bits[y * w * 3 + x * 3 + 0] = bi_bits[y * w * 4 + x * 4 + 2];
					bits[y * w * 3 + x * 3 + 1] = bi_bits[y * w * 4 + x * 4 + 1];
					bits[y * w * 3 + x * 3 + 2] = bi_bits[y * w * 4 + x * 4 + 0];
				}
			}
			struct write_context_t {
				uint8_t data[2'000'000]{};
				int size = 0;
				int type = 0;
			};
			auto write_context = std::make_unique<write_context_t>();
			auto write_func = [](void* _context, void* data, int size) {
				auto context = (write_context_t*)_context;
				memcpy(&context->data[context->size], data, size);
				context->size += size;
			};
			if(profile_num_frames > 1) {
				stbi_write_jpg_to_func(write_func, write_context.get(), w, h, 3, bits.get(), 50);
				write_context->type = 0; // JPG
			} else {
				stbi_write_png_to_func(write_func, write_context.get(), w, h, 3, bits.get(), w * 3);
				write_context->type = 1; // PNG
			}
			write_context->size = (write_context->size + 3) & ~3; // pad to 32bit
			fwrite(&write_context->size, sizeof(int), 1, profile_outfile);
			fwrite(&write_context->type, sizeof(int), 1, profile_outfile);
			fwrite(write_context->data, 1, write_context->size, profile_outfile);

			if(profile_frame_count == profile_num_frames) {
				fclose(profile_outfile);
				send_response("$OK");

				debugger_state = state::debugging;
				activate_debugger();
			} else {
				debugger_state = state::profile;
				goto start_profile;
			}
		}

		if(debugger_state == state::connected && data_available()) {
			handle_packet();
		}
	}

	void vsync_post() {
		if(!enabled)
			return;
	}

	uaecptr KPutCharX{};
	uaecptr Trap7{};
	uaecptr AddressError{};
	uaecptr IllegalError{};
	std::string KPutCharOutput;

	void output(const char* string) {
		if(gdbconn != INVALID_SOCKET && !in_handle_packet) {
			std::string response = "$O";
			while(*string)
				response += hex8(*string++);
			send_response(response);
		}
	}

	/*
	void log_output(const TCHAR* tstring) {
		auto utf8 = string_to_utf8(tstring);
		if(utf8.substr(0, 5) == "DBG: ") {
			utf8 = utf8.substr(0, utf8.length() - 1); // get rid of extra newline from uaelib
			for(size_t start = 0;;) { // append "DBG: " to every newline, because GDB splits text by lines and vscode doesn't know that the extra lines are DBG output
				auto p = utf8.find('\n', start);
				if(p == std::string::npos || p == utf8.length() - 1)
					break;

				utf8.replace(p, 1, "\nDBG: ");
				start = p + 6;
			}

		}
		output(utf8.c_str());
	}
	*/

	// returns true if gdbserver handles debugging
	bool debug() {
		if(!enabled)
			return false;

		fsemu_action_process_command_in_main(FSEMU_ACTION_WARP, 0);

		// break at start of process
		if(debugger_state == state::inited) {
			if(debugging_trigger) {
				//KPutCharX
				auto execbase = get_long_debug(4);
				KPutCharX = execbase - 0x204;
				for(auto& bpn : bpnodes) {
					if(bpn.enabled)
						continue;
					bpn.value1 = KPutCharX;
					bpn.type = BREAKPOINT_REG_PC;
					bpn.oper = BREAKPOINT_CMP_EQUAL;
					bpn.enabled = 1;
					barto_log("GDBSERVER: Breakpoint for KPutCharX at 0x%x installed\n", bpn.value1);
					break;
				}

				// TRAP#7 breakpoint (GCC generates this opcode when it encounters undefined behavior)
				Trap7 = get_long_debug(regs.vbr + 0x9c);
				for(auto& bpn : bpnodes) {
					if(bpn.enabled)
						continue;
					bpn.value1 = Trap7;
					bpn.type = BREAKPOINT_REG_PC;
					bpn.oper = BREAKPOINT_CMP_EQUAL;
					bpn.enabled = 1;
					barto_log("GDBSERVER: Breakpoint for TRAP#7 at 0x%x installed\n", bpn.value1);
					break;
				}

				AddressError = get_long_debug(regs.vbr + 3 * 4);
				for(auto& bpn : bpnodes) {
					if(bpn.enabled)
						continue;
					bpn.value1 = AddressError;
					bpn.type = BREAKPOINT_REG_PC;
					bpn.oper = BREAKPOINT_CMP_EQUAL;
					bpn.enabled = 1;
					barto_log("GDBSERVER: Breakpoint for AddressError at 0x%x installed\n", bpn.value1);
					break;
				}

				IllegalError = get_long_debug(regs.vbr + 4 * 4);
				for(auto& bpn : bpnodes) {
					if(bpn.enabled)
						continue;
					bpn.value1 = IllegalError;
					bpn.type = BREAKPOINT_REG_PC;
					bpn.oper = BREAKPOINT_CMP_EQUAL;
					bpn.enabled = 1;
					barto_log("GDBSERVER: Breakpoint for IllegalError at 0x%x installed\n", bpn.value1);
					break;
				}

				// watchpoint for NULL (GCC sees this as undefined behavior)
				// disabled for now, always triggered in OpenScreen()
				/*for(auto& mwn : mwnodes) {
					if(mwn.size)
						continue;
					mwn.addr = 0;
					mwn.size = 4;
					mwn.rwi = 1 | 2; // read + write
					// defaults from debug.cpp@memwatch()
					mwn.val_enabled = 0;
					mwn.val_mask = 0xffffffff;
					mwn.val = 0;
					mwn.access_mask = MW_MASK_CPU_D_R | MW_MASK_CPU_D_W; // CPU data read/write only
					mwn.reg = 0xffffffff;
					mwn.frozen = 0;
					mwn.modval_written = 0;
					mwn.mustchange = 0;
					mwn.bus_error = 0;
					mwn.reportonly = false;
					mwn.nobreak = false;
					memwatch_setup();
					barto_log("GDBSERVER: Watchpoint for NULL installed\n");
					break;
				}*/

				// enable break at exceptions - doesn't break when exceptions occur in Kickstart
				debug_illegal = 1;
				debug_illegal_mask = (1 << 3) | (1 << 4); // 3 = address error, 4 = illegal instruction

				// from debug.cpp@process_breakpoint()
				processptr = 0;
				xfree(processname);
				processname = nullptr;
				savestate_quick(0, 1); // save state for "monitor reset"
			}
			barto_log("GDBSERVER: Waiting for connection...\n");
			for (int i = 0; i < time_out * 10; i++)	{
				if (is_connected()) {
					barto_log("GDBSERVER: connected\n");
					useAck = true;
					debugger_state = state::debugging;
					debugmem_enable_stackframe(true);
					debugmem_trace = true;
					break;
				}
				usleep(100000);
			}
			if (debugger_state != state::debugging) {
				barto_log("GDBSERVER: timed out after %ds\n", time_out);
			}
		}

		// something stopped execution and entered debugger
		if(debugger_state == state::connected) {
//while(!IsDebuggerPresent()) Sleep(100); __debugbreak();
			auto pc = munge24(m68k_getpc());
			if (pc == KPutCharX) {
				// if this is too slow, hook uaelib trap#86
				auto ascii = static_cast<uint8_t>(m68k_dreg(regs, 0));
				KPutCharOutput += ascii;
				if(ascii == '\0') {
					std::string response = "$O";
					for(const auto& ch : KPutCharOutput)
						response += hex8(ch);
					send_response(response);
					KPutCharOutput.clear();
				}
				deactivate_debugger();
				return true;
			}

			std::string response{ "S05" };

			//if(memwatch_triggered) // can't use, debug() will reset it, so just check mwhit
			if(mwhit.size) {
				for(const auto& mwn : mwnodes) {
					if(mwn.size && mwhit.addr >= mwn.addr && mwhit.addr < mwn.addr + mwn.size) {
						if(mwn.addr == 0) {
							response = "S0B"; // undefined behavior -> SIGSEGV
						} else {
//while(!IsDebuggerPresent()) Sleep(100); __debugbreak();
//							auto data = get_long_debug(mwn.addr);
							response = "T05";
							if(mwhit.rwi == 2)
								response += "watch";
							else if(mwhit.rwi == 1)
								response += "rwatch";
							else
								response += "awatch";
							response += ":";
							response += hex32(mwhit.addr);
							response += ";";
						}
						// so we don't trigger again
						mwhit.size = 0;
						mwhit.addr = 0;
						goto send_response;
					}
				}
			}
			for(const auto& bpn : bpnodes) {
				if(bpn.enabled && bpn.type == BREAKPOINT_REG_PC && bpn.value1 == pc) {
					// see binutils-gdb/include/gdb/signals.def for number of signals
					if(pc == Trap7) {
						response = "S07"; // TRAP#7 -> SIGEMT
						// unwind PC & stack for better debugging experience (otherwise we're probably just somewhere in Kickstart)
						regs.pc = regs.instruction_pc_user_exception - 2;
						m68k_areg(regs, A7 - A0) = regs.usp;
					} else if(pc == AddressError) {
						response = "S0A"; // AddressError -> SIGBUS
						// unwind PC & stack for better debugging experience (otherwise we're probably just somewhere in Kickstart)
						regs.pc = regs.instruction_pc_user_exception; // don't know size of opcode that caused exception
						m68k_areg(regs, A7 - A0) = regs.usp;
					} else if(pc == IllegalError) {
						response = "S04"; // AddressError -> SIGILL
						// unwind PC & stack for better debugging experience (otherwise we're probably just somewhere in Kickstart)
						regs.pc = regs.instruction_pc_user_exception; // don't know size of opcode that caused exception
						m68k_areg(regs, A7 - A0) = regs.usp;
					} else {
						response = "T05swbreak:;";
					}
					goto send_response;
				}
			}
send_response:
			send_response("$" + response);
			trace_mode = 0;
			debugger_state = state::debugging;
		}

		// debugger active
		while(debugger_state == state::debugging) {
			handle_packet();

			#ifdef _WIN32
			MSG msg{};
			while(PeekMessage(&msg, nullptr, 0U, 0U, PM_REMOVE)) {
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			#endif
			Sleep(1);
		}

		return true;
	}
} // namespace barto_gdbserver
