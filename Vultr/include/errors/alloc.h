#pragma once
#include <types/types.h>

void *debug_mem_alloc(uint size, char *file, uint line);
void *debug_mem_realloc(void *pointer, uint size, char *file, uint line);
void debug_mem_free(void *pointer, char *file, uint line);

#ifdef ALLOC_REPORT
#define malloc(n) debug_mem_alloc(n, __FILE__, __LINE__)
#define realloc(n, m) debug_mem_realloc(n, m, __FILE__, __LINE__)
#define free(n) debug_mem_free(n, __FILE__, __LINE__)
#endif
