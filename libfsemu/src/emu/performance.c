#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#define FSE_INTERNAL_API
#include <fsemu/performance.h>

#include <fs/emu/path.h>
#include <fs/log.h>
#include <fs/glib.h>
#include <fs/base.h>
#include <fs/thread.h>

#include <inttypes.h>

typedef struct fsemu_performance_entry
{
  char series[4];
  int64_t time;
  int64_t value;
} fsemu_performance_entry;

const int BUFFER_SIZE = 1024;

static int64_t fsemu_performance_epoch;
static volatile fsemu_performance_entry *buffer;
static fsemu_performance_entry *buffer_end;
static volatile fsemu_performance_entry * volatile write_p;
static volatile fsemu_performance_entry * read_p;
static FILE *fsemu_performance_file;
// Write mutex, to allow writing to the ring buffer from multiple threads
static fs_mutex *fsemu_performance_mutex;

void fsemu_performance_log_with_time(char series[4], int64_t value, int64_t time)
{
  if (buffer == NULL) {
    return;
  }
  fs_mutex_lock(fsemu_performance_mutex);
  write_p->series[0] = series[0];
  write_p->series[1] = series[1];
  write_p->series[2] = series[2];
  write_p->series[3] = series[3];
  write_p->time = time;
  write_p->value = value;
  if (++write_p == buffer_end) {
    write_p = buffer;
  }
  fs_mutex_unlock(fsemu_performance_mutex);
}

void fsemu_performance_log(char series[4], int64_t value)
{
  fsemu_performance_log_with_time(series, value, fs_get_monotonic_time());
}

void fsemu_performance_flush(void)
{
  char series[5] = { 0 };
  volatile fsemu_performance_entry *p = read_p;
  // const fsemu_performance_entry *write_p_copy = write_p;
  if (read_p > write_p) {
    // Read remaining ring buffer when writing has wrapped
    while(p < buffer_end) {
      series[0] = p->series[0];
      series[1] = p->series[1];
      series[2] = p->series[2];
      series[3] = p->series[3];
      int64_t time = p->time - fsemu_performance_epoch;
      int64_t value = p->value;
      fprintf(fsemu_performance_file, "%s,%" PRId64 ",%" PRId64 "\n", series, time, value);
      ++p;
    }
    p = buffer;
  }
  while(p < write_p) {
    series[0] = p->series[0];
    series[1] = p->series[1];
    series[2] = p->series[2];
    series[3] = p->series[3];
    int64_t time = p->time - fsemu_performance_epoch;
    int64_t value = p->value;
    fprintf(fsemu_performance_file, "%s,%" PRId64 ",%" PRId64 "\n", series, time, value);
    ++p;
  }
  read_p = p;
}

void fsemu_performance_init(void)
{
  const gchar *log_file_path = g_getenv("FSEMU_PERFORMANCE_LOG");
  if (log_file_path) {
    fs_log("Performance: Log file is %s\n", log_file_path);
    fsemu_performance_file = g_fopen(log_file_path, "wb");
  }
  if (fsemu_performance_file) {
    fsemu_performance_mutex = fs_mutex_create();
    fsemu_performance_entry *allocated = malloc(sizeof(fsemu_performance_entry) * BUFFER_SIZE);
    buffer = allocated;
    buffer_end = allocated + BUFFER_SIZE;
    read_p = buffer;
    write_p = buffer;
    fsemu_performance_epoch = fs_get_monotonic_time();
    fs_log("Performance: Buffer initialized with %d entries\n", BUFFER_SIZE);
  } else {
    fs_log("Performance: Could not open log file for writing\n");
  }
}
