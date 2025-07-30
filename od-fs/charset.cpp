#include "sysconfig.h"
#include "sysdeps.h"

#include <string.h>

// fs-uae uses only chars / UTF-8 internally, so TCHAR is typedefed to
// char (WinUAE uses wchar_t internally).

char *ua (const TCHAR *s) {
	if (s == NULL) return NULL;
	return strdup (s);
}

char *au (const TCHAR *s) {
	if (s == NULL) return NULL;
	return strdup (s);
}

TCHAR* utf8u (const char *s)
{
	if (s == NULL) return NULL;
	return ua (s);
}

char* uutf8 (const TCHAR *s)
{
	if (s == NULL) return NULL;
	return ua (s);
}

TCHAR *au_copy (TCHAR *dst, int maxlen, const char *src)
{
	// this should match the WinUAE au_copy behavior, where either the
	// entire string is copied (and null-terminated), or the result is
	// an empty string
	if (uae_tcslcpy (dst, src, maxlen) >= maxlen) {
		dst[0] = '\0';
	}
	return dst;
}

char *ua_copy (char *dst, int maxlen, const TCHAR *src)
{
	return au_copy (dst, maxlen, src);
}

TCHAR *my_strdup_ansi (const char *src)
{
	return strdup (src);
}

#define NO_TRANSLATION

TCHAR *au_fs (const char *src) {
#ifdef NO_TRANSLATION
    if (src == NULL) return NULL;
    return strdup(src);
#else
    gsize read, written;
    gchar *result = g_convert(src, -1, "UTF-8",
            "ISO-8859-1", &read, &written, NULL);
    if (result == NULL) {
        write_log("WARNING: au_fs_copy failed to convert string %s", src);
        return strdup("");
    }
    return result;
#endif
}

char *ua_fs (const TCHAR *s, int defchar) {
#ifdef NO_TRANSLATION
    if (s == NULL) return NULL;
    return strdup(s);
#else
    // we convert from fs-uae's internal encoding (UTF-8) to latin-1 here,
    // so file names can be read properly in the amiga

    char def[] = "?";
    if (defchar < 128) {
        def[0] = defchar;
    }

    gsize read, written;
    gchar *result = g_convert_with_fallback(s, -1, "ISO-8859-1",
            "UTF-8", def, &read, &written, NULL);
    if (result == NULL) {
        write_log("WARNING: ua_fs failed to convert string %s", s);
        return strdup("");
    }

    // duplicate with libc malloc
    char *result_malloced = strdup(result);
    free(result);
    return result_malloced;
#endif
}

TCHAR *au_fs_copy (TCHAR *dst, int maxlen, const char *src) {
#ifdef NO_TRANSLATION
    dst[0] = 0;
    strncpy(dst, src, maxlen);
    return dst;
#else
    gsize read, written;
    gchar *result = g_convert(src, -1, "UTF-8",
            "ISO-8859-1", &read, &written, NULL);
    if (result == NULL) {
        write_log("WARNING: au_fs_copy failed to convert string %s", src);
        dst[0] = '\0';
        return dst;
    }

    strncpy(dst, result, maxlen);
    free(result);
    return dst;
#endif
}

char *ua_fs_copy (char *dst, int maxlen, const TCHAR *src, int defchar) {
#ifdef NO_TRANSLATION
    dst[0] = 0;
    strncpy(dst, src, maxlen);
    return dst;
#else
    char def[] = "?";
    if (defchar < 128) {
        def[0] = defchar;
    }

    gsize read, written;
    gchar *result = g_convert_with_fallback(src, -1, "ISO-8859-1",
            "UTF-8", def, &read, &written, NULL);
    if (result == NULL) {
        write_log("WARNING: ua_fs_copy failed to convert string %s", src);
        dst[0] = '\0';
        return dst;
    }

    strncpy(dst, result, maxlen);
    free(result);
    return dst;
#endif
}
