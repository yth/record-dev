#include "helper.h"

#include <stdlib.h> // size_t
#include <map> // map

#include "byte_vector.h" // byte_vector_t
#include "sha1.h" // sha1_context ... etc

#ifdef __cplusplus
extern "C" {
#endif

// Useful session counters
extern size_t bytes_read_session;
extern size_t bytes_written_session;

extern size_t bytes_serialized_session;
extern size_t bytes_unserialized_session;

// Useful lifetime counters
extern size_t bytes_read;
extern size_t bytes_written;

extern size_t bytes_serialized;
extern size_t bytes_unserialized;

// Useful lifetime generic store value counters
extern size_t i_count;
extern size_t d_count;
extern size_t r_count;
extern size_t s_count;

FILE *open_file(SEXP filename) {
	const char* cfilename = CHAR(STRING_ELT(filename, 0));
	FILE *fp = fopen(cfilename, "r+");
	if (fp == NULL) {
		Rf_error("%s does not exist", cfilename);
	}
	return fp;
}

// Help read n bytes into buffer and check error messages
// Doesn't restore file offset
void read_n(FILE* file, void *buf, size_t n) {
	if (!(fread(buf, n, 1, file))) {
		perror("read_n");
		abort();
	}

	bytes_read_session += n;
	bytes_read += n;
}

// Help write n bytes into file and check error messages
// Doesn't restore file offset
void write_n(FILE* file, void *buf, size_t n) {
	if (!(fwrite(buf, n, 1, file))) {
		perror("write_n");
		abort();
	}

	bytes_written_session += n;
	bytes_written += n;
}

// Serialize val and store the result in vector
void serialize_val(byte_vector_t vector, SEXP val) {
	free_content(vector);
	struct R_outpstream_st out;
	R_outpstream_t stream = &out;
	R_InitOutPStream(stream, (R_pstream_data_t) vector,
						R_pstream_binary_format, 3,
						append_byte, append_buf,
						NULL, R_NilValue);
	R_Serialize(val, stream);

	bytes_serialized_session += vector->size;
	bytes_serialized += vector->size;
}

// Unserialize val stored in vector
SEXP unserialize_val(byte_vector_t vector) {
	struct R_inpstream_st in;
	R_inpstream_t stream = &in;

	R_InitInPStream(stream, (R_pstream_data_t) vector,
						R_pstream_binary_format,
						get_byte, get_buf,
						NULL, R_NilValue);

	bytes_unserialized_session += vector->capacity;
	bytes_unserialized += vector->capacity;

	return R_Unserialize(stream);
}

// Help wrap fclose and intended file pointer to null
void close_file(FILE **fpp) {
	// Add 1 byte for safety
	write_n(*fpp, (void *) "\n", 1);

	fflush(*fpp);
	if (fclose(*fpp)) {
		perror("close_file");
	}

	*fpp = NULL;
}

// Track how many values of a particular type was recorded in the generic store
void track_type(SEXP val) {
	switch(TYPEOF(val)) {
		case INTSXP: i_count++; break;
		case REALSXP: d_count++; break;
		case STRSXP: s_count++; break;
		case RAWSXP: r_count++; break;
	}
	return;
}

#ifdef __cplusplus
} // extern "C"
#endif
