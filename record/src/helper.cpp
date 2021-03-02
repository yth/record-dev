#include <cstdio>

#ifdef __cplusplus
extern "C" {
#endif


// Writes a size_t to file
void write_size_t(FILE* file, size_t val) {
	return;
}


// Read a size_t from file starting at the indicated offset
size_t read_size_t(FILE* file, size_t file_offset, size_t value_offset) {
	return 1;
}


#ifdef __cplusplus
} // extern "C"
#endif
