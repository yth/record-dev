#include <R.h>
#include <Rinternals.h>

#ifndef RCRD_STATS_STORE_H
#define RCRD_STATS_STORE_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Create stats.bin in the database
 * @method create_stats
 * @return R_NilValue on success, throw and error otherwise
 */
SEXP init_stats_store(SEXP stats);

/**
 * Loads stats.bin in the database
 * @method load_stats
 * @return R_NilValue on success, throw and error otherwise
 */
SEXP load_stats_store(SEXP stats);

/**
 * This function writes database information to file and close the file.
 * @method close_stats
 * @return R_NilValue on success
 */
SEXP close_stats_store();

/**
 * Report database statistics
 * @method report
 */
SEXP print_report();

/**
 * This function asks for how many R values the C add_val has seen.
 * @method count_vals
 * @return number of total values encountered by add_val
 */
SEXP count_vals();

/**
 * This function asks for how many values are stored in the database
 * @method size_db
 * @return [description]
 */
SEXP size_db();

/**
 * This function asks for how many simple integer values stored in the database
 * @method size_ints
 * @return [description]
 */
SEXP size_ints();

#ifdef __cplusplus
} // extern "C"
#endif

#endif // RCRD_STATS_STORE_H
