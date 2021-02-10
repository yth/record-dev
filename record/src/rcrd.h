#include <R.h>
#include <Rinternals.h>

#ifndef RCRD_RCRD_H
#define RCRD_RCRD_H

/**
 * This function creates or loads a storage for a collection of values.
 * @method record_init
 * @param  file_name   char* that should be the storage name; file for now
 * @return             SEXP that represent the collection
 */
SEXP record_init(SEXP path_name);


/**
 * This function adds an R value to the specified storage
 * @method r2c
 * @param  r_object SEXP that represents an R object
 * @param  storage  SEXP that represents the storage
 * @return          SEXP that can act as a hash for the value in the store
 */
// SEXP r2c(SEXP r_object, SEXP storage);


/**
 * This function returns an R value from the specified storage based on hash
 * @method c2r
 * @param  hash    SEXP that can act as a hash for the value in the store
 * @param  storage SEXP that represents the storage
 * @return         SEXP that is the R value specified by the hash
 * TODO: HANDLE HASH COLLISION OR CREATE ANOTHER TYPE OF UNIQUE IDENTIFIER
 */
// SEXP c2r(SEXP hash, SEXP storage);

#endif // RCRD_RCRD_H
