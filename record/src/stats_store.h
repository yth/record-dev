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
SEXP initiate_stats_store(SEXP stats);

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
SEXP report();

#ifdef __cplusplus
} // extern "C"
#endif

#endif // RCRD_STATS_STORE_H
