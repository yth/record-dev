#include "helper.h"

#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

// Useful session counters
extern size_t bytes_read_session;
extern size_t bytes_written_session;

// Useful procecss counters
extern size_t bytes_read_process;
extern size_t bytes_written_process;

// Useful lifetime counters
extern size_t bytes_read;
extern size_t bytes_written;

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
	bytes_read_session += n;
	bytes_read_process += n;
	bytes_read += n;
}

// Help write n bytes into file and check error messages
// Doesn't restore file offset
void write_n(FILE* file, void *buf, size_t n) {
	if (!(fwrite(buf, n, 1, file))) {
		perror("write_n");
		abort();
	}
	bytes_written_session += n;
	bytes_written_process += n;
	bytes_written += n;
}

#ifdef __cplusplus
} // extern "C"
#endif
