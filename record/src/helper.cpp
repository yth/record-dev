#include "helper.h"

#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif


// Writes a size_t to file
void write_size_t(FILE* file, size_t val) {
	fwrite(&val, sizeof(size_t), 1, file);
	return;
}


// Read a size_t from file starting at the indicated offset
size_t read_size_t(FILE* file, size_t file_offset, size_t value_offset) {
	size_t res = 0;

	fseek(file, value_offset, SEEK_SET);

	if (fread(&res, sizeof(size_t), 1, file) != sizeof(size_t)) {
		perror("Could not read a full size_t.");
	}

	fseek(file, file_offset, SEEK_SET);

	return res;
}


// Read n bytes starting at an offset from a file_ptr
// Heap allocates the result. User is responsible for freeing it.
char *read_n(FILE* file, size_t file_offset, size_t value_offset, size_t n) {
	char *buf = (char *) malloc(n);
	if (!buf) {
		perror("Could not malloc.");
	}

	fseek(file, value_offset, SEEK_SET);

	if (fread(buf, 1, n, file) != n) {
		free(buf);
		perror("Could not read a full n bytes.");
	}

	fseek(file, file_offset, SEEK_SET);

	return buf;
}

#ifdef __cplusplus
} // extern "C"
#endif
