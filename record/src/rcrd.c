#include "rcrd.h"
#include "R_ext/Error.h"
#include "R_ext/Print.h"
#include "byte_vector.h"
#include <R_ext/RS.h>
#include <string.h>
#include <stdio.h> // FILE, fopen, close, fprintf


// #include "io_helper.c"


// #include <sys/mman.h> // mmap related facilities
// #include <stdint.h> // uint64_t
// #include <assert.h> // assert
//
// #include <sys/types.h> // open
// #include <sys/stat.h> // open
// #include <fcntl.h> // open
// #include <unistd.h> // close
// #include <assert.h> // assert



const char *db_file_name = "tmp.txt";

SEXP testthis(SEXP rstring){
  const char* cstring = CHAR(STRING_ELT(rstring, 0));

  // printf("%s", cstring);
  return R_NilValue;
}


/**
 * This function creates a database for a collection of values.
 * @method record_init
 * @param filename 
 * @return file pointer wrapped as a R external pointer
 */
SEXP record_init(SEXP filename) {
  const char* name = CHAR(STRING_ELT(filename, 0));
  // printf("%s", name);

	/* FILE *db = fopen(name, "w+"); */
  FILE *db = fopen(name, "w+");
	if (db == NULL) {
		printf("Could not start the database.");
	}
	return R_MakeExternalPtr(db, R_NilValue, R_NilValue);
}


/**
 * This function closes a database.
 * @method record_close
 * @param  file_ptr     wrapped FILE pointer
 */
SEXP record_close(SEXP file_ptr) {
/* 	FILE *file = R_ExternalPtrAddr(file_ptr); */
/* 	if (fclose(file)) { */
/* 		printf("Could not close the database."); */
/* 	} */
/* 	R_ClearExternalPtr(file_ptr); */
	return R_NilValue;
}


/**
 * This functions directly adds an R value to the specified storage.
 * @method r2cd
 * @param  r_object [description]
 * @param  storage  [description]
 * @return          [description]
 */
SEXP r2cd(SEXP r_string_object, SEXP file_ptr) {
	const char* c_string = CHAR(STRING_ELT(r_string_object, 0));
	FILE *file = R_ExternalPtrAddr(file_ptr);

	if (strlen(c_string) != fwrite(c_string, 1, strlen(c_string), file)) {
		return R_NilValue;
	}

	if (1 != fwrite("\n", 1, 1, file)) {
		return R_NilValue;
	}

	return r_string_object;
}

// R_serialize(value, R_NULL, R_FALSE, R_TRUE, R_NULL, R_NULL);

/**
 * This function adds an R value to the C layer.
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
// SEXP c2r(SEXP hash, SEXP storage) {
// 	const char* res = "wrong result";
// 	SEXP r_res = PROTECT(allocVector(STRSXP, 1));
// 	SET_STRING_ELT(r_res, 0, mkChar(res));
// 	UNPROTECT(1);
//
// 	return r_res;
// }
