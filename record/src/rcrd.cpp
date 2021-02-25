#include "rcrd.h"


#include "R_ext/Error.h"
#include "R_ext/Print.h"
#include <R_ext/RS.h>


#include <string> // std::string, strlen
#include <map> // std::map
#include <stdio.h> // FILE, fopen, close, fwrite
#include <stdarg.h> // testing


#include "byte_vector.h"
#include "sha1.h"


// Globals
FILE *file;
uint32_t offset;
std::map<std::string, uint32_t> *gbov;


/**
 * This function creates a database for a collection of values.
 * @method record_init
 * @param filename
 * @return file pointer wrapped as a R external pointer
 */
SEXP open_db(SEXP filename) {
	const char* name = CHAR(STRING_ELT(filename, 0));

	/* FILE *db = fopen(name, "w+"); */
	FILE *db = fopen(name, "w+");
	if (db == NULL) {
		Rf_error("Could not start the database.");
	}
	file = db;

	offset = 0;

	gbov = new std::map<std::string, uint32_t>;

	return R_NilValue;
}


/**
 * This function closes a database.
 * @method record_close
 * @param  file_ptr     wrapped FILE pointer
 */
SEXP close_db() {
	if (fclose(file)) {
		Rf_error("Could not close the database.");
	}
	file = NULL;

	delete gbov;

	return R_NilValue;
}


/**
 * This functions directly adds an R value to the specified storage.
 * @method r2cd
 * @param  r_string_object SEXP that contains at least a single string
 * @param  file_ptr        SEXP that contains a FILE ptr pointing to an opened
 *                         filed
 * @return                 r_string_object on success
 */
SEXP add_value(SEXP val) {
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

	(*gbov)[hash] = offset;

	// TODO: Make sure fwrite writes enough bytes every time
	if (vector->size != fwrite(vector->buf, 1, vector->size, file)) {
		// TODO: Consider reuse;
		free_vector(vector);
		Rf_error("Could not write out.");
	}

	// Modify offset here7777777

	return val;
}

SEXP has_value(SEXP val) {
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

	std::map<std::string, uint32_t>::iterator it;
	it = gbov->find(hash);

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
