#include <R.h>
#include <Rinternals.h>

#ifndef RCRD_SIMPLE_INT_STORE_H
#define RCRD_SIMPLE_INT_STORE_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Create the common ints storage
 * @method create_ints
 * @param  ints        file name
 * @return             R_NilValue on succcecss
 */
SEXP init_simple_int_store(SEXP ints);

/**
 * Loads ints.bin in the database
 * @method load_ints
 * @return R_NilValue on success, throw and error otherwise
 */
SEXP load_simple_int_store(SEXP ints);

/**
 * This function writes ints data to file and close the file.
 * @method close_ints
 * @return R_NilValue on success
 */
SEXP close_simple_int_store();

/**
 * This function assess if the input is a simple integer
 * @method is_simple_int
 * @param  SEXP          Any R value
 * @return               1 if it is a simple int, 0 otherwise
 */
int is_simple_int(SEXP value);

/**
 * Adds an R scalar integer value to a separate int database.
 * @method add_int
 * @param  val strictly an integer value from -5000 to 5000
 * @return val
 */
SEXP add_simple_int(SEXP val);

/**
 * This function asks if the C layer has seen a int in range [-5000, 5000]
 * @method have_seen
 * @param  val       R value in form of SEXP
 * @return           R value of True or False as a SEXP
 */
SEXP have_seen_simple_int(SEXP val);

/**
 * This function samples from the simple ints that the database has.
 * @method get_int
 * @return [description]
 */
SEXP get_simple_int(int index);

#ifdef __cplusplus
} // extern "C"
#endif

#endif // RCRD_SIMPLE_INT_STORE_H
