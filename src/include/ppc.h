#ifndef UAE_PPC_H
#define UAE_PPC_H

void ppc_reboot(void);
void ppc_stop(void);
void uae_ppc_poll_queue(void);
void ppc_interrupt(bool active);

void ppc_hsync_handler(void);
void ppc_wakeup(void);
void ppc_crash(void);

#define PPC_STATE_STOP 0
#define PPC_STATE_ACTIVE 1
#define PPC_STATE_SLEEP 2
#define PPC_STATE_CRASH 3

extern volatile int ppc_state;

#endif // UAE_PPC_H
