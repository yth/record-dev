#include <cstdio>

#include <Rinternals.h>

#ifndef RCRD_HELPER_H
#define RCRD_HELPER_H

#ifdef __cplusplus
extern "C" {
#endif

// Help wrap fopen with checks and unpack SEXP filename
FILE *open_file(SEXP filename);

// Help read n bytes into buffer and check error messages
// Doesn't restore file offset
void read_n(FILE* file, void *buf, size_t n);

// Help write n bytes into file and check error messages
// Doesn't restore file offset
void write_n(FILE* file, void *buf, size_t n);

#ifdef __cplusplus
} // extern "C"
#endif

#endif // RCRD_HELPER_H
