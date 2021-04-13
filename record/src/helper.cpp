#include "helper.h"

#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

FILE *open_file(SEXP filename) {
	const char* cfilename = CHAR(STRING_ELT(filename, 0));
	FILE *fp = fopen(cfilename, "r+");
	if (fp == NULL) {
		Rf_error("%s does not exist", cfilename);
	}
	return fp;
}

// Help read n bytes into buffer and check error messages
// Doesn't restore file offset
void read_n(FILE* file, void *buf, size_t n) {
	if (!(fread(buf, n, 1, file))) {
		perror("read_n");
		abort();
	}
}

// Help write n bytes into file and check error messages
// Doesn't restore file offset
void write_n(FILE* file, void *buf, size_t n) {
	if (!(fwrite(buf, n, 1, file))) {
		perror("write_n");
		abort();
	}
}

#ifdef __cplusplus
} // extern "C"
#endif
