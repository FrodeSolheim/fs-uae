// Implement MMAN and SHM functionality for Win32
// Copyright (C) 2000, Brian King
// GNU Public License

#ifndef UAE_MMAN_H
#define UAE_MMAN_H

#include "uae/types.h"
#ifdef FSUAE
#include "uae/limits.h"
#include "uae/memory.h"
#include <stdio.h> /* for size_t */
#endif

#define MAX_SHMID 256

typedef int uae_key_t;

/* One shmid data structure for each shared memory segment in the system. */
struct uae_shmid_ds {
    uae_key_t key;
    size_t size;
    size_t rosize;
    void *addr;
    TCHAR name[MAX_PATH];
    void *attached;
    int mode;
    void *natmembase;
    bool fake;
    int maprom;
};

void *uae_shmat(addrbank *ab, int shmid, void *shmaddr, int shmflg);
int uae_shmdt(const void *shmaddr);
int uae_shmget(uae_key_t key, size_t size, int shmflg, const TCHAR *name);
int uae_shmctl(int shmid, int cmd, struct uae_shmid_ds *buf);

#define PROT_READ   0x01
#define PROT_WRITE  0x02
#define PROT_EXEC   0x04

#define IPC_PRIVATE 0x01
#define IPC_RMID    0x02
#define IPC_CREAT   0x04
#define IPC_STAT    0x08

#endif /* UAE_MMAN_H */
