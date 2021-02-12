#include "rcrd.h"
#include "R_ext/Error.h"
#include "R_ext/Print.h"


#include <R_ext/RS.h>


#include <sys/mman.h> // mmap related facilities
#include <stdint.h> // uint64_t
#include <assert.h> // assert

#include <sys/types.h> // open
#include <sys/stat.h> // open
#include <fcntl.h> // open
#include <unistd.h> // close
#include <assert.h> // assert

#include <stdio.h> // printf



// Global Variable
char* STORAGE;
int fd;


/**
 * This function creates or loads a storage for a collection of values.
 * @method record_init
 * @param  file_name   char* that should be the storage name; file for now
 * @return             SEXP that represent the collection
 */
SEXP record_init(SEXP path_name) {
	// TODO: Raw file for now, in the future should be handled by an object.
	// const char* path = CHAR(STRING_ELT(path_name, 0));
	// fd = open(path, O_RDWR);
	// if (fd < 0) {
	// 	fd = open("./tmp.txt", O_CREAT | O_RDWR, S_IRWXU); // No O_TMPFILE
	// 	assert(fd > -1);
	// }

	// No physical page error
	// STORAGE = (char*) mmap(NULL, // Don't care where
	// 						1024 * 1024, // 1 MB
	// 						PROT_READ | PROT_WRITE, // Read/writable
	// 						MAP_SHARED, // Make changes visible
	// 						fd, // the file descriptor we just created
	// 						0); // Start from the start of the file

	// Simplest test
	STORAGE = (char*) malloc(1024 * 1024);
	printf("Storage: %p\n", STORAGE);
	//                   12345678911234
	const char* cster = "random string";
	memcpy(STORAGE, cster, 14);
	STORAGE[14] = '\0';

	// uint64_t memory_ptr = reinterpret_cast<uint64_t>(memory_file);
	uint64_t memory_ptr = (uint64_t) STORAGE;
	SEXP r_res = PROTECT(allocVector(REALSXP, 1));
	SET_REAL_ELT(r_res, 0, memory_ptr);
	UNPROTECT(1);

	// return Rf_ScalarInteger(memory_ptr); // Try this later
	return r_res;
}


/**
 * This function adds an R value to the specified storage
 * @method r2c
 * @param  r_object SEXP that represents an R object
 * @param  storage  SEXP that represents the storage
 * @return          SEXP that can act as a hash for the value in the store
 */
SEXP r2c(SEXP r_string_object, SEXP storage) {
	// TODO: Generalize what kind of R objects can be passed in

	// TODO: Find a way to not clobbering other values
	//		 Solution is probably creating an object that manages storage

		// uint64_t memory_ptr = (uint64_t) REAL0(storage);
	// char* memory_file = STORAGE; // (char*) memory_ptr;

	// int i = 0;
	// while (c_string[i] != '\0') {
	// 	memory_file[i] = c_string[i];
	// 	++i;
	// }
	// assert(i > 0);
	// free(STORAGE);

	// int fd2 = open("tmp.txt", O_CREAT | O_APPEND | O_RDWR, (mode_t) 0777);
	// if (fd2 < 0) {
	// 	perror("Could not open/create a file");
	// }

	const char* c_string = CHAR(STRING_ELT(r_string_object, 0));
	printf("c_string: %s\n", c_string);
	FILE* f = fopen("tmp.txt", "rw");
	if (f == NULL) {
	  Rf_error("Unable to open tmp.txt\n");
	}
	fprintf(f, "%s", c_string);
	fclose(f);

	// printf("printf: %s\n", c_string);
	// for (int j = 0; c_string[j] != '\0'; ++j) {
	// 	write(fd2, c_string + j, 1);
	// 	write(0, c_string + j, 1);
	// 	write(1, c_string + j, 1);
	// 	write(2, c_string + j, 1);
	// }

	// Write test
	// const char* cster = "random string";
	// printf("\nprintf: %s\n", STORAGE);
	// for (int j = 0; STORAGE + j != '\0'; ++j) {
	// 	write(fd2, STORAGE + j, 1);
	// 	write(0, STORAGE + j, 1);
	// 	write(1, STORAGE + j, 1);
	// 	write(2, STORAGE + j, 1);
	// }

	// if (close(fd2) < 0) {
	// 	perror("Could not close file descriptor");
	// }

	// // Don't unmapp in future
	// if (msync(memory_file, 100, MS_SYNC) < 0) {
	// 	perror("Could not write to disk");
	// } // Try MS_ASYNC in future
	//
	// if (munmap(memory_file, 1024 * 1024) < 0) {
	// 	perror("Could not unmap");
	// }

	SEXP r_res = PROTECT(allocVector(STRSXP, 1));
	SET_STRING_ELT(r_res, 0, mkChar(c_string));
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
