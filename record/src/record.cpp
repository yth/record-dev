#include "record.h"


#include "byte_vector.h"
#include "sha1.h"
#include "helper.h"

#include "stats_store.h"
#include "generic_store.h"
#include "simple_int_store.h"
#include "simple_dbl_store.h"


// Pulled in from stats_store.cpp
extern size_t count;
extern size_t size;
extern size_t offset;
extern size_t i_size;
extern size_t d_size;
extern size_t g_size;

// tmp
extern FILE* db_file;
extern FILE *index_file;
extern std::map<std::string, size_t> *gbov_map;


// Reusable buffer for everything
byte_vector_t vector = NULL;


/**
 * This function sets up the initiatial state of the database.
 * This function must be called first.
 * @method setup
 * @return R_NilValue on succecss
 */
SEXP setup() {
	vector = make_vector(1 << 30);

	return R_NilValue;
}


/**
 * This function tears down all traces of the database after running.
 * This function must be called last.
 * @method setup
 * @return R_NilValue on succecss
 */
SEXP teardown() {
	if (vector) {
		free_vector(vector);
		vector = NULL;
	}

	return R_NilValue;
}


/**
 * This functions directly adds an R value to the specified storage.
 * @method add_val
 * @param  val arbitrary R value
 * @return val same as input val
 */
SEXP add_val(SEXP val) {
	count += 1;

	if (is_simple_int(val)) {
		return add_simple_int(val);
	} else if (is_simple_dbl(val)) {
		// printf("is_simple_dbl: %f\n", *REAL(val));
		return add_simple_dbl(val);
	} else {
		return add_generic(val);
	}
}

SEXP have_seen(SEXP val) {
	if (is_simple_int(val)) {
		return have_seen_simple_int(val);
	} else if (is_simple_dbl(val)) {
		return have_seen_simple_dbl(val);
	} else {
		return have_seen_generic(val);
	}
}

SEXP sample_val() {
	// Specify a random value
	// TODO: Find a better rand() function that can return any size_t value
	int random_index = rand() % size;
	if (random_index < i_size) {
		return get_simple_int(random_index);
	} else if (random_index - i_size < d_size) {
		return get_simple_dbl(random_index - i_size);
	} else {
		return get_generic(random_index - i_size - d_size);
	}
}

SEXP read_vals(SEXP from, SEXP to) {
  //TODO
	return R_NilValue;
}

