#include "simple_str_store.h"

#include "helper.h"

#include "crc.h" // CRC32

#include <map> // std::map
#include <iterator> //advance
#include <string> // strlen
#include <cstdint> // uint32_t

FILE *str_file = NULL;
FILE *str_index_file = NULL;
std::map<uint32_t, size_t> *str_index = NULL;
size_t MAX_LENGTH = 8;

extern size_t size;
extern size_t s_size;
extern size_t s_offset;

/**
 * Load/create a brand new simple string store.
 * @method init_simple_str_store
 * @return R_NilValue on success, throw and error otherwise
 */
SEXP init_simple_str_store(SEXP strs) {
	str_file = open_file(strs);
	fseek(str_file, s_offset, SEEK_SET);
	return R_NilValue;
}

/**
 * Load an existing simple string store.
 * @method load_simple_str_store
 * @return R_NilValue on success, throw and error otherwise
 */
SEXP load_simple_str_store(SEXP strs) {
	return init_simple_str_store(strs);
}

/**
 * This functions writes simple string store to file and closes the file.
 * @method close_simple_str_store
 * @return R_NilValue on success
 */
SEXP close_simple_str_store() {
	if (str_file) {
		close_file(&str_file);
	}

	return R_NilValue;
}

/**
 * Load/create a brand new index associated with the simple string store.
 * @method init_simple_str_index
 * @return R_NilValue on success, throw and error otherwise
 */
SEXP init_simple_str_index(SEXP index) {
	str_index_file = open_file(index);
	str_index = new std::map<uint32_t, size_t>;
	return R_NilValue;
}

/**
 * Load an existing index associated with the simple string store.
 * @method load_simple_str_index
 * @return R_NilValue on success, throw and error otherwise
 */
SEXP load_simple_str_index(SEXP index) {
	init_simple_str_index(index);

	for (size_t i = 0; i < s_size; ++i) {
		uint32_t hash = 0;
		size_t index = 0;
		read_n(str_index_file, &hash, sizeof(uint32_t));
		read_n(str_index_file, &index, sizeof(size_t));
		(*str_index)[hash] = index;
	}

	return R_NilValue;
}

/**
 * This function writes the index associated with the simple str store to file
 * and closes the file.
 * @method close_simple_str_index
 * @return R_NilValue
 */
SEXP close_simple_str_index() {
	if (str_index_file) {
		// TODO: Think about ways to reuse rather than overwrite each time
		fseek(str_index_file, 0, SEEK_SET);

		std::map<uint32_t, size_t>::iterator it;
		for(it = str_index->begin(); it != str_index->end(); it++) {
			write_n(str_index_file, (void *) &(it->first), sizeof(uint32_t));
			write_n(str_index_file, &(it->second), sizeof(size_t));
		}

		close_file(&str_index_file);
	}

	if (str_index) {
		delete str_index;
		str_index = NULL;
	}

	return R_NilValue;
}

/**
 * This function assesses if the input is a simple string.
 * This function would always return true, therefore no need to implement it.
 * @method is_simple_str
 * @param  SEXP          Any R value
 * @return               1 if the value is a simple string, else 0
 */
int is_simple_str(SEXP value) {
	if (IS_SIMPLE_SCALAR(value, STRSXP)
		&& strlen(CHAR(STRING_ELT(value, 0))) <= MAX_LENGTH) {
		return 1;
	}
	return 0;
}

/**
 * Adds an simple string value to the simple string store.
 * @method add_simple_str
 * @param  val is a generic R value
 * @return val if val hasn't been added to store before, else R_NilValue
 */
SEXP add_simple_str(SEXP val) {
	const char* c_val = CHAR(STRING_ELT(val, 0));
	uint32_t hash = CRC32(c_val);
	std::map<uint32_t, size_t>::iterator it = str_index->find(hash);
	if (it == str_index->end()) { // TODO: Deal with collisions
		(*str_index)[hash] = s_size;
		s_size++;
		size++;

		size_t len = strlen(c_val);
		write_n(str_file, (void *) c_val, len);
		if (len < MAX_LENGTH) {
			char buf [MAX_LENGTH] = { 0 };
			write_n(str_file, buf, MAX_LENGTH - len);
		}

		s_offset += MAX_LENGTH;

		return val;
	}

	return R_NilValue;
}

/**
 * This function asks if the C layer has seen an given simple string
 * @method have_seen_simple_str
 * @param  val       R value in form of SEXP
 * @return           1 if the value has been encountered before, else 0
 */
int have_seen_simple_str(SEXP val) {
	const char* c_val = CHAR(STRING_ELT(val, 0));
	uint32_t hash = CRC32(c_val);

	std::map<uint32_t, size_t>::iterator it = str_index->find(hash);

	if (it == str_index->end()) {
		return 0;
	} else {
		return 1;
	}
}

/**
 * This function gets the simple string at the index'th place in the database.
 * @method get_simple_str
 * @return R value
 */
SEXP get_simple_str(int index) {
	char buf [9] = { 0 };

	fseek(str_file, index * 8, SEEK_SET);
	read_n(str_file, buf, 8);
	fseek(str_file, s_offset, SEEK_SET);

	SEXP r_res = PROTECT(allocVector(STRSXP, 1));
	SET_STRING_ELT(r_res, 0, mkChar(buf));
	UNPROTECT(1);

	return r_res;
}