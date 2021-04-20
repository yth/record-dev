#include "byte_vector.h"


// TODO: Use malloc instead since nothing is returned to R interpreter directly
#include <R_ext/RS.h> // R's allocations


#include <stdlib.h> //size_t

// Useful session counters
extern size_t bytes_appended_session;
extern size_t bytes_gotten_session;

// Useful process counters
extern size_t bytes_appended_process;
extern size_t bytes_gotten_process;

// Useful lifetime counters // Not implemented yet
extern size_t bytes_appended;
extern size_t bytes_gotten;

byte_vector_t make_vector(size_t capacity) {
	byte_vector_t v = (byte_vector_t) malloc(sizeof (struct byte_vector_st));
	if (v == NULL) {
		fprintf(stderr, "Attempt to malloc %lu bytes: ",
				sizeof(struct byte_vector_st));
		perror("make_vector first allocation");
		abort();
	}

	unsigned char *ptr = (unsigned char*) malloc(capacity);
	if (ptr == NULL) {
		fprintf(stderr, "Attempt to malloc %lu bytes: ", capacity);
		perror("make_vector second allocation");
		free(v);
		abort();
	}

	v->capacity = capacity;
	v->size = 0;
	v->buf = ptr;

	return v;
}

// Not used
void free_vector(byte_vector_t vector) {
	if (vector) {
		free(vector->buf);
		free(vector);
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

	bytes_appended_session += 1;
	bytes_appended_process += 1;
	bytes_appended += 1;
}

// Required for make use of a R_outpstream_t
void append_buf(R_outpstream_t stream, void *buf, int length) {
	unsigned char* cbuf = (unsigned char*) buf;
	byte_vector_t vector = (byte_vector_t) stream->data;
	if (vector->size + length >= vector->capacity) {
		fprintf(stderr, "append_buf attempts to overflow buffer\n");
		abort();
	}

	memcpy(vector->buf + vector->size, cbuf, length);
	vector->size += length;

	bytes_appended_session += length;
	bytes_appended_process += length;
	bytes_appended += length;
}

// Required for make use of a R_inpstream_t
int get_byte(R_inpstream_t stream) {
	byte_vector_t vector = (byte_vector_t) stream->data;
	if (vector->size >= vector->capacity) {
		fprintf(stderr, "get_byte attempts to read beyond buffer\n");
		abort();
	} else {
		bytes_gotten_session += 1;
		bytes_gotten_process += 1;
		bytes_gotten += 1;

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

		bytes_gotten_session += copy_length;
		bytes_gotten_process += copy_length;
		bytes_gotten += copy_length;
	} else {
		memcpy(buf, vector->buf + vector->size, length);
		vector->size += length;

		bytes_gotten_session += length;
		bytes_gotten_process += length;
		bytes_gotten += length;
	}
}
