#include <R.h>
#include <Rinternals.h>

#ifndef RCRD_RCRD_H
#define RCRD_RCRD_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * This function closes a database.
 * @method close_db
 * @return R_NilValue on success
 */
SEXP close_db();


/**
 * This function writes ints data to file and close the file.
 * @method close_ints
 * @return R_NilValue on success
 */
SEXP close_ints();

/**
 * Loads ints.bin in the database
 * @method load_ints
 * @return R_NilValue on success, throw and error otherwise
 */
SEXP load_ints(SEXP ints);


/**
 * Load the gbov.
 * @method load_gbov
 * @return R_NilValue on success, throw and error otherwise
 */
SEXP load_gbov(SEXP gbov);


/**
 * Load the indices associated with the gbov.
 * @method load_indices
 * @return R_NilValue on success, throw and error otherwise
 */
SEXP load_indices(SEXP indices);


/**
 * Create the gbov.
 * @method load_gbov
 * @return R_NilValue on success, throw and error otherwise
 */

SEXP create_gbov(SEXP gbov);


/**
 * Load the indices associated with the gbov.
 * @method load_indices
 * @return R_NilValue on success, throw and error otherwise
 */
SEXP create_indices(SEXP indices);


/**
 * Create the common ints storage
 * @method create_ints
 * @param  ints        file name
 * @return             R_NilValue on succcecss
 */
SEXP create_ints(SEXP ints);


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
 * @method count_vals
 * @param
 * @return  number of total values encountered
 */
SEXP count_vals();


SEXP size_db();


SEXP size_ints();


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
 * Report database statistics
 * @method report
 */
SEXP report();

/**
 * Create stats.bin in the database
 * @method create_stats
 * @return R_NilValue on success, throw and error otherwise
 */
SEXP create_stats(SEXP stats);


/**
 * Loads stats.bin in the database
 * @method load_stats
 * @return R_NilValue on success, throw and error otherwise
 */
SEXP load_stats(SEXP stats);


/**
 * This function writesdatabase information to file and close the file.
 * @method close_stats
 * @return R_NilValue on success
 */
SEXP close_stats();


#ifdef __cplusplus
} // extern "C"
#endif

#endif // RCRD_RCRD_H

