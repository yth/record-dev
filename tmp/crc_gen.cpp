#include <stdio.h> // fopen, fprintf, fclose
#include <cstdlib> // srand, rand

int main(int argc, char** argv) {
	srand(42);
	FILE *file = fopen("crc_table.h", "w+");

	// Start of file
	fprintf(file, "uint32_t crc_table [256] = {\n");

	fprintf(file, "\t");
	for (int i = 0; i < 256; ++i) {
		int rand_int = rand();
		fprintf(file, "%#010X", rand_int);

		if (i != 255) {
			fprintf(file, ", ");
			if (i % 4 == 3) {
				fprintf(file, "\n\t");
			}
		} else {
			fprintf(file, "\n");
		}
	}


	fprintf(file, "};\n");
	// End of file

	fclose(file);

	return 0;
}