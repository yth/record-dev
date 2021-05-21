#include "simple_dbl_store.h"

#include "helper.h"

FILE *dbl_file = NULL;

// Small scalar dbl storage
double DBL_STORE_MAX = 5000;
double DBL_STORE_MIN = -5000;
int DBL_STORE_SIZE = DBL_STORE_MAX - DBL_STORE_MIN + 1;

// hard wired to accommodate doubles that represents ints between -5000 to 5000
size_t dbl_db[10001] = { 0 };

extern size_t size;
extern size_t d_size;

/**
 * Load/create a brand new simple double store.
 * @method init_simple_dbl_store
 * @return R_NilValue on success, throw and error otherwise
 */
SEXP init_simple_dbl_store(SEXP dbls) {
	dbl_file = open_file(dbls);

	for (int i = 0; i < DBL_STORE_SIZE; ++i) {
		dbl_db[i] = 0;
	}

	return R_NilValue;
}

/**
 * Load an existing simple double store.
 * @method load_simple_dbl_store
 * @return R_NilValue on success, throw and error otherwise
 */
SEXP load_simple_dbl_store(SEXP dbls) {
	init_simple_dbl_store(dbls);

	for (size_t i = 0; i < DBL_STORE_SIZE; ++i) {
		read_n(dbl_file, dbl_db + i, sizeof(size_t));
	}

	return R_NilValue;
}

/**
 * This functions writes simple double R val store to file and closes the file.
 * @method close_simple_dbl_store
 * @return R_NilValue on success
 */
SEXP close_simple_dbl_store() {
	if (dbl_file) {
		fseek(dbl_file, 0, SEEK_SET);

		for(int i = 0; i < DBL_STORE_SIZE; ++i) {
			write_n(dbl_file, &(dbl_db[i]), sizeof(size_t));
		}

		close_file(&dbl_file);

		memset(dbl_db, 0, DBL_STORE_SIZE * sizeof(size_t));
	}

	return R_NilValue;
}

/**
 * This function assesses if the input is a simple double.
 * @method is_simple_dbl
 * @param  SEXP          Any R value
 * @return               1 if it is a simple dbl, 0 otherwise
 */
int is_simple_dbl(SEXP value) {
	if (IS_SIMPLE_SCALAR(value, REALSXP)) {
		if (asReal(value) <= DBL_STORE_MAX &&
			asReal(value) >= DBL_STORE_MIN) {

			double tmp = asReal(value);

			return (tmp - (int) tmp == 0) ? 1 : 0;
		}
	}

	return 0;
}

/**
 * Adds a simple double R value to the simple double store.
 * @method add_simple_dbl
 * @param  val is a simple double R value
 * @return val if val hasn't been added to store before, else R_NilValue
 */
SEXP add_simple_dbl(SEXP val) {
	// dbl_db[0] represents -5000
	int dbl_val = (int) (asReal(val) - DBL_STORE_MIN);
	if(dbl_db[dbl_val] == 0) {
		dbl_db[dbl_val] += 1;
		d_size += 1;
		size += 1;
		return val;
	} else {
		dbl_db[dbl_val] += 1;
		return R_NilValue;
	}
}

/**
 * This function asks if the C layer has seen a given simple double value.
 * @method have_seen_simple_dbl
 * @param  val       an simple double R value in form of SEXP
 * @return           1 if the value has been encountered before, else 0
 */
int have_seen_simple_dbl(SEXP val) {
	// dbl_db[0] represents -5000
	int index = (int) (asReal(val) - DBL_STORE_MIN);
	return dbl_db[index];
}

/**
 * This function gets the simple double at the index'th place in the database.
 * @method get_simple_dbl
 * @return R value
 */
SEXP get_simple_dbl(int index) {
	if (d_size < DBL_STORE_SIZE) {
		int values_passed = 0;
		for (int i = 0; i < DBL_STORE_SIZE; ++i) {
			if (dbl_db[i] > 0) {
				++values_passed;
			}

			if (values_passed == index + 1) {
				SEXP res;
				R_xlen_t n = 1;
				PROTECT(res = allocVector(REALSXP, n));
				double *res_ptr = REAL(res);
				res_ptr[0] = i - DBL_STORE_MAX;
				UNPROTECT(1);
				return res;
			}
		}

		return R_NilValue; // Delete later
	} else {
		SEXP res;
		R_xlen_t n = 1;
		PROTECT(res = allocVector(REALSXP, n));
		double *res_ptr = REAL(res);
		res_ptr[0] = index - DBL_STORE_MAX;
		UNPROTECT(1);
		return res;
	}
}
