/*
 *  (C) 2009 by Argonne National Laboratory.
 *      See COPYRIGHT in top-level directory.
 */

#ifndef __DARSHAN_LOG_UTILS_H
#define __DARSHAN_LOG_UTILS_H

#include "darshan-log-format.h"
#include "darshan-posix-log-format.h"
#include "uthash-1.9.2/src/uthash.h"

typedef struct darshan_fd_s* darshan_fd;

struct darshan_record_ref
{
    struct darshan_record rec;
    UT_hash_handle hlink;
};

darshan_fd darshan_log_open(const char *name, const char* mode);
int darshan_log_getheader(darshan_fd file, struct darshan_header *header);
int darshan_log_getjob(darshan_fd file, struct darshan_job *job);
int darshan_log_gethash(darshan_fd file, struct darshan_record_ref **hash);
int darshan_log_getexe(darshan_fd fd, char *buf);
int darshan_log_getmounts(darshan_fd fd, char*** mnt_pts,
    char*** fs_types, int* count);
int darshan_log_getmod(darshan_fd fd, int mod_id, void **mod_buf,
    int *mod_buf_sz);
void darshan_log_close(darshan_fd file);

/* convenience macros for printing out counters */
#define CP_PRINT_HEADER() printf("#<rank>\t<file>\t<counter>\t<value>\t<name suffix>\t<mount pt>\t<fs type>\n")
#define CP_PRINT(__job, __file, __counter, __mnt_pt, __fs_type) do {\
        printf("%" PRId64 "\t%" PRIu64 "\t%s\t%" PRId64 "\t...%s\t%s\t%s\n", \
            (__file)->rank, (__file)->hash, darshan_names[__counter], \
            (__file)->counters[__counter], (__file)->name_suffix, \
            __mnt_pt, __fs_type); \
} while(0)
#define CP_F_PRINT(__job, __file, __counter, __mnt_pt, __fs_type) do {\
        printf("%" PRId64 "\t%" PRIu64 "\t%s\t%f\t...%s\t%s\t%s\n", \
            (__file)->rank, (__file)->hash, darshan_f_names[__counter], \
            (__file)->fcounters[__counter], (__file)->name_suffix, \
            __mnt_pt, __fs_type); \
} while(0)

/* naive byte swap implementation */
#define DARSHAN_BSWAP64(__ptr) do {\
    char __dst_char[8]; \
    char* __src_char = (char*)__ptr; \
    __dst_char[0] = __src_char[7]; \
    __dst_char[1] = __src_char[6]; \
    __dst_char[2] = __src_char[5]; \
    __dst_char[3] = __src_char[4]; \
    __dst_char[4] = __src_char[3]; \
    __dst_char[5] = __src_char[2]; \
    __dst_char[6] = __src_char[1]; \
    __dst_char[7] = __src_char[0]; \
    memcpy(__ptr, __dst_char, 8); \
} while(0)
#define DARSHAN_BSWAP32(__ptr) do {\
    char __dst_char[4]; \
    char* __src_char = (char*)__ptr; \
    __dst_char[0] = __src_char[3]; \
    __dst_char[1] = __src_char[2]; \
    __dst_char[2] = __src_char[1]; \
    __dst_char[3] = __src_char[0]; \
    memcpy(__ptr, __dst_char, 4); \
} while(0)

#endif
