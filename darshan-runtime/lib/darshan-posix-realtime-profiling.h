#ifdef HAVE_CONFIG_H
# include <darshan-runtime-config.h>
#endif

#include <stdio.h>
#include <limits.h>
#include <unistd.h>
#include <semaphore.h>
#include <stdbool.h>

// TODO:
// Let DBUF_MAX_SIZE be configured by autogen build system
#ifndef DBUF_MAX_SIZE
#define DBUF_MAX_SIZE 256
#endif

// TODO:
// Let CONFIG_PID_INFO_PATH be configured by autogen build system
#ifndef CONFIG_PID_INFO_PATH
#define SHM_NAME "/double_buffer_shm"
#endif

enum posix_target_op_type {
    POSIX_OPEN              = 0,
    POSIX_OPEN64            = 1,
    POSIX___OPEN_2          = 2,
    POSIX_OPENAT            = 3,
    POSIX_OPENAT64          = 4,
    POSIX_READ              = 5,
    POSIX_WRITE             = 6,
    POSIX_PREAD             = 7,
    POSIX_PWRITE            = 8,
    POSIX_PREAD64           = 9,
    POSIX_PWRITE64          = 10,
    POSIX_READV             = 11,
#ifdef HAVE_PREADV
    POSIX_PREADV            = 12,
    POSIX_PREADV64          = 13,
#endif
#ifdef HAVE_PREADV2
    POSIX_PREADV2           = 14,
    POSIX_PREADV64V2        = 15,
#endif
    POSIX_WRITEV            = 16,
#ifdef HAVE_PWRITEV
    POSIX_PWRITEV           = 17,
    POSIX_PWRITEV64         = 18,
#endif
#ifdef HAVE_PWRITEV2
    POSIX_PWRITEV2          = 19,
    POSIX_PWRITEV64V2       = 20,
#endif
    POSIX_CREATE	    = 21,
    POSIX_CREATE64	    = 22,
};

/* A structure storing records for real-time profiling 
/* regarding open, read, write methods. Filename is filled only by open call.
/* pid, fd is given for every open, read, write call. As file descriptor is not
/* unique across multiple processes running simultaneously,
/* use pid, fd altogether to identify the target file of read, write */

struct posix_realtime_record
{
    char path[PATH_MAX];                // path in case op_type is open
    int pid;                            // pid of the current running process
    int fd;                             // file descriptor
    enum posix_target_op_type op_type;  // posix operation type
    ssize_t size;                       // the size of byte read/write. Has valid value only if op_type is read/write
};

struct double_buffer {
    struct posix_realtime_record buffer_1[DBUF_MAX_SIZE];
    struct posix_realtime_record buffer_2[DBUF_MAX_SIZE];
    struct posix_realtime_record* buf_producer;
    struct posix_realtime_record* buf_consumer;
    sem_t sem_producer;
    sem_t sem_consumer;
    size_t idx_producer;
    size_t idx_consumer;
    bool consumed;
};
