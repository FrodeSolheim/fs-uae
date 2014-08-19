#ifndef UAE_LOGGING_H
#define UAE_LOGGING_H

#define UAE_LOG_STUB(format, ...) \
{ \
	write_log(" -- stub -- %s " format "\n", __func__, ##__VA_ARGS__); \
}

#define UAE_LOG_STUB_MAX(max, format, ...) \
{ \
	static int log_stub_count = 0; \
	if (log_stub_count < max) { \
		LOG_STUB(format, ##__VA_ARGS__) \
		if (++log_stub_count == max) { \
			write_log("    (ignoring further calls to %s)\n", __func__); \
		} \
	} \
}

#define UAE_STUB(format, ...) \
{ \
	UAE_LOG_STUB(format, ##__VA_ARGS__) \
	printf(" -- stub -- %s " format "\n", __func__, ##__VA_ARGS__); \
}

/* start of deprecated defines */

#define STUB UAE_STUB
#define LOG_STUB UAE_LOG_STUB
#define LOG_STUB_MAX UAE_LOG_STUB_MAX
#define VERBOSE_STUB STUB

/* end of deprecated defines */

#endif // UAE_LOGGING_H
