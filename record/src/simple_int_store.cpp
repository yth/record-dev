#include "simple_int_store.h"

#include "helper.h"

FILE *int_file = NULL;

// Small scalar int storage
int INT_STORE_MAX = 5000;
int INT_STORE_MIN = -5000;
size_t int_db[10001] = { 0 };    // hard wired to accommodate -5000 to 5000

extern size_t size;
extern size_t i_size;

/**
 * Create the common ints storage
 * @method create_ints
 * @param  ints        file name
 * @return             R_NilValue on succcecss
 */
SEXP init_simple_int_store(SEXP ints) {
	int_file = open_file(ints);

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
SEXP load_simple_int_store(SEXP ints) {
	init_simple_int_store(ints);

	for (size_t i = 0; i < 10001; ++i) {
		read_n(int_file, int_db + i, sizeof(size_t));
	}

	return R_NilValue;
}


/**
 * This function writes ints data to file and close the file.
 * @method close_ints
 * @return [description]
 */
SEXP close_simple_int_store() {
	if (int_file) {
		fseek(int_file, 0, SEEK_SET);

		for(int i = 0; i < 10001; ++i) {
			write_n(int_file, &(int_db[i]), sizeof(size_t));
		}

		close_file(&int_file);

		memset(int_db, 0, 10001 * sizeof(size_t));
	}

	return R_NilValue;
}

/**
 * This function assess if the input is a simple integer
 * @method is_simple_int
 * @param  SEXP          Any R value
 * @return               1 if it is a simple int, 0 otherwise
 */
int is_simple_int(SEXP value) {
	if (IS_SIMPLE_SCALAR(value, INTSXP)) {
		if (asInteger(value) <= INT_STORE_MAX &&
			asInteger(value) >= INT_STORE_MIN) {
			return 1;
		}
	}

	return 0;
}

/**
 * Adds an R scalar integer value to a separate int database.
 * @method add_int
 * @param  val strictly an integer value from -5000 to 5000
 * @return val
 */
SEXP add_simple_int(SEXP val) {
	int int_val = asInteger(val) + 5000; // int_db[0] represents -5000L
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
 * This function asks if the C layer has seen a int in range [-5000, 5000]
 * @method have_seen
 * @param  val       R value in form of SEXP
 * @return           1 if the value has been encountered before, else 0
 */
int have_seen_simple_int(SEXP val) {
	int index = Rf_asInteger(val) + 5000;
	return int_db[index];
}

/**
 * This function gets the index'th valid int recorded in the database.
 * @method get_int
 * @return [description]
 */
SEXP get_simple_int(int index) {
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
