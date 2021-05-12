#include <R.h>
#include <Rinternals.h>

#ifndef RCRD_SIMPLE_DBLS_STORE_H
#define RCRD_SIMPLE_DBLS_STORE_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Create the common dbls storage
 * @method create_dbls
 * @param  dbls        file name
 * @return             R_NilValue on succcecss
 */
SEXP init_simple_dbl_store(SEXP dbls);

/**
 * Loads dbls.bin in the database
 * @method load_dbls
 * @return R_NilValue on success, throw and error otherwise
 */
SEXP load_simple_dbl_store(SEXP dbls);

/**
 * This function writes dbls data to file and close the file.
 * @method close_dbls
 * @return R_NilValue on success
 */
SEXP close_simple_dbl_store();

/**
 * This function assess if the input is a simple dbleger
 * @method is_simple_dbl
 * @param  SEXP          Any R value
 * @return               1 if it is a simple dbl, 0 otherwise
 */
int is_simple_dbl(SEXP value);

/**
 * Adds an R scalar dbleger value to a separate dbl database.
 * @method add_dbl
 * @param  val strictly an dbleger value from -5000 to 5000
 * @return val
 */
SEXP add_simple_dbl(SEXP val);

/**
 * This function asks if the C layer has seen a dbl in range [-5000, 5000]
 * @method have_seen
 * @param  val       R value in form of SEXP
 * @return           1 if the value has been encountered before, else 0
 */
int have_seen_simple_dbl(SEXP val);

/**
 * This function samples from the simple dbls that the database has.
 * @method get_dbl
 * @return [description]
 */
SEXP get_simple_dbl(int index);

#ifdef __cplusplus
} // extern "C"
#endif

#endif // RCRD_SIMPLE_DBLS_STORE_H
