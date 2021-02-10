#include "rcrd.h"


#include <R_ext/RS.h>


/**
 * This function creates or loads a collection of values.
 * @method record_init
 * @param  file_name   char* that should be the storage name; file for now
 * @return             SEXP that represent the collection
 */
SEXP record_init(SEXP file_name) {
	// Recover the input as a char*
	char* c_name = CHAR(STRING_ELT(file_name, 0));

	size_t l = strlen(c_name);

	// "Hello, [name]!" 8 characters outside of [name] and need + 1 for \0
	char* res = R_Calloc(9 + l, char);

	// Formate the return value
	strncpy(res, "Hello, ", 8); // Really 7; 8th is the '\0'
	strncpy(res + 7, c_name, l);
	res[9 + l - 2] = '!';
	res[9 + l - 1] = '\0';

	SEXP r_res = PROTECT(allocVector(STRSXP, 1));
	SET_STRING_ELT(r_res, 0, mkChar(res));
	UNPROTECT(1);

	return r_res;
}


/**
 * This function adds an R value to the specified storage
 * @method r2c
 * @param  r_object SEXP that represents an R object
 * @param  storage  SEXP that represents the storage
 * @return          SEXP that can act as a hash for the value in the store
 */
// SEXP r2c(SEXP r_object, SEXP storage) {
// 	char* res = "wrong result";
// 	SEXP r_res = PROTECT(allocVector(STRSXP, 1));
// 	SET_STRING_ELT(r_res, 0, mkChar(res));
// 	UNPROTECT(1);
//
// 	return r_res;
// }


/**
 * This function returns an R value from the specified storage based on hash
 * @method c2r
 * @param  hash    SEXP that can act as a hash for the value in the store
 * @param  storage SEXP that represents the storage
 * @return         SEXP that is the R value specified by the hash
 * TODO: HANDLE HASH COLLISION OR CREATE ANOTHER TYPE OF UNIQUE IDENTIFIER
 */
// SEXP c2r(SEXP hash, SEXP storage) {
// 	char* res = "wrong result";
// 	SEXP r_res = PROTECT(allocVector(STRSXP, 1));
// 	SET_STRING_ELT(r_res, 0, mkChar(res));
// 	UNPROTECT(1);
//
// 	return r_res;
// }
