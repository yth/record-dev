#include "generic_store.h"

#include "helper.h"

#include <map> // std::map
#include <random> // rand
#include <iterator> //advance
#include <string> // std::string, strlen

#include "byte_vector.h"
#include "sha1.h"

FILE *generics_file = NULL;
FILE *generics_index_file = NULL;
std::map<std::string, size_t> *gbov_map = NULL;

extern size_t offset;
extern size_t size;
extern size_t g_size;

extern byte_vector_t vector;

/**
 * Load/create a brand new generic store.
 * @method create_generic_store
 * @return R_NilValue on success, throw and error otherwise
 */
SEXP create_generic_store(SEXP generics) {
	generics_file = open_file(generics);
	fseek(generics_file, offset, SEEK_SET);
	return R_NilValue;
}

/**
 * Load an existing generic store.
 * @method load_generic_store
 * @return R_NilValue on success, throw and error otherwise
 */
SEXP load_generic_store(SEXP generics) {
	return create_generic_store(generics);
}

/**
 * This functions writes generic R val store to file and closes the file.
 * @method close_generic_store
 * @return R_NilValue on success
 */
SEXP close_generic_store() {
	if (generics_file) {
		close_file(&generics_file);
	}

	return R_NilValue;
}

/**
 * Load/create a brand new index associated with the generics store.
 * @method create_generic_index
 * @return R_NilValue on success, throw and error otherwise
 */
SEXP create_generic_index(SEXP index) {
	generics_index_file = open_file(index);

	gbov_map = new std::map<std::string, size_t>;

	return R_NilValue;
}

/**
 * Load an existing index associated with the generics store.
 * @method load_generic_index
 * @return R_NilValue on success, throw and error otherwise
 */
SEXP load_generic_index(SEXP index) {
	create_generic_index(index);

	size_t start = 0;
	char hash[20];
	for (size_t i = 0; i < g_size; ++i) {
		read_n(generics_index_file, hash, 20);
		read_n(generics_index_file, &start, sizeof(size_t));
		(*gbov_map)[std::string(hash, 20)] = start;
	}

	return R_NilValue;
}

/**
 * This function writes the index associated with the generics store to file
 * and closes the file.
 * @method close_generic_index
 * @return R_NilValue
 */
SEXP close_generic_index() {
	if (generics_index_file) {
		// TODO: Think about ways to reuse rather than overwrite each time
		fseek(generics_index_file, 0, SEEK_SET);

		std::map<std::string, size_t>::iterator it;
		for(it = gbov_map->begin(); it != gbov_map->end(); it++) {
			write_n(generics_index_file, (void *) it->first.c_str(), 20);
			write_n(generics_index_file, &(it->second), sizeof(size_t));
		}

		close_file(&generics_index_file);
	}

	if (gbov_map) {
		delete gbov_map;
		gbov_map = NULL;
	}

	return R_NilValue;
}

/**
 * This function assesses if the input is a generic.
 * This function would always return true, therefore no need to implement it.
 * @method is_generic
 * @param  SEXP          Any R value
 * @return               1
 */
// int is_generic(SEXP value) {
// 	return 1;
// }

/**
 * Adds an generic R value to the generics store.
 * @method add_generic
 * @param  val is a generic R value
 * @return val if val hasn't been added to store before, else R_NilValue
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
		g_size++;
		size++;

		// Write the blob
		write_n(generics_file, &(vector->size), sizeof(size_t));
		write_n(generics_file, vector->buf, vector->size);

		// Acting as NULL for linked-list next pointer
		write_n(generics_file, &(vector->size), sizeof(size_t));

		// Modify offset here
		offset += vector->size + sizeof(size_t) + sizeof(size_t);

		return val;
	} else {
		return R_NilValue;
	}
}

/**
 * This function asks if the C layer has seen an given generic value
 * @method have_seen_generic
 * @param  val       R value in form of SEXP
 * @return           1 if the value has been encountered before, else 0
 */
int have_seen_generic(SEXP val) {
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
		return 0;
	} else {
		return 1;
	}
}

/**
 * This function gets the generic value at the index'th place in the database.
 * @method get_dbl
 * @return R value
 */
SEXP get_generic(int index) {
	std::map<std::string, size_t>::iterator it = gbov_map->begin();
	std::advance(it, index);

	// Get the specified value
	size_t obj_size;
	free_content(vector);
	fseek(generics_file, it->second, SEEK_SET);
	read_n(generics_file, &obj_size, sizeof(size_t));
	read_n(generics_file, vector->buf, obj_size);
	fseek(generics_file, offset, SEEK_SET);
	vector->capacity = obj_size;

	SEXP res = unserialize_val(vector);

	// Restore vector
	vector->capacity = 1 << 30;

	return res;
}
