#include <R.h>
#include <Rinternals.h>

#ifndef RCRD_SIMPLE_INTS_STORE_H
#define RCRD_SIMPLE_INTS_STORE_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Create the common ints storage
 * @method create_ints
 * @param  ints        file name
 * @return             R_NilValue on succcecss
 */
SEXP create_ints(SEXP ints);

/**
 * Loads ints.bin in the database
 * @method load_ints
 * @return R_NilValue on success, throw and error otherwise
 */
SEXP load_ints(SEXP ints);

/**
 * This function writes ints data to file and close the file.
 * @method close_ints
 * @return R_NilValue on success
 */
SEXP close_ints();

/**
 * Adds an R scalar integer value to a separate int database.
 * @method add_int
 * @param  val strictly an integer value from -5000 to 5000
 * @return val
 */
SEXP add_int(SEXP val);

/**
 * This function asks if the C layer has seen a int in range [-5000, 5000]
 * @method have_seen
 * @param  val       R value in form of SEXP
 * @return           R value of True or False as a SEXP
 */
SEXP have_seen_int(SEXP val);

#ifdef __cplusplus
} // extern "C"
#endif

#endif // RCRD_SIMPLE_INTS_STORE_H
