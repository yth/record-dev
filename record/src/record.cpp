#include "record.h"

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
#include "helper.h" // write_size_t, read_size_t, read_n


// Globals
FILE *db_file = NULL;
FILE *index_file = NULL;
size_t offset = 0;
// TODO: Give count and size better names
size_t count = 0; // TODO: Consider: Maybe better to make this a double
size_t size = 0; // TODO: Consider: Maybe better to make this a double
std::map<std::string, size_t> *gbov_map = NULL;


// Small scalar int storage
int INT_STORE_MAX = 5000;
int INT_STORE_MIN = -5000;
FILE *int_file = NULL;
size_t i_size = 0;               // number of unique ints encountered
size_t int_db[10001] = { 0 };    // hard wired to accommodate -5000 to 5000

// Reusable buffer for stream
byte_vector_t vector = NULL;


/**
 * Load the indices associated with the gbov.
 * @method load_indices
 * @return R_NilValue on success throw and error otherwise
 */
SEXP create_indices(SEXP indices) {
	index_file = open_file(indices);

	offset = 0;
	size = 0;
	count = 0;

	gbov_map = new std::map<std::string, size_t>;
	vector = make_vector(1 << 30);
	return R_NilValue;
}

/**
 * Load the indices associated with the gbov.
 * @method load_indices
 * @return R_NilValue on success throw and error otherwise
 */
SEXP load_indices(SEXP indices) {
	create_indices(indices);

	// TODO: Wrap in helper function to make code shorter and easier to read
	// TODO: Check the return value instead of silence
	int _ = fread(&offset, sizeof(size_t), 1, index_file);
	_ = fread(&size, sizeof(size_t), 1, index_file);
	_ = fread(&count, sizeof(int), 1, index_file);

	for (size_t i = 0; i < size; ++i) {
		size_t start = 0;
		char hash[20];
		_ = fread(hash, 1, 20, index_file);
		_ = fread(&start, sizeof(size_t), 1, index_file);
		(*gbov_map)[std::string(hash, 20)] = start;
	}

	return R_NilValue;
}


/**
 * Create the common ints storage
 * @method create_ints
 * @param  ints        file name
 * @return             R_NilValue on succcecss
 */
SEXP create_ints(SEXP ints) {
	int_file = open_file(ints);

	i_size = 0;
	for (int i = 0; i < 10001; ++i) {
		int_db[i] = 0;
	}

	return R_NilValue;
}


/**
 * Loads ints.bin in the database
 * @method loads_ints
 * @return R_NilValue on success throw and error otherwise
 */
SEXP load_ints(SEXP ints) {
	create_ints(ints);

	// TODO: Use better error checking methods
	// TODO: Check the return value instead of silence
	if (fread(&i_size, sizeof(size_t), 1, int_file) < 0) {
		perror("Could not read all data from file.");
		exit(1);
	}

	for (size_t i = 0; i < 10001; ++i) {
		// TODO: Check the return value instead of silence
		if (fread(int_db + i, sizeof(size_t), 1, int_file) < 0) {
			perror("Could not read all data from file.");
			exit(1);
		}
	}

	return R_NilValue;
}


/**
 * Load the gbov. (Must be called before load_indices)
 * @method load_gbov
 * @return R_NilValue on success throw and error otherwise
 */
SEXP load_gbov(SEXP gbov) {
	db_file = open_file(gbov);
	fseek(db_file, 0, SEEK_END);
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
 * This function closes a database.
 * @method record_close
 * @param  file_ptr     wrapped FILE pointer
 */
SEXP close_db() {
	if (db_file) {
		fflush(db_file);
		if (fclose(db_file)) {
			Rf_error("Could not close the database.");
		}
		db_file = NULL;
	}

	if (index_file) {
		// TODO: Think about ways to reuse rather than overwrite
		// TODO: Error check
		fseek(index_file, 0, SEEK_SET);
		// TODO: Create a write_n function or check success here
		fwrite(&offset, sizeof(size_t), 1, index_file);
		offset = 0;

		fwrite(&size, sizeof(size_t), 1, index_file);
		size = 0;

		fwrite(&count, sizeof(int), 1, index_file);
		count = 0;

		std::map<std::string, size_t>::iterator it;
		for(it = gbov_map->begin(); it != gbov_map->end(); it++) {
			fwrite(it->first.c_str(), 1, 20, index_file);
			fwrite(&(it->second), sizeof(size_t), 1, index_file);
		}
		fflush(index_file);
		fclose(index_file);
		index_file = NULL;
	}

	if (int_file) {
		fseek(int_file, 0, SEEK_SET);
		write_size_t(int_file, i_size);
		for(int i = 0; i < 10001; ++i)
		{
			write_size_t(int_file, int_db[i]);
		}
		fflush(int_file);
		fclose(int_file);
		int_file = NULL;

		i_size = 0;
		size_t int_db[10001] = { 0 };
	}

	if (gbov_map) {
		delete gbov_map;
		gbov_map = NULL;
	}

	if (vector) {
		free_vector(vector);
		vector = NULL;
	}

	return R_NilValue;
}


/**
 * Adds an R scalar integer value to a separate int database.
 * @method add_int
 * @param  val strictly an integer value from -5000 to 5000
 * @return val
 */
SEXP add_int(SEXP val) {
	int int_val = Rf_asInteger(val) + 5000; // int_db[0] represents -5000L
	if(int_db[int_val] == 0) {
		int_db[int_val] += 1;
		i_size += 1;
		size += 1;
		return val;
	} else {
		int_db[int_val] += 1;

		return R_NilValue;
	}
}


/**
 * This functions directly adds an R value to the specified storage.
 * @method add_val
 * @param  val arbitrary R value
 * @return val same as input val
 */
SEXP add_val(SEXP val) {
	count += 1;

	if (IS_SIMPLE_SCALAR(val, INTSXP)) {
		if (asInteger(val) <= INT_STORE_MAX && asInteger(val) >= INT_STORE_MIN) {
			return add_int(val);
		}
	}

	// Serialize val
	free_content(vector);
	struct R_outpstream_st out;
	R_outpstream_t stream = &out;
	R_InitOutPStream(stream, (R_pstream_data_t) vector,
						R_pstream_binary_format, 3,
						append_byte, append_buf,
						NULL, R_NilValue);
	R_Serialize(val, stream);

	// Get the sha1 hash of the serialized value
	sha1_context ctx;
	unsigned char sha1sum[20];
	sha1_starts(&ctx);
	sha1_update(&ctx, (uint8 *)vector->buf, vector->size);
	sha1_finish(&ctx, sha1sum);

	std::string key((char *) sha1sum, 20);
	std::map<std::string, size_t>::iterator it = gbov_map->find(key);
	if (it == gbov_map->end()) {
		(*gbov_map)[key] = offset;

		// Write the blob
		write_size_t(db_file, vector->size); // serialized data size
		if (vector->size != fwrite(vector->buf, 1, vector->size, db_file)) {
			Rf_error("Could not write out.");
			return R_NilValue;
		}
		write_size_t(db_file, 0); // Act as a NULL and potential ptr in future

		// Modify offset here
		offset += vector->size + sizeof(size_t) + sizeof(size_t);

		size++;

		return val;
	}

	return R_NilValue;
}

// TODO: Let people pass in ints instead
SEXP have_seen_int(SEXP val) {
	int index = Rf_asInteger(val) + 5000;
	int found = 0;
	if (int_db[index]) {
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

SEXP have_seen(SEXP val) {
	if (IS_SIMPLE_SCALAR(val, INTSXP)) {
		if (asInteger(val) <= INT_STORE_MAX && asInteger(val) >= INT_STORE_MIN) {
			return have_seen_int(val);
		}
	}

	// Serialize value
	free_content(vector);
	struct R_outpstream_st out;
	R_outpstream_t stream = &out;
	R_InitOutPStream(stream, (R_pstream_data_t) vector,
						R_pstream_binary_format, 3,
						append_byte, append_buf,
						NULL, R_NilValue);
	R_Serialize(val, stream);

	// Get the sha1 hash of the serialized value
	sha1_context ctx;
	unsigned char sha1sum[20];
	sha1_starts(&ctx);
	sha1_update(&ctx, (uint8 *)vector->buf, vector->size);
	sha1_finish(&ctx, sha1sum);

	std::string key((char *) sha1sum, 20);
	std::map<std::string, size_t>::iterator it = gbov_map->find(key);

	SEXP res;
	PROTECT(res = allocVector(LGLSXP, 1));
	int *res_ptr = LOGICAL(res);
	if (it == gbov_map->end()) {
		res_ptr[0] = 0;
	} else {
		res_ptr[0] = 1;
	}
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

SEXP size_ints() {
	SEXP ret = PROTECT(allocVector(INTSXP, 1));
	INTEGER(ret)[0] = i_size;
	UNPROTECT(1);

	return ret;
}

SEXP read_vals(SEXP from, SEXP to) {
  //TODO
	return R_NilValue;
}

SEXP get_random_val() {
	// Specify a random value
	// TODO: Find a bette rand() function that can return any size_t value
	int random_index = rand() % size;
	if (random_index < i_size) {
		if (i_size < 10001) {
			int values_passed = 0;
			for (int i = 0; i < 10001; ++i) {
				if (int_db[i] > 0) {
					++values_passed;
				}

				if (values_passed == random_index + 1) {
					SEXP res;
					R_xlen_t n = 1;
					PROTECT(res = allocVector(INTSXP, n));
					int *res_ptr = INTEGER(res);
					res_ptr[0] = i - 5000;
					UNPROTECT(1);
					return res;
				}
			}
		} else {
			SEXP res;
			R_xlen_t n = 1;
			PROTECT(res = allocVector(INTSXP, n));
			int *res_ptr = INTEGER(res);
			res_ptr[0] = random_index - 5000;
			UNPROTECT(1);
			return res;
		}
	}

	std::map<std::string, size_t>::iterator it;
	it = gbov_map->begin();
	std::advance(it, random_index - i_size);

	// Get the specified value
	size_t* obj_size = (size_t*) read_n(db_file, offset, it->second, sizeof(size_t));
	unsigned char* serialized_value = (unsigned char*) read_n(db_file, offset, it->second + sizeof(size_t), *obj_size);

	// Deserialize the specified value

	// Create an R_inpstream_t of the serialized value
	struct R_inpstream_st in;
	R_inpstream_t stream = &in;

	// TODO: Consider reuse
	byte_vector_t vector = make_vector(0);
	vector->capacity = *obj_size;
	vector->buf = serialized_value;

	R_InitInPStream(stream, (R_pstream_data_t) vector,
						R_pstream_binary_format,
						get_byte, get_buf,
						NULL, R_NilValue);

	// Call R_Unserialize(stream) and catch the return value
	SEXP res = R_Unserialize(stream);

	// Clean Up
	free(obj_size);
	free(serialized_value);

	// Return the deserialized value
	return res;
}
