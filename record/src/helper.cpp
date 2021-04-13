#include "helper.h"

#include <stdlib.h>

#include <R_ext/RS.h>

#ifdef __cplusplus
extern "C" {
#endif


// Writes a size_t to file
void write_size_t(FILE* file, size_t val) {
	if (fwrite(&val, sizeof(size_t), 1, file) != 1) {
		perror("Could not write a full size_t");
	}
	return;
}


// Read a size_t from file starting at the indicated offset
size_t read_size_t(FILE* file, size_t file_offset, size_t value_offset) {
	size_t res = 0;

	fseek(file, value_offset, SEEK_SET);

	if (fread(&res, sizeof(size_t), 1, file) != sizeof(size_t)) {
		perror("Could not read a full size_t");
	}

	fseek(file, file_offset, SEEK_SET);

	return res;
}


// Read n bytes starting at an offset from a file_ptr
// Heap allocates the result. User is responsible for freeing it.
char *read_n(FILE* file, size_t file_offset, size_t value_offset, size_t n) {
	char *buf = (char *) malloc(n);
	if (!buf) {
		fprintf(stderr, "Attempt to malloc %lu bytes: ", n);
		perror("read_n");
	}

	fseek(file, value_offset, SEEK_SET);

	// TODO: Add loop
	if (fread(buf, 1, n, file) != n) {
		free(buf);
		perror("Could not read a full n bytes");
	}

	fseek(file, file_offset, SEEK_SET);

	return buf;
}

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
void readn(FILE* file, void *buf, size_t n) {
	size_t m = fread(buf, 1, n, file);
	if (m != n) {
		fprintf(stderr, "readn only read %lu out of %lu bytes\n", m, n);
		perror("readn");
		abort();
	}
}

void writen(FILE* file, void *buf, size_t n) {
	size_t m = fwrite(buf, n, 1, file);
	if (m != n) {
		fprintf(stderr, "writen only wrote %lu out of %lu bytes\n", m, n);
		perror("writen");
		abort();
	}
}


#ifdef __cplusplus
} // extern "C"
#endif
