#ifndef FSEMU_PYTHON_H_
#define FSEMU_PYTHON_H_

#ifdef __cplusplus
extern "C" {
#endif

void fsemu_python_init(void);

void fsemu_python_apply_options(void (*callback)(const char *name,
                                                 const char *value));

#ifdef __cplusplus
}
#endif

#endif  // FSEMU_PYTHON_H_
