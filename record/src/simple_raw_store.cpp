#include "simple_raw_store.h"

#include "helper.h"

FILE *raw_file = NULL;

size_t raw_db[256] = { 0 };

extern size_t size;     /* size of the database */
extern size_t count;    /* how many values have we encountered for the database */
extern size_t r_size;   /* number of unique raw values in the store */

/**
 * Create the common raw storage
 * @method init_simple_raw_store
 * @param  file                 file name
 * @return                      R_NilValue on succcecss
 */
SEXP init_simple_raw_store(SEXP file) {
	raw_file = open_file(file);

	for (int i = 0; i < 256; ++i) {
		raw_db[i] = 0;
	}

	return R_NilValue;
}

/**
 * Loads raw.bin in the database
 * @method loads_simple_raw_store
 * @param file                   file name
 * @return R_NilValue            on success throw and error otherwise
 */
SEXP load_simple_raw_store(SEXP file) {
	init_simple_raw_store(file);

	for (size_t i = 0; i < 256; ++i) {
		read_n(raw_file, raw_db + i, sizeof(size_t));
	}

	return R_NilValue;
}


/**
 * This function writes raw data to file and close the file.
 * @method close_simple_raw_store()
 * @return R_NilValue
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
 * This function checks if the input is a raw value
 * @method is_simple_raw
 * @param  SEXP          Arbitrary R value
 * @return               1 if it is a raw value, 0 otherwise
 */
int is_simple_raw (SEXP value) {
  return IS_SCALAR(value, RAWSXP);
}

/**
 * Adds an R scalar integer value to a separate int database.
 * @method add_raw
 * @param  val    an arbitrary raw value
 * @return val
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
 * This function asks if the C layer has seen a int in range [-5000, 5000]
 * @method have_seen
 * @param  val       R value in form of SEXP
 * @return           1 if the value has been encountered before, else 0
 */
int have_seen_simple_raw(SEXP val) {
  Rbyte *raw_val = RAW(val);
	return raw_db[*raw_val];
}

/**
 * This function gets the ith raw value in the database.
 * @method get_simple_raw
 * @return [description]
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

