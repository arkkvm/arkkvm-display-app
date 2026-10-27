#ifndef FILE_LOG_H
#define FILE_LOG_H

#ifdef __cplusplus
extern "C" {
#endif

void file_log_init(void);
void file_log_printf(const char * fmt, ...) __attribute__((format(printf, 1, 2)));

#ifdef __cplusplus
}
#endif

#endif
