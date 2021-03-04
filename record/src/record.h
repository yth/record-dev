#include <R.h>
#include <Rinternals.h>

#ifndef RCRD_RCRD_H
#define RCRD_RCRD_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * This function creates a database for a collection of values.
 * @method record_init
 * @return R_NilValue; File pointer is kept in the C layer
 */
SEXP open_db(SEXP filename);


/**
 * This function closes a database.
 * @method record_close
 * @return R_NilValue on success
 */
SEXP close_db();


/**
 * This functions directly adds an R value to the specified storage.
 * @method r2cd
 * @param  val      R value in form of SEXP
 * @return          R value on success
 */
SEXP add_value(SEXP val);


/**
 * This function asks if the C layer has seen a R value.
 * @method has_value
 * @param  val       R value in form of SEXP
 * @return           True or False
 */
SEXP has_value(SEXP val);


/**
 * This function returns a random value from the database
 * @method get_random_value
 * @return R value in form of SEXP from the database
 */
SEXP get_random_value();


#ifdef __cplusplus
} // extern "C"
#endif

#endif // RCRD_RCRD_H
