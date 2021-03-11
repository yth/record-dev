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
#include <fcntl.h> //open
#include <errno.h>


#include "byte_vector.h"
#include "sha1.h"
#include "helper.h" // write_size_t, read_size_t


// Globals
FILE *db_file;
FILE *index_file;
size_t offset;
int count; // Maybe better to make this a double
size_t size;
std::map<std::string, size_t> *gbov_map;


/**
 * This function creates a database for a collection of values.
 * @method open_db
 * @param filename
 * @return file pointer wrapped as a R external pointer
 */
SEXP open_db(SEXP filename) {
  const char* name = CHAR(STRING_ELT(filename, 0));

  /*mode = an octal expression (leading 0) of the unix file mode. See e.g. the chmod manpage.
    0644 means the owner can read+write (4+2=6), the group can read (4), and others can read (4)*/
  // int fd = open(pathname, O_CREAT | O_WRONLY | O_EXCL, S_IRUSR | S_IWUSR, 0644);
  // if (fd < 0) {
  //   /* failure */
  //   if (errno == EEXIST) {
  //     /* the file already existed */
  //     open_db_for_read(pathname);
  //   }
  // } else {
  //   /* now you can use the file */
  //   open_db_for_write(pathname);
  // }
	FILE *db = fopen(name, "w+");
	if (db == NULL) {
		Rf_error("Could not start the database.");
	}
	db_file = db;

	index_file = NULL;

	offset = 0;
	count = 0;
	size = 0;

	gbov_map = new std::map<std::string, size_t>;

	return R_NilValue;
}


/**
 * Load the gbov. (Must be called before load_indices)
 * @method load_gbov
 * @return R_NilValue on success throw and error otherwise
 */
SEXP load_gbov(SEXP gbov) {
	const char* name = CHAR(STRING_ELT(gbov, 0));
	FILE *db = fopen(name, "r+");
	if (db == NULL) {
		Rf_error("Could not load the database.");
	}

	db_file = db;
	fseek(db_file, 0, SEEK_END);

	index_file = NULL;

	offset = 0;
	count = 0;
	size = 0;

	gbov_map = new std::map<std::string, size_t>;

	return R_NilValue;
}


/**
 * Load the indices associated with the gbov.
 * @method load_indices
 * @return R_NilValue on success throw and error otherwise
 */
SEXP load_indices(SEXP indices) {
	const char* name = CHAR(STRING_ELT(indices, 0));
	FILE *idx = fopen(name, "r+");
	if (idx == NULL) {
		Rf_error("Could not load the database.");
	}

	index_file = idx;

	// TODO: Check the return value instead of silence
	int i = fread(&offset, sizeof(size_t), 1, index_file);
	i = fread(&size, sizeof(size_t), 1, index_file);
	i = fread(&count, sizeof(int), 1, index_file);

	for (size_t i = 0; i < size; ++i) {
		size_t start = 0;
		char hash[40];
		i = fread(hash, 1, 40, index_file);
		i = fread(&start, sizeof(size_t), 1, index_file);
		(*gbov_map)[std::string(hash, 40)] = start;
	}

	return R_NilValue;
}


/**
 * This function closes a database.
 * @method record_close
 * @param  file_ptr     wrapped FILE pointer
 */
SEXP close_db() {
	fflush(db_file);
	if (fclose(db_file)) {
		Rf_error("Could not close the database.");
	}
	db_file = NULL;

	if (index_file) {
		// TODO: Think about ways to reuse rather than overwrite
		// TODO: Error check
		fseek(index_file, 0, SEEK_SET);
		// TODO: Create a write_n function or check success here
		fwrite(&offset, sizeof(size_t), 1, index_file);
		fwrite(&size, sizeof(size_t), 1, index_file);
		fwrite(&count, sizeof(int), 1, index_file);

		std::map<std::string, size_t>::iterator it;
		for(it = gbov_map->begin(); it != gbov_map->end(); it++) {
			fwrite(it->first.c_str(), 1, 40, index_file);
			fwrite(&(it->second), sizeof(size_t), 1, index_file);
		}
		fflush(index_file);
		fclose(index_file);
	}

	delete gbov_map;

	return R_NilValue;
}


/**
 * Create the gbov. (Must be called before create_indices)
 * @method load_gbov
 * @return R_NilValue on success throw and error otherwise
 */

SEXP create_gbov(SEXP gbov) {
	return load_gbov(gbov);
}


/**
 * Load the indices associated with the gbov.
 * @method load_indices
 * @return R_NilValue on success throw and error otherwise
 */
SEXP create_indices(SEXP indices) {
	const char* name = CHAR(STRING_ELT(indices, 0));
	FILE *idx = fopen(name, "r+");
	if (idx == NULL) {
		Rf_error("Could not load the database.");
	}

	index_file = idx;

	return R_NilValue;
}


/**
 * This functions directly adds an R value to the specified storage.
 * @method add_val
 * @param  val arbitrary R value
 * @return val same as input val
 */
SEXP add_val(SEXP val) {
	count += 1;

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
	char hash[41] = { 0 }; // TODO: Do not actually need 41 bytes here

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
	if (gbov_map->begin() != gbov_map->end()) {
		std::map<std::string, size_t>::iterator it;
		it = gbov_map->find(std::string(hash, 40));
		if (it != gbov_map->end()) {
			have_seen = 1;
		}
	}

	if (!have_seen) {

		(*gbov_map)[std::string(hash, 40)] = offset;

		// Write the blob's size
		write_size_t(db_file, vector->size);

		// TODO: Make sure fwrite writes enough bytes every time
		if (vector->size != fwrite(vector->buf, 1, vector->size, db_file)) {
			// TODO: Consider reuse;
			free_vector(vector);
			Rf_error("Could not write out.");
		}

		// Acting as a NULL
		// Will be used to make the file act as if it had a linked list for duplicates
		write_size_t(db_file, 0);

		// Modify offset here
		// TODO: Check for overflow
		offset += vector->size + sizeof(size_t) + sizeof(size_t);

		size++;

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
	it = gbov_map->find(std::string(hash, 40));

	int found = 0;
	if (it != gbov_map->end()) {
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

SEXP size_db() {
	SEXP ret = PROTECT(allocVector(INTSXP, 1));
	INTEGER(ret)[0] = size;
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
	it = gbov_map->begin();
	std::advance(it, random_index);

	// Get the specified value
	size_t* size = (size_t*) read_n(db_file, offset, it->second, sizeof(size_t));
	unsigned char* serialized_value = (unsigned char*) read_n(db_file, offset, it->second + sizeof(size_t), *size);

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
