#include "record.h"


#include "R_ext/Error.h"
#include "R_ext/Print.h"
#include <R_ext/RS.h>


#include <string> // std::string, strlen
#include <map> // std::map
#include <stdio.h> // FILE, fopen, close, fwrite
#include <stdarg.h> // testing
#include <random> // rand
#include <iterator> //advance


#include "byte_vector.h"
#include "sha1.h"
#include "helper.h" // write_size_t, read_size_t


// Globals
FILE *file;
size_t offset;
int count;
std::map<std::string, size_t> *gbov;


/**
 * This function creates a database for a collection of values.
 * @method record_init
 * @param filename
 * @return file pointer wrapped as a R external pointer
 */
SEXP open_db(SEXP filename) {
	const char* name = CHAR(STRING_ELT(filename, 0));

	FILE *db = fopen(name, "w+");
	if (db == NULL) {
		Rf_error("Could not start the database.");
	}
	file = db;

	offset = 0;
	count = 0;

	gbov = new std::map<std::string, size_t>;

	return R_NilValue;
}


/**
 * This function closes a database.
 * @method record_close
 * @param  file_ptr     wrapped FILE pointer
 */
SEXP close_db() {
	fflush(file);
	if (fclose(file)) {
		Rf_error("Could not close the database.");
	}
	file = NULL;

	delete gbov;

	return R_NilValue;
}


/**
 * This functions directly adds an R value to the specified storage.
 * @method add_val
 * @param  val arbitrary R value
 * @return val same as input val
 */
SEXP add_val(SEXP val) {
	struct R_outpstream_st out;
	R_outpstream_t stream = &out;

	byte_vector_t vector = make_vector(100);

	R_InitOutPStream(stream, (R_pstream_data_t) vector,
						R_pstream_binary_format, 3,
						append_byte, append_buf,
						NULL, R_NilValue);

	R_Serialize(val, stream);

	// TODO: Think about reuse
	sha1_context ctx;
	unsigned char sha1sum[20];
	char hash[41] = { 0 };

	sha1_starts(&ctx);
	sha1_update(&ctx, (uint8 *)vector->buf, vector->size);
	sha1_finish( &ctx, sha1sum );

	// TODO: Use the original sha1sum when being readable is no longer wanted
	// Make readable for easier debugging
	for(int i = 0; i < 20; ++i) {
		sprintf(hash + i * 2, "%02x", sha1sum[i] );
	}

	// TODO: Check if we have seen the value before

	int have_seen = 0;
	if (gbov->begin() != gbov->end()) {
		std::map<std::string, size_t>::iterator it;
		it = gbov->find(std::string(hash, 40));
		if (it != gbov->end()) {
			have_seen = 1;
		}
	}

	if (!have_seen) {

		(*gbov)[std::string(hash, 40)] = offset;

		// Write the blob's size
		write_size_t(file, vector->size);

		// TODO: Make sure fwrite writes enough bytes every time
		if (vector->size != fwrite(vector->buf, 1, vector->size, file)) {
			// TODO: Consider reuse;
			free_vector(vector);
			Rf_error("Could not write out.");
		}

		// Acting as a NULL
		// Will be used to make the file act as if it had a linked list for duplicates
		write_size_t(file, 0);

		// Modify offset here
		// TODO: Check for overflow
		offset += vector->size + sizeof(size_t) + sizeof(size_t);
		count += 1;

		return val;
	}

	return R_NilValue;
}


SEXP has_seen(SEXP val) {
	struct R_outpstream_st out;
	R_outpstream_t stream = &out;

	byte_vector_t vector = make_vector(100);

	R_InitOutPStream(stream, (R_pstream_data_t) vector,
						R_pstream_binary_format, 3,
						append_byte, append_buf,
						NULL, R_NilValue);

	R_Serialize(val, stream);

	sha1_context ctx;
	unsigned char sha1sum[20];
	char hash[41] = { 0 };

	sha1_starts(&ctx);
	sha1_update(&ctx, (uint8 *)vector->buf, vector->size);
	sha1_finish( &ctx, sha1sum );

	// TODO: Use the original sha1sum when being readable is no longer wanted
	// Make readable for easier debugging
	for(int i = 0; i < 20; ++i) {
		sprintf(hash + i * 2, "%02x", sha1sum[i] );
	}

	std::map<std::string, size_t>::iterator it;
	it = gbov->find(std::string(hash, 40));

	int found = 0;
	if (it != gbov->end()) {
		found = 1;
	}

	SEXP res;
	R_xlen_t n = 1;
	PROTECT(res = allocVector(LGLSXP, n));
	int *res_ptr = LOGICAL(res);
	res_ptr[0] = found;
	UNPROTECT(1);
	return res;
}

SEXP count_vals() {
	SEXP ret = PROTECT(allocVector(INTSXP, 1));
	INTEGER(ret)[0] = count;
	UNPROTECT(1);

	return ret;
}

SEXP read_vals(SEXP from, SEXP to) {
  //TODO
	return R_NilValue;
}

SEXP get_random_val() {
	// Specify a random value
	int random_index = rand() % count;
	std::map<std::string, size_t>::iterator it;
	it = gbov->begin();
	std::advance(it, random_index);

	// Get the specified value
	size_t* size = (size_t*) read_n(file, offset, it->second, sizeof(size_t));
	unsigned char* serialized_value = (unsigned char*) read_n(file, offset, it->second + sizeof(size_t), *size);

	// Deserialize the specified value

	// Create an R_inpstream_t of the serialized value
	struct R_inpstream_st in;
	R_inpstream_t stream = &in;

	// TODO: Consider reuse
	byte_vector_t vector = make_vector(0);
	vector->capacity = *size;
	vector->buf = serialized_value;

	R_InitInPStream(stream, (R_pstream_data_t) vector,
						R_pstream_binary_format,
						get_byte, get_buf,
						NULL, R_NilValue);

	// Call R_Unserialize(stream) and catch the return value
	SEXP res = R_Unserialize(stream);

	// Clean Up
	free(size);
	free(serialized_value);

	// Return the deserialized value
	return res;
}
