#include <R.h>
#include <Rinternals.h>

#ifndef RCRD_RCRD_H
#define RCRD_RCRD_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * This function creates a database for a collection of values.
 * @method open_db
 * @return R_NilValue; File pointer is kept in the C layer
 */
SEXP open_db(SEXP filename);


/**
 * This function closes a database.
 * @method close_db
 * @return R_NilValue on success
 */
SEXP close_db();


/**
 * Load the gbov.
 * @method load_gbov
 * @return R_NilValue on success throw and error otherwise
 */
SEXP load_gbov(SEXP gbov);


/**
 * Load the indices associated with the gbov.
 * @method load_indices
 * @return R_NilValue on success throw and error otherwise
 */
SEXP load_indices(SEXP indices);


/**
 * Create the gbov.
 * @method load_gbov
 * @return R_NilValue on success throw and error otherwise
 */

SEXP create_gbov(SEXP gbov);


/**
 * Load the indices associated with the gbov.
 * @method load_indices
 * @return R_NilValue on success throw and error otherwise
 */
SEXP create_indices(SEXP indices);


/**
 * This functions directly adds an R value to the specified storage.
 * @method add_val
 * @param  val      R value in form of SEXP
 * @return          R value on success
 */
SEXP add_val(SEXP val);


/**
 * This function asks if the C layer has seen a R value.
 * @method have_seen
 * @param  val       R value in form of SEXP
 * @return           R value of True or False as a SEXP
 */
SEXP have_seen(SEXP val);


/**
 * This function asks if the C layer has seen a R value.
 * @method count_vals
 * @param
 * @return  number of total values encountered
 */
SEXP count_vals();


/**
 * This function asks if the C layer has seen a R value.
 * @method count_u_vals
 * @param
 * @return  number of values stored in the file
 */
SEXP size_db();


/**
 * This function asks if the C layer has seen a R value.
 * @method read_vals
 * @param   from, to
 * @return  values from to
 */
SEXP read_vals(SEXP from, SEXP to);


/**
 * This function returns a random value from the database
 * @method get_random_val
 * @return R value in form of SEXP from the database
 */
SEXP get_random_val();


#ifdef __cplusplus
} // extern "C"
#endif

#endif // RCRD_RCRD_H
