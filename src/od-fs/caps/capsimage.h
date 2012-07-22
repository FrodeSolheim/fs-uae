#ifndef CAPS_CAPSIMAGE_H
#define CAPS_CAPSIMAGE_H

#ifdef __AMIGA__
#include <exec/types.h>
typedef UBYTE CapsUByte;
typedef LONG  CapsLong;
typedef ULONG CapsULong;
#else
#include <stdint.h>
typedef uint8_t  CapsUByte;
typedef int32_t  CapsLong;
typedef uint32_t CapsULong;
#endif // __AMIGA__

#define CAPS_FILEEXT "ipf"
#define CAPS_FILEPFX ".ipf"
#define CAPS_NAME "capsimage.device"

#define CAPS_UNITS 10

// Flags provided for locking, in order:
// 0: re-align data as index synced recording
// 1: decode track to word aligned size
// 2: generate cell density for variable density tracks
// 3: generate density for automatically sized cells
// 4: generate density for unformatted cells
// 5: generate unformatted data
// 6: generate unformatted data, that changes each revolution
// 7: directly use source memory buffer supplied with LockImageMemory
// 8: flakey data is created on one revolution, updated with each lock
// 9: ...Info.type holds the expected structure type
#define DI_LOCK_INDEX    (1L<<0)
#define DI_LOCK_ALIGN    (1L<<1)
#define DI_LOCK_DENVAR   (1L<<2)
#define DI_LOCK_DENAUTO  (1L<<3)
#define DI_LOCK_DENNOISE (1L<<4)
#define DI_LOCK_NOISE    (1L<<5)
#define DI_LOCK_NOISEREV (1L<<6)
#define DI_LOCK_MEMREF   (1L<<7)
#define DI_LOCK_UPDATEFD (1L<<8)
#define DI_LOCK_TYPE     (1L<<9)

#define CAPS_MAXPLATFORM 4
#define CAPS_MTRS 5

#define CTIT_FLAG_FLAKEY (1L<<31)
#define CTIT_MASK_TYPE 0xff

#if defined(__GNUC__) && !defined(__mc68000__)
#pragma pack(1)
#endif // __GNUC__

// decoded caps date.time
struct CapsDateTimeExt {
    CapsULong year;
    CapsULong month;
    CapsULong day;
    CapsULong hour;
    CapsULong min;
    CapsULong sec;
    CapsULong tick;
};

// library version information block
struct CapsVersionInfo {
    CapsULong type;     // library type
    CapsULong release;  // release ID
    CapsULong revision; // revision ID
    CapsULong flag;     // supported flags
};

// disk image information block
struct CapsImageInfo {
    CapsULong type;        // image type
    CapsULong release;     // release ID
    CapsULong revision;    // release revision ID
    CapsULong mincylinder; // lowest cylinder number
    CapsULong maxcylinder; // highest cylinder number
    CapsULong minhead;     // lowest head number
    CapsULong maxhead;     // highest head number
    struct CapsDateTimeExt crdt; // image creation date.time
    CapsULong platform[CAPS_MAXPLATFORM]; // intended platform(s)
};

// disk track information block
struct CapsTrackInfo {
    CapsULong type;       // track type
    CapsULong cylinder;   // cylinder#
    CapsULong head;       // head#
    CapsULong sectorcnt;  // available sectors
    CapsULong sectorsize; // sector size
    CapsULong trackcnt;   // track variant count
    CapsUByte *trackbuf;  // track buffer memory
    CapsULong tracklen;   // track buffer memory length
    CapsUByte *trackdata[CAPS_MTRS]; // track data pointer if available
    CapsULong tracksize[CAPS_MTRS]; // track data size
    CapsULong timelen;    // timing buffer length
    CapsULong *timebuf;   // timing buffer
};

// disk track information block
struct CapsTrackInfoT1 {
    CapsULong type;       // track type
    CapsULong cylinder;   // cylinder#
    CapsULong head;       // head#
    CapsULong sectorcnt;  // available sectors
    CapsULong sectorsize; // sector size
    CapsUByte *trackbuf;  // track buffer memory
    CapsULong tracklen;   // track buffer memory length
    CapsULong timelen;    // timing buffer length
    CapsULong *timebuf;   // timing buffer
    CapsULong overlap;    // overlap position
};

#if defined(__GNUC__) && !defined(__mc68000__)
#pragma pack()
#endif // __GNUC__

// image type
enum {
    ciitNA=0, // invalid image type
    ciitFDD   // floppy disk
};

// platform IDs, not about configuration, but intended use
enum {
    ciipNA=0,    // invalid platform (dummy entry)
    ciipAmiga,   // Amiga
    ciipAtariST, // Atari ST
    ciipPC       // PC
};

// track type
enum {
    ctitNA=0,  // invalid type
    ctitNoise, // cells are unformatted (random size)
    ctitAuto,  // automatic cell size, according to track size
    ctitVar    // variable density
};

// image error status
enum {
    imgeOk,
    imgeUnsupported,
    imgeGeneric,
    imgeOutOfRange,
    imgeReadOnly,
    imgeOpen,
    imgeType,
    imgeShort,
    imgeTrackHeader,
    imgeTrackStream,
    imgeTrackData,
    imgeDensityHeader,
    imgeDensityStream,
    imgeDensityData,
    imgeIncompatible,
    imgeUnsupportedType
};

#ifndef CLIB_CAPSIMAGE_PROTOS_H

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

CapsLong CAPSInit(void);
CapsLong CAPSExit(void);
CapsLong CAPSAddImage(void);
CapsLong CAPSRemImage(CapsLong id);
CapsLong CAPSLockImage(CapsLong id, char *name);
CapsLong CAPSLockImageMemory(CapsLong id, CapsUByte *buffer, CapsULong length, CapsULong flag);
CapsLong CAPSUnlockImage(CapsLong id);
CapsLong CAPSLoadImage(CapsLong id, CapsULong flag);
CapsLong CAPSGetImageInfo(struct CapsImageInfo *pi, CapsLong id);
CapsLong CAPSLockTrack(struct CapsTrackInfo *pi, CapsLong id, CapsULong cylinder, CapsULong head, CapsULong flag);
CapsLong CAPSUnlockTrack(CapsLong id, CapsULong cylinder, CapsULong head);
CapsLong CAPSUnlockAllTracks(CapsLong id);
char *CAPSGetPlatformName(CapsULong pid);
CapsLong CAPSGetVersionInfo(struct CapsVersionInfo *pi, CapsULong flag);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // CLIB_CAPSIMAGE_PROTOS_H

#endif // CAPS_CAPSIMAGE_H
