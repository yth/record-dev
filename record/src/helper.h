#include <cstdio>


#ifndef RCRD_HELPER_H
#define RCRD_HELPER_H


#ifdef __cplusplus
extern "C" {
#endif


// Writes a size_t to file
void write_size_t(FILE* file, size_t val);


// Read a size_t from file starting at the indicated offset
size_t read_size_t(FILE* file, size_t file_offset, size_t value_offset);


#ifdef __cplusplus
} // extern "C"
#endif

#endif // RCRD_HELPER_H