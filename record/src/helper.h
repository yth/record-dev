#include <cstdio>

#include <Rinternals.h>

#include "byte_vector.h" // byte_vector_t
#include <map> // map

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

// Serialize val and store the result in vector
void serialize_val(byte_vector_t vector, SEXP val);

// Unserialize val stored in vector
SEXP unserialize_val(byte_vector_t vector);

#ifdef __cplusplus
} // extern "C"
#endif

#endif // RCRD_HELPER_H
