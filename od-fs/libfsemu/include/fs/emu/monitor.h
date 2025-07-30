#ifndef FS_EMU_MONITOR_H
#define FS_EMU_MONITOR_H

typedef struct FSEmuMonitorRect {
    int x;
    int y;
    int w;
    int h;
} FSEmuMonitorRect;

#define FS_EMU_MONITOR_FLAG_LEFT (1 << 0)
#define FS_EMU_MONITOR_FLAG_MIDDLE_LEFT (1 << 1)
#define FS_EMU_MONITOR_FLAG_MIDDLE_RIGHT (1 << 2)
#define FS_EMU_MONITOR_FLAG_RIGHT (1 << 3)

#define FS_EMU_MONITOR_MAX_COUNT 8

typedef struct FSEmuMonitor {
    int index;
    int flags;
    FSEmuMonitorRect rect;
    int refresh_rate;
} FSEmuMonitor;

int fs_emu_monitor_count();
bool fs_emu_monitor_get_by_index(int index, FSEmuMonitor* monitor);
bool fs_emu_monitor_get_by_flag(int flag, FSEmuMonitor* monitor);

#endif /* FS_EMU_MONITOR_H */
