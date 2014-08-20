#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "fs/data.h"
#include "fs/base.h"
#include "fs/endian.h"
#include "fs/filesys.h"
#include <limits.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <glib.h>

static FILE* g_dat_file;
static GHashTable *g_dat_table;

#pragma pack(push, 1)

typedef struct end_of_central_directory_record {
    char signature[4];
    uint16_t disk_number;
    uint16_t central_start_disk_number;
    uint16_t num_disk_entries;
    uint16_t num_entries;
    uint32_t central_directory_size;
    uint32_t central_directory_offset;
    uint16_t comment_length;
} end_of_central_directory_record;

typedef struct central_file_header {
    char signature[4];
    uint16_t version_made_by;
    uint16_t version_needed_to_extract;
    uint16_t general_purpose_bit_flag;
    uint16_t compression_method;
    uint16_t last_mod_file_time;
    uint16_t last_mod_file_date;
    uint32_t crc_32;
    uint32_t compressed_size;
    uint32_t uncompressed_size;
    uint16_t file_name_length;
    uint16_t extra_field_length;
    uint16_t file_comment_length;
    uint16_t disk_number_start;
    uint16_t internal_file_attributes;
    uint32_t external_file_attributes;
    uint32_t relative_offset_of_local_header;
} central_file_header;

typedef struct local_file_header {
    char signature[4];
    uint16_t version_needed_to_extract;
    uint16_t general_purpose_bit_flag;
    uint16_t compression_method;
    uint16_t last_mod_file_time;
    uint16_t last_mod_file_date;
    uint32_t crc_32;
    uint32_t compressed_size;
    uint32_t uncompressed_size;
    uint16_t file_name_length;
    uint16_t extra_field_length;
} local_file_header;

#pragma pack(pop)

#define ERROR_EOCDR_SEEK 3
#define ERROR_EOCDR_READ 4
#define ERROR_EOCDR_SIG 5
#define ERROR_EOCDR_ENTRIES 6

#define ERROR_CFH_SEEK 7
#define ERROR_CFH_READ 8
#define ERROR_CFH_SIG 9
#define ERROR_CFH_NAME 10

static int read_zip_entries (FILE *f) {
    if (fseek(f, 0, SEEK_END) != 0) {
        // fseek to end of file failed
        return 2;
    }
    int file_size = ftell(f);

    // we do not support zip file comments here (which are added at the end),
    // and assume that the file ends with the (fixed size) end of
    // central directory.

    int pos = file_size - sizeof(end_of_central_directory_record);
    if (fseek(f, pos, SEEK_SET) != 0) {
        return ERROR_EOCDR_SEEK;
    }

    end_of_central_directory_record eocdr;
    if (fread(&eocdr, sizeof(end_of_central_directory_record), 1, f) != 1) {
        return ERROR_EOCDR_READ;
    }
    if (strncmp(eocdr.signature, "PK\x05\x06", 4) != 0) {
        // no signature found, not a zip file
        return ERROR_EOCDR_SIG;
    }
    if (le16toh(eocdr.num_entries) > 8192) {
        return ERROR_EOCDR_ENTRIES;
    }

    pos = le32toh(eocdr.central_directory_offset);
    central_file_header cfh;
    for (int i = 0; i < le16toh(eocdr.num_entries); i++) {
        // printf("reading zip entry %d from position %d\n", i, pos);
        if (fseek(f, pos, SEEK_SET) != 0) {
            return ERROR_CFH_SEEK;
        }
        if (fread(&cfh, sizeof(central_file_header), 1, f) != 1) {
            return ERROR_CFH_READ;
        }
        if (strncmp(cfh.signature, "PK\x01\x02", 4) != 0) {
            // no signature found, not a central file header
            return ERROR_CFH_SIG;
        }
        uint16_t name_len = le16toh(cfh.file_name_length);
        char *name = malloc(name_len + 1);
        name[name_len] = 0;
        pos += sizeof(central_file_header);
        if (fread(name, name_len, 1, f) != 1) {
            free(name);
            return ERROR_CFH_NAME;
        }
        // printf("%s\n", name);

        g_hash_table_insert(
            g_dat_table, name,
            GUINT_TO_POINTER(le32toh(cfh.relative_offset_of_local_header)));

        pos += name_len + le16toh(cfh.extra_field_length) + \
                le16toh(cfh.file_comment_length);
    }

    /*
    char signature[4];
    if (fread(signature, 4, 1, f) != 1) {
        return 4;
    }
    if (strncmp(signature, "PK\x05\x06", 4) != 0) {
        // no signature found, not a zip file
        return 5;
    }
    */

    return 0;
}

int fs_data_file_content(const char *name, char **data, int *size) {
    // printf("fs_data_file_content %s\n", name);
    if (g_dat_file == NULL) {
        return 1;
    }
    if (g_dat_table == NULL) {
        return 2;
    }
    char *name2 = strdup(name);
    char *c = name2;
    while(*c) {
        if (*c == '\\') {
            *c = '/';
        }
        ++c;
    }
    gpointer ptr = g_hash_table_lookup(g_dat_table, name2);
    free(name2);
    if (ptr == NULL) {
        // printf("did not find entry in dat file\n");
        return 3;
    }
    int pos = GPOINTER_TO_UINT(ptr);
    if (fseek(g_dat_file, pos, SEEK_SET) != 0) {
        printf("could not seek to dat file entry position %d\n", pos);
        return 6;
    }

    local_file_header lfh;
    if (fread(&lfh, sizeof(local_file_header), 1, g_dat_file) != 1) {
        printf("could not read local file header\n");
        return 7;
    }
    if (strncmp(lfh.signature, "PK\x03\x04", 4) != 0) {
        // no signature found, not a local file header
        return 8;
    }
    if (lfh.compression_method != 0) {
        // file is stored compressed
        return 9;
    }
    if (lfh.compressed_size != lfh.uncompressed_size) {
        return 9;
    }

    if (data == NULL) {
        return 10;
    }
    if (size == NULL) {
        return 11;
    }

    *size = le32toh(lfh.uncompressed_size);
    if (*size > 10 * 1024 * 1024) {
        // don't try to allocate very large files
        return 12;
    }

    *data = malloc(*size);

    pos += sizeof(local_file_header) + le16toh(lfh.file_name_length) + \
            le16toh(lfh.extra_field_length);

    if (fseek(g_dat_file, pos, SEEK_SET) != 0) {
        printf("could not seek to dat file data position %d\n", pos);
        return 13;
    }

    if (fread(*data, *size, 1, g_dat_file) != 1) {
        printf("could not read entry data\n");
        return 14;
    }

    // printf("read data into %p[%d]\n", *data, *size);
    return 0;
}

int fs_data_init(const char *app_name, const char *dat_name) {
    printf("fs_data_init %s %s\n", app_name, dat_name);
    char exe_path[PATH_MAX + 1];
    int result = fs_get_application_exe_path(exe_path, PATH_MAX);
    if (result != 1) {
        return 1;
    }

    g_dat_table = g_hash_table_new(g_str_hash, g_str_equal);

    // check for embedded dat file
    printf("checking dat file: %s\n", exe_path);
    g_dat_file = fs_fopen(exe_path, "rb");
    int error = read_zip_entries(g_dat_file);
    if (error == 0) {
        return 0;
    }
    printf("no dat file: %s\n", exe_path);
    fclose(g_dat_file);
    g_dat_file = NULL;

    result = fs_get_application_exe_dir(exe_path, PATH_MAX);
    if (result != 1) {
        return 1;
    }

    // check if dat file is beside the executable
    char *dat_path = fs_path_join(exe_path, dat_name, NULL);
    printf("checking dat file: %s\n", dat_path);
    g_dat_file = fs_fopen(dat_path, "rb");
    free(dat_path);

    if (g_dat_file == NULL) {
        char *dat_path = fs_path_join(
            exe_path, "..", "share", app_name, dat_name, NULL);
        printf("checking dat file: %s\n", dat_path);
        g_dat_file = fs_fopen(dat_path, "rb");
        free(dat_path);
    }

    if (g_dat_file == NULL) {
        error = 10;
    }
    else {
        error = read_zip_entries(g_dat_file);
        if (error == 0) {
/*
            char *data_out;
            int size_out;
            int r = fs_data_file_content("share/fs-uae/menu_fade_item.png", &data_out, &size_out);
            printf("result:%d %p %d\n", r, data_out, size_out);
*/
            return 0;
        }
    }

    return error;
}
