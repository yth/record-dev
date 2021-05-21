#include "simple_raw_store.h"

#include "helper.h"

FILE *raw_file = NULL;

size_t raw_db[256] = { 0 };

extern size_t size;     /* size of the database */
extern size_t r_size;   /* number of unique raw values in the store */

/**
 * Load/create a brand new simple raw store.
 * @method init_simple_raw_store
 * @return R_NilValue on success, throw and error otherwise
 */
SEXP init_simple_raw_store(SEXP file) {
	raw_file = open_file(file);

	for (int i = 0; i < 256; ++i) {
		raw_db[i] = 0;
	}

	return R_NilValue;
}

/**
 * Load an existing simple raw store.
 * @method load_simple_raw_store
 * @return R_NilValue on success, throw and error otherwise
 */
SEXP load_simple_raw_store(SEXP file) {
	init_simple_raw_store(file);

	for (size_t i = 0; i < 256; ++i) {
		read_n(raw_file, raw_db + i, sizeof(size_t));
	}

	return R_NilValue;
}

/**
 * This functions writes simple raw R val store to file and closes the file.
 * @method close_simple_raw_store
 * @return R_NilValue on success
 */
SEXP close_simple_raw_store() {
	if (raw_file) {
		fseek(raw_file, 0, SEEK_SET);

		for(int i = 0; i < 256; ++i) {
			write_n(raw_file, &(raw_db[i]), sizeof(size_t));
			raw_db[i] = 0;
		}

		close_file(&raw_file);
	}

	return R_NilValue;
}

/**
 * This function assesses if the input is a simple raw.
 * @method is_simple_raw
 * @param  SEXP          Any R value
 * @return               1 if it is a simple raw, 0 otherwise
 */
int is_simple_raw (SEXP value) {
  return IS_SCALAR(value, RAWSXP);
}

/**
 * Adds a simple integer R value to the simple raw store.
 * @method add_simple_raw
 * @param  val is an R raw value
 * @return val if val hasn't been added to store before, else R_NilValue
 */
SEXP add_simple_raw(SEXP val) {
  Rbyte *raw_val = RAW(val);

	if(raw_db[*raw_val] == 0) {
		raw_db[*raw_val] += 1;
		r_size += 1;
		size += 1;
		return val;
	} else {
		raw_db[*raw_val] += 1;
		return R_NilValue;
	}
}

/**
 * This function asks if the C layer has seen a given simple raw value.
 * @method have_seen_simple_raw
 * @param  val       R raw value in form of SEXP
 * @return           1 if the value has been encountered before, else 0
 */
int have_seen_simple_raw(SEXP val) {
	Rbyte *raw_val = RAW(val);
	return raw_db[*raw_val];
}

/**
 * This function gets the simple raw at the index'th place in the database.
 * @method get_simple_raw
 * @return R value
 */
SEXP get_simple_raw(int index) {
	if (r_size < 256) {
		int values_passed  = 0;
		for (int i = 0; i < 256; ++i) {
			if (raw_db[i] > 0) {
				++values_passed;
			}

			if (values_passed == index + 1) {
				SEXP res;
				R_xlen_t n = 1;
				PROTECT(res = allocVector(RAWSXP, n));
				Rbyte *res_ptr = RAW(res);
				res_ptr[0] = i;
				UNPROTECT(1);
				return res;
			}
		}
	} else {
		SEXP res;
		R_xlen_t n = 1;
		PROTECT(res = allocVector(RAWSXP, n));
		Rbyte *res_ptr = RAW(res);
		res_ptr[0] = index;
		UNPROTECT(1);
		return res;
	}
}
