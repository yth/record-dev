#include "hello.h"


#include <R_ext/RS.h>


#include <string.h>


SEXP hello(SEXP name) {
	// Recover the input as a char*
	const char* c_name = CHAR(STRING_ELT(name, 0));

	size_t l = strlen(c_name);

	// "Hello, [name]!" 8 characters outside of [name] and need + 1 for \0
	char* res = Calloc(9 + l, char);

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
