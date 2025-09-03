#ifndef FSAPP_INSTANCE_H
#define FSAPP_INSTANCE_H

#ifdef __cplusplus
extern "C" {
#endif

// Single instance guard return codes
typedef enum {
    FSAPP_INSTANCE_OK = 0,           // Successfully acquired instance lock
    FSAPP_INSTANCE_ALREADY_RUNNING,  // Another instance is already running
    FSAPP_INSTANCE_ERROR             // Error occurred (permissions, etc.)
} fsapp_instance_result_t;

/**
 * Attempt to acquire the single instance lock.
 * Should be called very early in main() before heavy initialization.
 *
 * @param app_name Unique application identifier (e.g., "FS-UAE", "MyApp")
 * @return FSAPP_INSTANCE_OK if successful, error code otherwise
 */
fsapp_instance_result_t fsapp_instance_acquire_lock(const char* app_name);

/**
 * Release the single instance lock.
 * Should be called during cleanup/exit.
 */
void fsapp_instance_release_lock(void);

#ifdef __cplusplus
}
#endif

#endif  // FSAPP_INSTANCE_H