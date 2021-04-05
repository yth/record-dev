#include "byte_vector.h"


// TODO: Use malloc instead since nothing is returned to R interpreter directly
#include <R_ext/RS.h> // R's allocations


#include <stdlib.h> //size_t


byte_vector_t make_vector(size_t capacity) {
	byte_vector_t v = (byte_vector_t) malloc(sizeof (struct byte_vector_st));
	if (v == NULL) {
		fprintf(stderr, "Attempt to malloc %lu bytes: ",
				sizeof(struct byte_vector_st));
		perror("make_vector first allocation");
		return NULL;
	}

	unsigned char *ptr = (unsigned char*) malloc(capacity);
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

// Not used
void free_vector(byte_vector_t vector) {
	if (vector) {
		Free(vector->buf);
		Free(vector);
	}
}

// For potential reuse
void free_content(byte_vector_t vector) {
	if (vector) {
		vector->size = 0;
	}
}

// Not used
void grow_vector(byte_vector_t vector) {
	vector->capacity *= 2;
	vector->buf = (unsigned char *) realloc(vector->buf, vector->capacity);
	if (vector->buf) {
		return;
	}
	fprintf(stderr, "Attempt to malloc %lu bytes: ", vector->capacity);
	perror("grow_vector");
}

// Required for make use of a R_outpstream_t
void append_byte(R_outpstream_t stream, int c) {
	byte_vector_t vector = (byte_vector_t) stream->data;
	if (vector->size == vector->capacity) {
		fprintf(stderr, "append_byte attempts to overflow buffer\n");
		abort();
	}
	vector->buf[vector->size] = c;
	vector->size += 1;
}

// Required for make use of a R_outpstream_t
void append_buf(R_outpstream_t stream, void *buf, int length) {
	unsigned char* cbuf = (unsigned char*) buf;
	byte_vector_t vector = (byte_vector_t) stream->data;
	// while (vector->size + length >= vector->capacity) {
	// 	grow_vector(vector);
	// }
	if (vector->size + length >= vector->capacity) {
		fprintf(stderr, "append_buf attempts to overflow buffer\n");
		abort();
	}

	memcpy(vector->buf + vector->size, cbuf, length);
	vector->size += length;
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
		size_t copy_length = vector->capacity - vector->size;
		memcpy(buf, vector->buf + vector->size, copy_length);
		vector->size += copy_length;
	} else {
		memcpy(buf, vector->buf + vector->size, length);
		vector->size += length;
	}
}
