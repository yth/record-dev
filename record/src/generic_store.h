#include <R.h>
#include <Rinternals.h>

#ifndef RCRD_GENERIC_STORE_H
#define RCRD_GENERIC_STORE_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Load/create a brand new generic store.
 * @method load_gbov
 * @return R_NilValue on success, throw and error otherwise
 */
SEXP create_generic_store(SEXP generics);

/**
 * Load an existing generic store.
 * @method load_gbov
 * @return R_NilValue on success, throw and error otherwise
 */
SEXP load_generic_store(SEXP generics);

/**
 * This functions writes generic R val store to file and closes the file.
 * @method close_gbov
 * @return R_NilValue on success
 */
SEXP close_generic_store();

/**
 * Load/create a brand new index associated with the generics store.
 * @method load_indices
 * @return R_NilValue on success, throw and error otherwise
 */
SEXP create_generic_index(SEXP index);

/**
 * Load an existing index associated with the generics store.
 * @method load_indices
 * @return R_NilValue on success, throw and error otherwise
 */
SEXP load_generic_index(SEXP index);

/**
 * This function writes the index associated with the generics store to file
 * and closes the file.
 * @method close_indices
 * @return R_NilValue
 */
SEXP close_generic_index();

/**
 * This function assesses if the input is a generic.
 * This function would always return true, therefore no need to implement it.
 * @method is_generic
 * @param  SEXP          Any R value
 * @return               1
 */
// int is_generic(SEXP value);

/**
 * Adds an generic R value to the generics store.
 * @method add_dbl
 * @param  val is a generic R value
 * @return val if val hasn't been added to store before, else R_NilValue
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
 * This function gets the generic value at the index'th place in the database.
 * @method get_dbl
 * @return R value
 */
SEXP get_generic(int index);

#ifdef __cplusplus
} // extern "C"
#endif

#endif // RCRD_GENERIC_STORE_H
