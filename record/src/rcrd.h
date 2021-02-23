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
 * This function adds an R value to the C layer.
 * @method r2c
 * @param  r_object SEXP that represents an R object
 * @param  storage  SEXP that represents the storage
 * @return          SEXP that can act as a hash for the value in the store
 */
//SEXP r2c(SEXP r_object, SEXP storage);


/**
 * This function returns an R value from the specified storage based on hash.
 * @method c2r
 * @param  hash    SEXP that can act as a hash for the value in the store
 * @param  storage SEXP that represents the storage
 * @return         SEXP that is the R value specified by the hash
 * TODO: HANDLE HASH COLLISION OR CREATE ANOTHER TYPE OF UNIQUE IDENTIFIER
 */
// SEXP c2r(SEXP hash, SEXP storage);

#ifdef __cplusplus
}; // extern "C"
#endif

#endif // RCRD_RCRD_H
