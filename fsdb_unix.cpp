 /*
  * UAE - The Un*x Amiga Emulator
  *
  * Library of functions to make emulated filesystem as independent as
  * possible of the host filesystem's capabilities.
  * This is the Unix version.
  *
  * Copyright 1999 Bernd Schmidt
  */

#include "sysconfig.h"
#include "sysdeps.h"

#include "fsdb.h"

/* Return nonzero for any name we can't create on the native filesystem.  */
int fsdb_name_invalid (const char *n)
{
    if (strcmp (n, FSDB_FILE) == 0)
	return 1;
    if (n[0] != '.')
	return 0;
    if (n[1] == '\0')
	return 1;
    return n[1] == '.' && n[2] == '\0';
}

/* For an a_inode we have newly created based on a filename we found on the
 * native fs, fill in information about this file/directory.  */
int fsdb_fill_file_attrs (a_inode *aino)
{
    struct stat statbuf;
    /* This really shouldn't happen...  */
    if (stat (aino->nname, &statbuf) == -1)
	return 0;
    aino->dir = S_ISDIR (statbuf.st_mode) ? 1 : 0;
    aino->amigaos_mode = ((S_IXUSR & statbuf.st_mode ? 0 : A_FIBF_EXECUTE)
			  | (S_IWUSR & statbuf.st_mode ? 0 : A_FIBF_WRITE)
			  | (S_IRUSR & statbuf.st_mode ? 0 : A_FIBF_READ));
    return 1;
}

int fsdb_set_file_attrs (a_inode *aino, int mask)
{
    struct stat statbuf;
    int mode;

    if (stat (aino->nname, &statbuf) == -1)
	return ERROR_OBJECT_NOT_AROUND;

    mode = statbuf.st_mode;
    /* Unix dirs behave differently than AmigaOS ones.  */
    if (! aino->dir) {
	if (mask & A_FIBF_READ)
	    mode &= ~S_IRUSR;
	else
	    mode |= S_IRUSR;

	if (mask & A_FIBF_WRITE)
	    mode &= ~S_IWUSR;
	else
	    mode |= S_IWUSR;

	if (mask & A_FIBF_EXECUTE)
	    mode &= ~S_IXUSR;
	else
	    mode |= S_IXUSR;

	chmod (aino->nname, mode);
    }

    aino->amigaos_mode = mask;
    aino->dirty = 1;
    return 0;
}

/* Return nonzero if we can represent the amigaos_mode of AINO within the
 * native FS.  Return zero if that is not possible.  */
int fsdb_mode_representable_p (const a_inode *aino)
{
    if (aino->dir)
	return aino->amigaos_mode == 0;
    return (aino->amigaos_mode & (A_FIBF_DELETE | A_FIBF_SCRIPT | A_FIBF_PURE)) == 0;
}

char *fsdb_create_unique_nname (a_inode *base, const char *suggestion)
{
    char tmp[256] = "__uae___";
    strncat (tmp, suggestion, 240);
    for (;;) {
	int i;
	char *p = build_nname (base->nname, tmp);
	if (access (p, R_OK) < 0 && errno == ENOENT) {
	    printf ("unique name: %s\n", p);
	    return p;
	}
	free (p);

	/* tmpnam isn't reentrant and I don't really want to hack configure
	 * right now to see whether tmpnam_r is available...  */
	for (i = 0; i < 8; i++) {
	    tmp[i] = "_abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789"[uaerand () % 63];
	}
    }
}
