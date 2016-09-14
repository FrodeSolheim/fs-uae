#ifndef UAE_PARALLEL_H
#define UAE_PARALLEL_H

#include "uae/types.h"

extern int isprinter (void);
extern void doprinter (uae_u8);
extern void flushprinter (void);
extern void closeprinter (void);
extern int isprinteropen (void);
extern void initparallel (void);

void parallel_exit(void);
#ifdef FSUAE
void parallel_ack(void);
void parallel_poll_ack(void);
#endif

#endif /* UAE_PARALLEL_H */
