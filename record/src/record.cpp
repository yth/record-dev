#include "record.h"


#include <string> // std::string, strlen
#include <map> // std::map
#include <stdio.h> // FILE, fopen, close
#include <stdarg.h> // testing
#include <random> // rand
#include <iterator> //advance

#include "byte_vector.h"
#include "sha1.h"
#include "helper.h"

#include "stats_store.h"
#include "simple_ints_store.h"


// Pulled in from stats_store.cpp
extern size_t count;
extern size_t size;
extern size_t offset;
extern size_t i_size;

// Pulled in from simple_ints_store.cpp
extern int INT_STORE_MAX;
extern int INT_STORE_MIN;
extern size_t int_db[10001];

// Globals
FILE *db_file = NULL;
FILE *index_file = NULL;


std::map<std::string, size_t> *gbov_map = NULL;


// Reusable buffer for everything
byte_vector_t vector = NULL;


/**
 * This function sets up the initiatial state of the database.
 * This function must be called first.
 * @method setup
 * @return R_NilValue on succecss
 */
SEXP setup() {
	return R_NilValue;
}


/**
 * This function tears down all traces of the database after running.
 * This function must be called last.
 * @method setup
 * @return R_NilValue on succecss
 */
SEXP teardown() {
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
 * This functions directly adds an R value to the specified storage.
 * @method add_val
 * @param  val arbitrary R value
 * @return val same as input val
 */
SEXP add_val(SEXP val) {
	count += 1;

	if (is_simple_int(val)) {
		return add_int(val);
	}

	serialize_val(vector, val);

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
		size++;

		// Write the blob
		write_n(db_file, &(vector->size), sizeof(size_t));
		write_n(db_file, vector->buf, vector->size);

		// Acting as NULL for linked-list next pointer
		write_n(db_file, &(vector->size), sizeof(size_t));

		// Modify offset here
		offset += vector->size + sizeof(size_t) + sizeof(size_t);

		return val;
	}

	return R_NilValue;
}

SEXP have_seen(SEXP val) {
	if (is_simple_int(val)) {
		return have_seen_int(val);
	}

	serialize_val(vector, val);

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

SEXP read_vals(SEXP from, SEXP to) {
  //TODO
	return R_NilValue;
}

SEXP sample_val() {
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

	size_t obj_size;
	free_content(vector);
	fseek(db_file, it->second, SEEK_SET);
	read_n(db_file, &obj_size, sizeof(size_t));
	read_n(db_file, vector->buf, obj_size);
	fseek(db_file, offset, SEEK_SET);
	vector->capacity = obj_size;

	SEXP res = unserialize_val(vector);

	// Restore vector
	vector->capacity = 1 << 30;

	return res;
}

///////////////////////////////////////////////////////////////////////////////

/**
 * Create the gbov. (Must be called before create_indices)
 * @method load_gbov
 * @return R_NilValue on success throw and error otherwise
 */
SEXP create_gbov(SEXP gbov) {
	db_file = open_file(gbov);
	fseek(db_file, offset, SEEK_SET);
	return R_NilValue;
}


/**
 * Load the gbov. (Must be called before load_indices)
 * @method load_gbov
 * @return R_NilValue on success throw and error otherwise
 */
SEXP load_gbov(SEXP gbov) {
	return create_gbov(gbov);
}


/**
 * This functions writes generic R val store to file and closes the file.
 * @method close_gbov
 * @return R_NilValue on success
 */
SEXP close_gbov() {
	if (db_file) {
		write_n(db_file, (void *) "\n", 1);
		fflush(db_file);
		if (fclose(db_file)) {
			Rf_error("Could not close the database.");
		}
		db_file = NULL;
	}

	return R_NilValue;
}


/**
 * Load the indices associated with the gbov.
 * @method load_indices
 * @return R_NilValue on success throw and error otherwise
 */
SEXP create_indices(SEXP indices) {
	index_file = open_file(indices);

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

	size_t start = 0;
	char hash[20];
	for (size_t i = 0; i < size - i_size; ++i) {
		read_n(index_file, hash, 20);
		read_n(index_file, &start, sizeof(size_t));
		(*gbov_map)[std::string(hash, 20)] = start;
	}

	return R_NilValue;
}


/**
 * This function  writes indices to file and closes the file.
 * @method close_indices
 * @return [description]
 */
SEXP close_indices() {
	if (index_file) {
		// TODO: Think about ways to reuse rather than overwrite each time
		fseek(index_file, 0, SEEK_SET);

		std::map<std::string, size_t>::iterator it;
		for(it = gbov_map->begin(); it != gbov_map->end(); it++) {
			write_n(index_file, (void *) it->first.c_str(), 20);
			write_n(index_file, &(it->second), sizeof(size_t));
		}

		write_n(index_file, (void *) "\n", 1);
		fflush(index_file);
		fclose(index_file);
		index_file = NULL;
	}

	return R_NilValue;
}
