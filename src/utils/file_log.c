#include "file_log.h"

#include <errno.h>
#include <pthread.h>
#include <stdarg.h>
#include <stdio.h>
#include <sys/stat.h>
#include <time.h>
#include <unistd.h>

#define LOG_DIR "/userdata/arkkvm/logs"
#define LOG_FILE LOG_DIR "/arkkvm_ui.log"
#define LOG_FILE_ROTATED LOG_DIR "/arkkvm_ui.log.1"
/* Active + .1 = 2 files total; each capped at 20 MiB. */
#define MAX_LOG_BYTES (20 * 1024 * 1024)

static pthread_mutex_t g_log_mu = PTHREAD_MUTEX_INITIALIZER;
static FILE * g_log_fp;
static long g_log_size;
static ino_t g_log_ino;

static void ensure_log_dir(void)
{
    if(mkdir("/userdata/arkkvm", 0755) != 0 && errno != EEXIST) {
        return;
    }
    mkdir(LOG_DIR, 0755);
}

static void apply_line_buffering(FILE * fp)
{
    if(fp) {
        setvbuf(fp, NULL, _IOLBF, 0);
    }
}

static long path_file_size(void)
{
    struct stat st;

    if(stat(LOG_FILE, &st) != 0) {
        return -1;
    }
    return (long)st.st_size;
}

static int path_inode(ino_t * out)
{
    struct stat st;

    if(stat(LOG_FILE, &st) != 0) {
        return -1;
    }
    *out = st.st_ino;
    return 0;
}

static void close_locked(void)
{
    if(g_log_fp) {
        fclose(g_log_fp);
        g_log_fp = NULL;
    }
    g_log_size = 0;
    g_log_ino  = 0;
}

static int open_append_locked(void)
{
    ensure_log_dir();
    g_log_fp = fopen(LOG_FILE, "a");
    if(!g_log_fp) {
        g_log_size = 0;
        g_log_ino  = 0;
        return -1;
    }
    apply_line_buffering(g_log_fp);
    g_log_size = path_file_size();
    if(g_log_size < 0) {
        g_log_size = 0;
    }
    if(path_inode(&g_log_ino) != 0) {
        g_log_ino = 0;
    }
    return 0;
}

static void rotate_locked(void)
{
    close_locked();

    unlink(LOG_FILE_ROTATED);
    if(rename(LOG_FILE, LOG_FILE_ROTATED) != 0 && errno != ENOENT) {
        /* Keep writing to the same path if rename fails; truncate to enforce cap. */
        g_log_fp = fopen(LOG_FILE, "w");
        if(g_log_fp) {
            apply_line_buffering(g_log_fp);
            g_log_size = 0;
            if(path_inode(&g_log_ino) != 0) {
                g_log_ino = 0;
            }
        }
        return;
    }

    (void)open_append_locked();
}

/* Re-sync size/inode from disk; reopen if clear_storage_logs replaced the file. */
static int ensure_writable_locked(void)
{
    ino_t ino;
    long sz;

    if(!g_log_fp) {
        return open_append_locked();
    }

    sz = path_file_size();
    if(sz < 0 || path_inode(&ino) != 0) {
        /* Path gone (e.g. clear logs) — reopen a fresh file. */
        close_locked();
        return open_append_locked();
    }

    if(ino != g_log_ino) {
        close_locked();
        return open_append_locked();
    }

    g_log_size = sz;
    if(g_log_size >= MAX_LOG_BYTES) {
        rotate_locked();
        if(!g_log_fp) {
            return -1;
        }
    }
    return 0;
}

static void write_timestamp_locked(void)
{
    time_t now;
    struct tm tm_now;
    int n;

    if(!g_log_fp) {
        return;
    }
    now = time(NULL);
    if(localtime_r(&now, &tm_now) == NULL) {
        return;
    }
    n = fprintf(g_log_fp, "[%04d-%02d-%02d %02d:%02d:%02d] ", tm_now.tm_year + 1900, tm_now.tm_mon + 1,
                tm_now.tm_mday, tm_now.tm_hour, tm_now.tm_min, tm_now.tm_sec);
    if(n > 0) {
        g_log_size += n;
    }
}

void file_log_init(void)
{
    pthread_mutex_lock(&g_log_mu);
    if(g_log_fp) {
        pthread_mutex_unlock(&g_log_mu);
        return;
    }

    if(open_append_locked() == 0 && g_log_size >= MAX_LOG_BYTES) {
        rotate_locked();
    }
    pthread_mutex_unlock(&g_log_mu);
}

void file_log_printf(const char * fmt, ...)
{
    va_list ap;
    int n;

    if(!fmt) {
        return;
    }

    pthread_mutex_lock(&g_log_mu);
    if(ensure_writable_locked() != 0) {
        pthread_mutex_unlock(&g_log_mu);
        return;
    }

    write_timestamp_locked();
    va_start(ap, fmt);
    n = vfprintf(g_log_fp, fmt, ap);
    va_end(ap);
    if(n > 0) {
        g_log_size += n;
    }
    pthread_mutex_unlock(&g_log_mu);
}
