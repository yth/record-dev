#include <R.h>
#include <Rinternals.h>

#ifndef RCRD_RCRD_H
#define RCRD_RCRD_H

#ifdef __cplusplus
extern "C" {
#endif


/**
 * This function sets up the initiatial state of the database.
 * This function must be called first.
 * @method setup
 * @return R_NilValue on succecss
 */
SEXP setup();


/**
 * This function tears down all traces of the database after running.
 * This function must be called last.
 * @method setup
 * @return R_NilValue on succecss
 */
SEXP teardown();


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
 * Adds an R scalar integer value to a separate int database.
 * @method add_int
 * @param  val strictly an integer value from -5000 to 5000
 * @return val
 */
SEXP add_int(SEXP val);


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
 * This function asks if the C layer has seen a int in range [-5000, 5000]
 * @method have_seen
 * @param  val       R value in form of SEXP
 * @return           R value of True or False as a SEXP
 */
SEXP have_seen_int(SEXP val);


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
SEXP sample_val();


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


#ifdef __cplusplus
} // extern "C"
#endif

#endif // RCRD_RCRD_H

