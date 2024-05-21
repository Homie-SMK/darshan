#ifdef HAVE_CONFIG_H
# include <darshan-runtime-config.h>
#endif

#include <stdio.h>
#include <unistd.h>

#ifndef RR_BUF_SIZE
#define RR_BUF_SIZE 128
#endif

#define PRODUCER 0
#define CONSUMER 1

// TODO:
// Let CONFIG_PID_INFO_PATH be configured by autogen build system
#ifndef CONFIG_PID_INFO_PATH
#define SHM_NAME "HEAD"
#endif

enum posix_target_op_type {
    POSIX_OPEN,              
    POSIX_OPEN64,            
    POSIX___OPEN_2,          
    POSIX_OPENAT,            
    POSIX_OPENAT64,          
    POSIX_READ,              
    POSIX_WRITE,
    POSIX_AIO_READ,
    POSIX_AIO_READ64,
    POSIX_AIO_WRITE,
    POSIX_AIO_WRITE64,             
    POSIX_PREAD,            
    POSIX_PWRITE,            
    POSIX_PREAD64,           
    POSIX_PWRITE64,          
    POSIX_READV,             
#ifdef HAVE_PREADV
    POSIX_PREADV,            
    POSIX_PREADV64,          
#endif
#ifdef HAVE_PREADV2
    POSIX_PREADV2,           
    POSIX_PREADV64V2,        
#endif
    POSIX_WRITEV,            
#ifdef HAVE_PWRITEV
    POSIX_PWRITEV,           
    POSIX_PWRITEV64,         
#endif
#ifdef HAVE_PWRITEV2
    POSIX_PWRITEV2,          
    POSIX_PWRITEV64V2,       
#endif
    POSIX_CREATE,	    
    POSIX_CREATE64,
    POSIX_CLOSE,
    POSIX_AIO_RETURN_READ,
    POSIX_AIO_RETURN_READ64,
    POSIX_AIO_RETURN_WRITE,	
    POSIX_AIO_RETURN_WRITE64,    
};

/* A structure storing records for real-time profiling 
   regarding open, read, write methods. Filename is filled only by open call.
   pid, fd is given for every open, read, write call. As file descriptor is not
   unique across multiple processes running simultaneously,
   use pid, fd altogether to identify the target file of read, write */

typedef struct realtime_record
{
    int fd;
    enum posix_target_op_type type;  // posix operation type
    unsigned long long size;            // the size of byte read/write. Has valid value only if op_type is read/write
} realtime_record_t;

typedef struct head {
    struct sm_segment* init_seg;
    struct sm_segment* curr_producer_seg;
    struct sm_segment* curr_consumer_seg;
    pthread_rwlock_t rwlock;
    unsigned int next_seg_idx;
} head_t;

typedef struct sm_segment {
    unsigned int rr_idx;
    realtime_record_t realtime_record_buf[RR_BUF_SIZE];
    unsigned int role:1; 
    pthread_mutex_t l;
    struct sm_segment *next;
} sm_segment_t;

extern head_t *head;
