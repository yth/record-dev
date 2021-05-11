#include "generic_store.h"

#include "helper.h"

#include <map> // std::map
#include <random> // rand
#include <iterator> //advance
#include <string> // std::string, strlen

#include "byte_vector.h"
#include "sha1.h"


FILE *db_file = NULL;
FILE *index_file = NULL;
std::map<std::string, size_t> *gbov_map = NULL;

extern size_t offset;
extern size_t size;
extern size_t i_size;
extern size_t d_size;

extern byte_vector_t vector;


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
		close_file(&db_file);
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
	for (size_t i = 0; i < size - i_size - d_size; ++i) {
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

		close_file(&index_file);
	}

	if (gbov_map) {
		delete gbov_map;
		gbov_map = NULL;
	}

	return R_NilValue;
}

/**
 * This function assess if the input is a generic.
 * @method is_generic
 * @param  SEXP          Any R value
 * @return               1, therefore no need to actually implement
 */
// int is_generic(SEXP value) {
// 	return 1;
// }

/**
 * Adds an generic R value to the database.
 * @method add_dbl
 * @param  val is a generic R value
 * @return val
 */
SEXP add_generic(SEXP val) {
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
	} else {
		return R_NilValue;
	}
}

/**
 * This function asks if the C layer has seen an given generic value
 * @method have_seen
 * @param  val       R value in form of SEXP
 * @return           R value of True or False as a SEXP
 */
SEXP have_seen_generic(SEXP val) {
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

/**
 * This function gets the generic at the index'th place in the database.
 * @method get_dbl
 * @return [description]
 */
SEXP get_generic(int index) {
	std::map<std::string, size_t>::iterator it;
	it = gbov_map->begin();
	std::advance(it, index);

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

