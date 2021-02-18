#include <Rinternals.h>

#ifndef RCRD_BYTE_VECTOR_H
#define RCRD_BYTE_VECTOR_H

typedef struct byte_vector_st {
	size_t capacity;
	size_t size;
	unsigned char *buf;
} *byte_vector_t;

byte_vector_t make_vector(size_t capacity);

void free_vector(byte_vector_t vector);

// Required for make use of a R_outpstream_t
void outchar(R_outpstream_t stream, int c);

// Required for make use of a R_outpstream_t
void outbytes(R_outpstream_t stream, void *buf, int length);

#endif // RCRD_BYTE_VECTOR_H
