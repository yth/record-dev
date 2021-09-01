// TODO: Add sampling for logical, integer, double, complex, character, raw, list

#include "record.h"

#include "byte_vector.h"
#include "helper.h" // rans_d_size_t

// Include all stores
#include "stats_store.h"
#include "generic_store.h"

#include "int_store.h"
#include "simple_int_store.h"

#include "dbl_store.h"
#include "simple_dbl_store.h"

#include "simple_raw_store.h"
#include "simple_str_store.h"

// Reusable buffer for everything
byte_vector_t vector = NULL;

// Pulled in from stats_store.cpp
extern size_t count;
extern size_t size;

extern size_t i_size;
extern size_t s_i_size;

extern size_t d_size;
extern size_t s_d_size;

extern size_t s_r_size;
extern size_t s_s_size;
extern size_t g_size;

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
 * This functions adds an R value to the database.
 * @method add_val
 * @param  val      R value in form of SEXP
 * @return          val if val hasn't been added to database before,
 *                  else R_NilValue
 */
SEXP add_val(SEXP val) {
	count += 1;

	if (is_simple_int(val)) {
		return add_simple_int(val);
	} else if (is_int(val)) {
		return add_int(val);
	} else if (is_simple_dbl(val)) {
		return add_simple_dbl(val);
	} else if (is_dbl(val)) {
		return add_dbl(val);
	} else if (is_simple_raw(val)) {
		return add_simple_raw(val);
	} else if (is_simple_str(val)) {
		return add_simple_str(val);
	} else {
		return add_generic(val);
	}
}

/**
 * This function asks if the C layer has seen a R value.
 * This function is mainly used for testing.
 * @method have_seen
 * @param  val       R value in form of SEXP
 * @return           R value of True or False as a SEXP
 */
SEXP have_seen(SEXP val) {
	SEXP res = PROTECT(allocVector(LGLSXP, 1));
	int *res_ptr = LOGICAL(res);

	if (is_simple_int(val)) {
		res_ptr[0] = have_seen_simple_int(val);
	} else if (is_simple_dbl(val)) {
		res_ptr[0] = have_seen_simple_dbl(val);
	} else if (is_simple_raw(val)) {
		res_ptr[0] = have_seen_simple_raw(val);
	} else if (is_simple_str(val)) {
		res_ptr[0] = have_seen_simple_str(val);
	} else {
		res_ptr[0] = have_seen_generic(val);
	}

	UNPROTECT(1);
	return res;
}

/**
 * This function returns a random value from the database
 * @method sample_val
 * @return R value in form of SEXP from the database
 */
SEXP sample_val() {
	size_t random_index = rand_size_t() % size;

	if (random_index < s_i_size + i_size) {
		return sample_int();
	} else if (random_index - (s_i_size + i_size) < s_d_size + d_size) {
		return sample_dbl();
	} else if (random_index - s_i_size - s_d_size < s_r_size) {
		return get_simple_raw(random_index - s_i_size - s_d_size);
	} else if (random_index - s_i_size - s_d_size - s_r_size < s_s_size) {
		return get_simple_str(random_index - s_i_size - s_d_size - s_r_size);
	} else {
		return get_generic(random_index - s_i_size - s_d_size - s_r_size - s_s_size);
	}
}

/**
 * This function returns a value from the database specified by an order
 * @method get_val
 * @param  i       R value in form of SEXP that is an index,
 *                 it must be non-negative
 * @return R value in form of SEXP from the database at ith position
 */
SEXP get_val(SEXP i) {
	int index = asInteger(i);

	// TODO: Let int db understand simple int db and etc

	if (index < s_i_size) {
		return get_simple_int(index);
	} if (index - s_i_size < i_size) {
		return get_int(index - s_i_size);
	} else if (index - s_i_size - i_size < s_d_size) {
		return get_simple_dbl(index - s_i_size - i_size);
	} else if (index - s_i_size - i_size - s_d_size < d_size) {
		return get_dbl(index - s_i_size - i_size - s_d_size);
	} else if (index - s_i_size - i_size - s_d_size - d_size < s_r_size) {
		return get_simple_raw(index - s_i_size - i_size - s_d_size - d_size);
	} else if (index - s_i_size - i_size - s_d_size - d_size - s_r_size < s_s_size) {
		return get_simple_str(index - s_i_size - i_size - s_d_size - d_size - s_r_size);
	} else {
		return get_generic(index - s_i_size - i_size - s_d_size - d_size - s_r_size - s_s_size);
	}
}
