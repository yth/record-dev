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

// Reusable buffer for everything
byte_vector_t vector = NULL;


// Report Related
FILE *stats_file = NULL;

// Useful session counters
size_t bytes_read_session = 0;
size_t bytes_written_session = 0;

size_t bytes_serialized_session = 0;
size_t bytes_unserialized_session = 0;

size_t bytes_appended_session = 0;
size_t bytes_gotten_session = 0;

// Useful process counters
size_t bytes_read_process = 0;
size_t bytes_written_process = 0;

size_t bytes_serialized_process = 0;
size_t bytes_unserialized_process = 0;

size_t bytes_appended_process = 0;
size_t bytes_gotten_process = 0;

// Useful lifetime counters // Not implemented yet
size_t bytes_read = 0;
size_t bytes_written = 0;

size_t bytes_serialized = 0;
size_t bytes_unserialized = 0;

size_t bytes_appended = 0;
size_t bytes_gotten = 0;

/**
 * Create stats.bin in the database
 * @method create_stats
 * @return R_NilValue on success, throw and error otherwise
 */
SEXP create_stats(SEXP stats) {
	stats_file = open_file(stats);

	bytes_read_session = 0;
	bytes_written_session = 0;

	bytes_serialized_session = 0;
	bytes_unserialized_session = 0;

	bytes_appended_session = 0;
	bytes_gotten_session = 0;

	return R_NilValue;
}

/**
 * Loads stats.bin in the database
 * @method load_stats
 * @return R_NilValue on success, throw and error otherwise
 */
SEXP load_stats(SEXP stats) {
	create_stats(stats);

	read_n(stats_file, &bytes_read, sizeof(size_t));
	read_n(stats_file, &bytes_written, sizeof(size_t));

	read_n(stats_file, &bytes_serialized, sizeof(size_t));
	read_n(stats_file, &bytes_unserialized, sizeof(size_t));

	read_n(stats_file, &bytes_appended, sizeof(size_t));
	read_n(stats_file, &bytes_gotten, sizeof(size_t));

	return R_NilValue;
}

SEXP close_stats() {
	if (stats_file) {
		write_n(stats_file, &bytes_read, sizeof(size_t));
		bytes_read = 0;

		write_n(stats_file, &bytes_written, sizeof(size_t));
		bytes_written = 0;

		write_n(stats_file, &bytes_serialized, sizeof(size_t));
		bytes_serialized = 0;

		write_n(stats_file, &bytes_unserialized, sizeof(size_t));
		bytes_unserialized = 0;

		write_n(stats_file, &bytes_appended, sizeof(size_t));
		bytes_appended = 0;

		write_n(stats_file, &bytes_gotten, sizeof(size_t));
		bytes_gotten = 0;

		write_n(stats_file, (void *) "\n", 1);
		fflush(stats_file);

		if (fclose(stats_file)) {
			Rf_error("Could not close the stats file.");
		}
		stats_file = NULL;
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

	read_n(index_file, &offset, sizeof(size_t));
	read_n(index_file, &size, sizeof(size_t));
	read_n(index_file, &count, sizeof(int));

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

	read_n(int_file, &i_size, sizeof(size_t));

	for (size_t i = 0; i < 10001; ++i) {
		read_n(int_file, int_db + i, sizeof(size_t));
	}

	return R_NilValue;
}


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
 * This function closes a database.
 * @method record_close
 * @param  file_ptr     wrapped FILE pointer
 */
SEXP close_db() {
	if (db_file) {
		write_n(db_file, (void *) "\n", 1);
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
		write_n(index_file, &offset, sizeof(size_t));
		offset = 0;

		write_n(index_file, &size, sizeof(size_t));
		size = 0;

		write_n(index_file, &count, sizeof(int));
		count = 0;

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

	if (int_file) {
		fseek(int_file, 0, SEEK_SET);
		write_n(int_file, &i_size, sizeof(size_t));
		for(int i = 0; i < 10001; ++i) {
			write_n(int_file, &(int_db[i]), sizeof(size_t));
		}
		write_n(int_file, (void *) "\n", 1);
		fflush(int_file);
		fclose(int_file);
		int_file = NULL;
		i_size = 0;
		int_db[10001] = { 0 };
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

SEXP report() {
	// Session
	printf("Session Information:\n");
	printf("  bytes read: %lu\n", bytes_read_session);
	printf("  bytes written: %lu\n", bytes_written_session);
	printf("  bytes serialized: %lu\n", bytes_serialized_session);
	printf("  bytes unserialized: %lu\n", bytes_unserialized_session);
	printf("  bytes appended: %lu\n", bytes_appended_session);
	printf("  bytes gotten: %lu\n", bytes_gotten_session);
	printf("\n");

	// Process
	printf("Process Information (APPROXIMATE ONLY):\n");
	printf("  bytes read: %lu\n", bytes_read_process);
	printf("  bytes written: %lu\n", bytes_written_process);
	printf("  bytes serialized: %lu\n", bytes_serialized_process);
	printf("  bytes unserialized: %lu\n", bytes_unserialized_process);
	printf("  bytes appended: %lu\n", bytes_appended_process);
	printf("  bytes gotten: %lu\n", bytes_gotten_process);
	printf("\n");

	// Lifetime // Not implemented; just placeholder
	printf("Database Lifetime Information (APPROXIMATE ONLY):\n");
	printf("  bytes read: %lu\n", bytes_read);
	printf("  bytes written: %lu\n", bytes_written);
	printf("  bytes serialized: %lu\n", bytes_serialized);
	printf("  bytes unserialized: %lu\n", bytes_unserialized);
	printf("  bytes appended: %lu\n", bytes_appended);
	printf("  bytes gotten: %lu\n", bytes_gotten);

	return R_NilValue;
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
