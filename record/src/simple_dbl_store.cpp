#include "simple_dbl_store.h"

#include "helper.h"

FILE *dbl_file = NULL;

// Small scalar dbl storage
double DBL_STORE_MAX = 5000;
double DBL_STORE_MIN = -5000;
size_t dbl_db[10001] = { 0 };    // hard wired to accommodate -5000 to 5000

extern size_t size;
extern size_t count;
extern size_t d_size;

/**
 * Create the common dbls storage
 * @method create_dbls
 * @param  dbls        file name
 * @return             R_NilValue on succcecss
 */
SEXP init_simple_dbl_store(SEXP dbls) {
	dbl_file = open_file(dbls);

	for (int i = 0; i < 10001; ++i) {
		dbl_db[i] = 0;
	}

	return R_NilValue;
}

/**
 * Loads dbls.bin in the database
 * @method loads_dbls
 * @return R_NilValue on success throw and error otherwise
 */
SEXP load_simple_dbl_store(SEXP dbls) {
	init_simple_dbl_store(dbls);

	for (size_t i = 0; i < 10001; ++i) {
		read_n(dbl_file, dbl_db + i, sizeof(size_t));
	}

	return R_NilValue;
}


/**
 * This function writes dbls data to file and close the file.
 * @method close_dbls
 * @return [description]
 */
SEXP close_simple_dbl_store() {
	if (dbl_file) {
		fseek(dbl_file, 0, SEEK_SET);

		for(int i = 0; i < 10001; ++i) {
			write_n(dbl_file, &(dbl_db[i]), sizeof(size_t));
		}

		close_file(&dbl_file);

		memset(dbl_db, 0, 10001 * sizeof(size_t));
	}

	return R_NilValue;
}

/**
 * This function assess if the input is a simple dbleger
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
 * Adds an R scalar dbleger value to a separate dbl database.
 * @method add_dbl
 * @param  val strictly an dbleger value from -5000 to 5000
 * @return val
 */
SEXP add_simple_dbl(SEXP val) {
	int dbl_val = (int) (asReal(val) + 5000); // dbl_db[0] represents -5000
	if(dbl_db[dbl_val] == 0) {
		dbl_db[dbl_val] += 1;
		d_size += 1;
		size += 1;
		return val;
	} else {
		dbl_db[dbl_val] += 1;
		count += 1;

		return R_NilValue;
	}
}

// TODO: Let people pass in dbls instead
SEXP have_seen_simple_dbl(SEXP val) {
	int index = (int) (asReal(val) + 5000);
	int found = 0;
	if (dbl_db[index]) {
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

/**
 * This function gets the index'th valid dbl recorded in the database.
 * @method get_dbl
 * @return [description]
 */
SEXP get_simple_dbl(int index) {
	if (d_size < 10001) {
		int values_passed = 0;
		for (int i = 0; i < 10001; ++i) {
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
