#include "helper.h"

#include "R_ext/Error.h"

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

	if (sizeof(size_t) != fread(&res, sizeof(size_t), 1, file)) {
		Rf_error("Could not read a full size_t.");
	}

	fseek(file, file_offset, SEEK_SET);

	return res;
}


#ifdef __cplusplus
} // extern "C"
#endif
