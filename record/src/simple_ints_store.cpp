#include "simple_ints_store.h"

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
SEXP init_simple_ints_store(SEXP ints) {
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
	init_simple_ints_store(ints);

	read_n(int_file, &i_size, sizeof(size_t));

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
SEXP close_ints() {
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

