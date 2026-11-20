#ifndef FSUAE_MAIN_H
#define FSUAE_MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

void fsuae_main_init(void);

// Logs the test log lines that FSUAE_TEST_LOG_LINES asks for (none without it): called by the
// emulation thread at the end of every emulated frame.
void fsuae_main_log_test_lines(void);

#ifdef __cplusplus
}
#endif

#endif /* FSUAE_MAIN_H */
