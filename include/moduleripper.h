#ifndef UAE_MODULERIPPER_H
#define UAE_MODULERIPPER_H

#ifdef FSUAE
// FS-UAE builds prowizard as a C library
#ifdef __cplusplus
extern "C" {
#endif
#endif
extern int prowizard_search (unsigned char *in_data, int in_size);
#ifdef FSUAE
#ifdef __cplusplus
}
#endif
#endif
extern void moduleripper (void);

#endif /* UAE_MODULERIPPER_H */
