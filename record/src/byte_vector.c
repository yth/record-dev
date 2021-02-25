#include "byte_vector.h"


#include <stdlib.h> //size_t


byte_vector_t make_vector(size_t capacity) {
	byte_vector_t v = (byte_vector_t) malloc(sizeof(struct byte_vector_st));
	if (v == NULL) {
		perror("Could not malloc.");
		return NULL;
	}

	unsigned char *ptr = (unsigned char*) malloc(capacity);
	if (ptr == NULL) {
		perror("Could not malloc.");
		free(v);
		return NULL;
	}

	v->capacity = capacity;
	v->size = 0;
	v->buf = ptr;

	return v;
}

void free_vector(byte_vector_t vector) {
	if (vector) {
		free(vector->buf);
		free(vector);
	}
}

// For potential reuse
void free_content(byte_vector_t vector) {
	if (vector) {
		free(vector->buf);
		vector->size = 0;
		vector->capacity = 0;
	}
}

// TODO: Add mechanism to not go over a max size
void grow_vector(byte_vector_t vector) {
	unsigned char *ptr;
	ptr = (unsigned char*)realloc(vector->buf, vector->size * 2);
	if (ptr) { // if ptr != vector->buf, do I need to free vector-> buf?
		vector->size *= 2;
		vector->buf = ptr;
	} else {
		perror("Could not realloc.");
	}
}

// Required for make use of a R_outpstream_t
void append_byte(R_outpstream_t stream, int c) {
	byte_vector_t vector = (byte_vector_t) stream->data;
	if (vector->size == vector->capacity) {
		grow_vector(vector);
	}
	vector->buf[vector->size] = c;
	vector->size += 1;
}

// Required for make use of a R_outpstream_t
void append_buf(R_outpstream_t stream, void *buf, int length) {
	unsigned char* cbuf = (unsigned char*) buf;
	for (int i = 0; i < length; ++i) {
		append_byte(stream, cbuf[i]);
	}
}
