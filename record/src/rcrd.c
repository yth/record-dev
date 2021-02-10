#include "rcrd.h"


#include <R_ext/RS.h>


#include <sys/mman.h> // mmap related facilities
#include <stdint.h> // uint64_t
#include <assert.h> // assert
#include <sys/types.h> // open
#include <sys/stat.h> // open
#include <fcntl.h> // open
#include <unistd.h>


/**
 * This function creates or loads a storage for a collection of values.
 * @method record_init
 * @param  file_name   char* that should be the storage name; file for now
 * @return             SEXP that represent the collection
 */
SEXP record_init(SEXP path_name) {
	// TODO: Raw file for now, in the future should be handled by an object.
	const char* path = CHAR(STRING_ELT(path_name, 0));
	int fd = open(path, O_RDWR);
	if (fd < 0) {
		fd = open("./tmp.txt", O_CREAT | O_RDWR, S_IRWXU);
		assert(fd > -1);
	}

	char* memory_file = (char*) mmap(NULL, // Don't care where
									 1024 * 1024, // 1 MB
									 PROT_READ | PROT_WRITE, // Read/writable
									 MAP_SHARED, // Make changes visible
									 fd, // the file descriptor we just created
									 0); // Start from the start of the file

	// uint64_t memory_ptr = reinterpret_cast<uint64_t>(memory_file);
	uint64_t memory_ptr = (uint64_t) memory_file;
	SEXP r_res = PROTECT(allocVector(INTSXP, 1));
	SET_INTEGER_ELT(r_res, 0, memory_ptr);
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
SEXP r2c(SEXP r_object, SEXP storage) {
	const char* res = "wrong result";
	SEXP r_res = PROTECT(allocVector(STRSXP, 1));
	SET_STRING_ELT(r_res, 0, mkChar(res));
	UNPROTECT(1);

	return r_res;
}


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
