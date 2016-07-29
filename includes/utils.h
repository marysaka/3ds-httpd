#ifndef HTTP_SEVER_UTILS_H
#define HTTP_SEVER_UTILS_H
#include <3ds.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>

void			*memalloc(size_t size);
void			*memdup(void *data, size_t size);
void			memdel(void **data);

int				startWith(char *str, char *start);

__attribute__((format(printf,1,2)))
void failExit(const char *fmt, ...);
#endif
