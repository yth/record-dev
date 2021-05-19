#include <R.h>
#include <Rinternals.h>

#ifndef RCRD_SIMPLE_RAW_STORE_H
#define RCRD_SIMPLE_RAW_STORE_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Create the common raw storage
 * @method init_simple_raw_store
 * @param  file                 file name
 * @return                      R_NilValue on succcecss
 */
SEXP init_simple_raw_store(SEXP file);

/**
 * Loads ints.bin in the database
 * @method load_simple_raw_store
 * @return R_NilValue on success, throw and error otherwise
 */
SEXP load_simple_raw_store(SEXP raw);

/**
 * This function writes raw data to file and close the file.
 * @method close_simple_raw_store
 * @return R_NilValue on success
 */
SEXP close_simple_raw_store();

/**
 * This function assess if the input is a simple raw value
 * @method is_simple_raw
 * @param  SEXP          Any R value
 * @return               1 if it is a simple raw value, 0 otherwise
 */
int is_simple_raw(SEXP value);

/**
 * Adds an R scalar integer value to a separate  database.
 * @method add_simple_raw
 * @param  val           strictly a raw value
 * @return val
 */
SEXP add_simple_raw(SEXP val);

/**
 * This function asks if the C layer has seen a int in range [-5000, 5000]
 * @method have_seen_simple_raw
 * @param  val                 R value in form of SEXP
 * @return                     1 if the value has been encountered before, else 0
 */
int have_seen_simple_raw(SEXP val);

/**
 * This function samples from the simple ints that the database has.
 * @method get_simple_raw
 * @return SEXP             a sampled raw value
 */
SEXP get_simple_raw(int index);

#ifdef __cplusplus
} // extern "C"
#endif

#endif // RCRD_SIMPLE_RAW_STORE_H
