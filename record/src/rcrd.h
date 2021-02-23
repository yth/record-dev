#include <R.h>
#include <Rinternals.h>

#ifndef RCRD_RCRD_H
#define RCRD_RCRD_H


/**
 * This function creates a database for a collection of values.
 * @method record_init
 * @return FILE pointer wrapped as a R external pointer
 */
SEXP open_db(SEXP filename);


/**
 * This function closes a database.
 * @method record_close
 * @param  file_ptr     wrapped FILE pointer
 * @return R_NilValue on success
 */
SEXP close_db(SEXP file_ptr);


/**
 * This functions directly adds an R value to the specified storage.
 * @method r2cd
 * @param  r_object [description]
 * @param  storage  [description]
 * @return          [description]
 */
SEXP add_value(SEXP r_string_object, SEXP file_ptr);


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

#endif // RCRD_RCRD_H
