#include <R.h>
#include <Rinternals.h>

#ifndef RCRD_GENERIC_STORE_H
#define RCRD_GENERIC_STORE_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Load the indices associated with the gbov.
 * @method load_indices
 * @return R_NilValue on success, throw and error otherwise
 */
SEXP create_indices(SEXP indices);


/**
 * Load the indices associated with the gbov.
 * @method load_indices
 * @return R_NilValue on success, throw and error otherwise
 */
SEXP load_indices(SEXP indices);


/**
 * This function  writes indices to file and closes the file.
 * @method close_indices
 * @return [description]
 */
SEXP close_indices();

/**
 * Create the gbov.
 * @method load_gbov
 * @return R_NilValue on success, throw and error otherwise
 */

SEXP create_gbov(SEXP gbov);


/**
 * Load the gbov.
 * @method load_gbov
 * @return R_NilValue on success, throw and error otherwise
 */
SEXP load_gbov(SEXP gbov);


/**
 * This functions writes generic R val store to file and closes the file.
 * @method close_gbov
 * @return R_NilValue on success
 */
SEXP close_gbov();

/**
 * This function assess if the input is a generic.
 * @method is_generic
 * @param  SEXP          Any R value
 * @return               1, therefore no need to actually implement
 */
// int is_generic(SEXP value);

/**
 * Adds an generic R value to the database.
 * @method add_dbl
 * @param  val is a generic R value
 * @return val
 */
SEXP add_generic(SEXP val);

/**
 * This function asks if the C layer has seen an given generic value
 * @method have_seen
 * @param  val       R value in form of SEXP
 * @return           1 if the value has been encountered before, else 0
 */
int have_seen_generic(SEXP val);

/**
 * This function gets the generic at the index'th place in the database.
 * @method get_dbl
 * @return [description]
 */
SEXP get_generic(int index);

#ifdef __cplusplus
} // extern "C"
#endif

#endif // RCRD_GENERIC_STORE_H
