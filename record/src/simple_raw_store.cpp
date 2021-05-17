#include "simple_raw_store.h"

#include "helper.h"

FILE *raw_file = NULL;

size_t raw_db[256] = { 0 };

extern size_t size;
extern size_t count;
extern size_t i_size;

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
		read_n(raw_file, raw_db + i, sizeof(Rbyte));     /*  object.size(as.raw(n)) returns 56 bytes */
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
			write_n(raw_file, &(raw_db[i]), sizeof(Rbyte));
		}

		close_file(&raw_file);

		memset(raw_db, 0, 256 * sizeof(Rbyte));
	}

	return R_NilValue;
}

/**
 * This function checks if the input is a raw value
 * @method is_simple_raw
 * @param  SEXP          Arbitrary R value
 * @return               1 if it is a raw value, 0 otherwise
 */
int is_scalar_raw (SEXP value) {
  IS_SCALAR(value, RAWSXP)
}

/**
 * Adds an R scalar integer value to a separate int database.
 * @method add_raw
 * @param  val    an arbitrary raw value
 * @return val
 */
SEXP add_raw(SEXP val) {
	Rbyte raw_val = RAW(val);
  int i = raw_val;
  //TODO
	if(raw_db[i] == 0) {
		raw_db[i] += 1;
		i_size += 1;
		size += 1;
		return val;
	} else {
		raw_db[i] += 1;
		count += 1;

		return R_NilValue;
	}
}

/**
 * This function asks if the C layer has seen a int in range [-5000, 5000]
 * @method have_seen
 * @param  val       R value in form of SEXP
 * @return           1 if the value has been encountered before, else 0
 */
int have_seen_raw(SEXP val) {
  Rbyte raw_val = RAW(val);
  int i = raw_val;
	return raw_db[i];
}

/**
 * This function gets the ith raw value in the database.
 * @method get_raw
 * @return [description]
 */
SEXP get_raw(int i) {
	if (i_size < 10001) {
		int values_passed = 0;
		for (int i = 0; i < 10001; ++i) {
			if (int_db[i] > 0) {
				++values_passed;
			}

			if (values_passed == index + 1) {
				SEXP res;
				R_xlen_t n = 1;
				PROTECT(res = allocVector(INTSXP, n));
				int *res_ptr = INTEGER(res);
				res_ptr[0] = i - INT_STORE_MAX;
				UNPROTECT(1);
				return res;
			}
		}
	} else {
		SEXP res;
		R_xlen_t n = 1;
		PROTECT(res = allocVector(INTSXP, n));
		int *res_ptr = INTEGER(res);
		res_ptr[0] = index - INT_STORE_MAX;
		UNPROTECT(1);
		return res;
	}
}
