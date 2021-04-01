#include "byte_vector.h"


#include <R_ext/RS.h> // R's allocations


#include <stdlib.h> //size_t


byte_vector_t make_vector(size_t capacity) {
	byte_vector_t v = (byte_vector_t) Calloc(1, struct byte_vector_st);
	if (v == NULL) {
		fprintf(stderr, "Attempt to malloc %lu bytes: ",
				sizeof(struct byte_vector_st));
		perror("make_vector first allocation");
		return NULL;
	}

	unsigned char *ptr = (unsigned char*) Calloc(capacity, char);
	if (ptr == NULL) {
		fprintf(stderr, "Attempt to malloc %lu bytes: ", capacity);
		perror("make_vector second allocation");
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
		Free(vector->buf);
		Free(vector);
	}
}

// For potential reuse
void free_content(byte_vector_t vector) {
	if (vector) {
		Free(vector->buf);
		vector->size = 0;
		vector->capacity = 0;
	}
}

// TODO: Add mechanism to not go over a max size
void grow_vector(byte_vector_t vector) {
	vector->capacity *= 2;
	vector->buf = (unsigned char*)Realloc(vector->buf, vector->capacity, char);
	if (vector->buf) {
		return;
	}
	Rf_error("Could not realloc.");
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

// Required for make use of a R_inpstream_t
int get_byte(R_inpstream_t stream) {
	byte_vector_t vector = (byte_vector_t) stream->data;
	if (vector->size >= vector->capacity) {
		// perror("GET_BYTE: read error byte");
		// TODO: Think about better ways to handle this error check result
		return -1;
	} else {
		return vector->buf[vector->size++];
	}
}

// Required for make use of a R_inpstream_t
void get_buf(R_inpstream_t stream, void *buf, int length) {
	byte_vector_t vector = (byte_vector_t) stream->data;
	if (vector->size + length >= vector->capacity) {
		// perror("GET_BUF: read error buf");
		// TODO: Think about better ways to handle this error check result
		;
	}
	memcpy(buf, vector->buf + vector->size, length);
	vector->size += length;
}
