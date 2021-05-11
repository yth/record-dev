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
 * This functions adds an R value to the database.
 * @method add_val
 * @param  val      R value in form of SEXP
 * @return          val on success
 */
SEXP add_val(SEXP val);

/**
 * This function asks if the C layer has seen a R value.
 * This function is mainly used for testing.
 * @method have_seen
 * @param  val       R value in form of SEXP
 * @return           R value of True or False as a SEXP
 */
SEXP have_seen(SEXP val);

/**
 * This function returns a random value from the database
 * @method get_random_val
 * @return R value in form of SEXP from the database
 */
SEXP sample_val();

#ifdef __cplusplus
} // extern "C"
#endif

#endif // RCRD_RCRD_H

