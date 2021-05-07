#include "stats_store.h"

#include "helper.h"

FILE *stats_file = NULL;

// Useful session counters
size_t bytes_read_session = 0;
size_t bytes_written_session = 0;

size_t bytes_serialized_session = 0;
size_t bytes_unserialized_session = 0;

// Useful lifetime counters // Not implemented yet
size_t bytes_read = 0;
size_t bytes_written = 0;

size_t bytes_serialized = 0;
size_t bytes_unserialized = 0;

// Database Statistics
// TODO: Give count, size, offset, i_size better names
size_t count = 0; // TODO: Consider: Maybe better to make this a double
size_t size = 0; // TODO: Consider: Maybe better to make this a double
size_t offset = 0;
size_t i_size = 0; // number of unique simple ints encountered
size_t d_size = 0; // number of unique simple dbls encountered

/**
 * Create stats.bin in the database
 * @method initiate_stats_store
 * @return R_NilValue on success, throw and error otherwise
 */
SEXP init_stats_store(SEXP stats) {
	stats_file = open_file(stats);

	bytes_read_session = 0;
	bytes_written_session = 0;

	bytes_serialized_session = 0;
	bytes_unserialized_session = 0;

	offset = 0;
	size = 0;
	count = 0;

	i_size = 0;
	d_size = 0;

	return R_NilValue;
}

/**
 * Loads stats.bin in the database
 * @method load_stats
 * @return R_NilValue on success, throw and error otherwise
 */
SEXP load_stats_store(SEXP stats) {
	init_stats_store(stats);

	read_n(stats_file, &bytes_read, sizeof(size_t));
	read_n(stats_file, &bytes_written, sizeof(size_t));

	read_n(stats_file, &bytes_serialized, sizeof(size_t));
	read_n(stats_file, &bytes_unserialized, sizeof(size_t));

	// Database Information
	read_n(stats_file, &offset, sizeof(size_t));
	read_n(stats_file, &size, sizeof(size_t));
	read_n(stats_file, &count, sizeof(int));
	read_n(stats_file, &i_size, sizeof(size_t));
	read_n(stats_file, &d_size, sizeof(size_t));

	return R_NilValue;
}

SEXP close_stats_store() {
	if (stats_file) {
		fseek(stats_file, 0, SEEK_SET);

		write_n(stats_file, &bytes_read, sizeof(size_t));
		bytes_read = 0;

		write_n(stats_file, &bytes_written, sizeof(size_t));
		bytes_written = 0;

		write_n(stats_file, &bytes_serialized, sizeof(size_t));
		bytes_serialized = 0;

		write_n(stats_file, &bytes_unserialized, sizeof(size_t));
		bytes_unserialized = 0;

		// Database Information
		write_n(stats_file, &offset, sizeof(size_t));
		offset = 0;

		write_n(stats_file, &size, sizeof(size_t));
		size = 0;

		write_n(stats_file, &count, sizeof(int));
		count = 0;

		write_n(stats_file, &i_size, sizeof(size_t));
		i_size = 0;

		write_n(stats_file, &d_size, sizeof(size_t));
		d_size = 0;

		// Safety byte
		write_n(stats_file, (void *) "\n", 1);
		fflush(stats_file);

		if (fclose(stats_file)) {
			Rf_error("Could not close the stats file.");
		}
		stats_file = NULL;
	}

	return R_NilValue;
}

SEXP print_report() {
	// Session
	fprintf(stderr, "Session Information:\n");
	fprintf(stderr, "  bytes read: %lu\n", bytes_read_session);
	fprintf(stderr, "  bytes written: %lu\n", bytes_written_session);
	fprintf(stderr, "  bytes serialized: %lu\n", bytes_serialized_session);
	fprintf(stderr, "  bytes unserialized: %lu\n", bytes_unserialized_session);
	fprintf(stderr, "\n");

	// Lifetime // Not implemented; just placeholder
	fprintf(stderr, "Database Lifetime Information (APPROXIMATE ONLY):\n");
	fprintf(stderr, "  bytes read: %lu\n", bytes_read);
	fprintf(stderr, "  bytes written: %lu\n", bytes_written);
	fprintf(stderr, "  bytes serialized: %lu\n", bytes_serialized);
	fprintf(stderr, "  bytes unserialized: %lu\n", bytes_unserialized);
	fprintf(stderr, "\n");

	// Database Statistics
	// TODO: Organize this better
	fprintf(stderr, "Database Statistics (NEED BETTER ORGANIZATION)\n");
	fprintf(stderr, "  Unique elements in the database: %lu\n", size);
	fprintf(stderr, "  Elements tried to be added to the database: %lu\n", count);
	fprintf(stderr, "  Bytes in the generic database: %lu\n", offset);
	fprintf(stderr, "  Elements in simple integer store: %lu\n", i_size);
	fprintf(stderr, "  Elements in simple double store: %lu\n", d_size);
	fprintf(stderr, "\n");

	return R_NilValue;
}

SEXP count_vals() {
	SEXP ret = PROTECT(allocVector(INTSXP, 1));
	INTEGER(ret)[0] = count;
	UNPROTECT(1);

	return ret;
}

SEXP size_db() {
	SEXP ret = PROTECT(allocVector(INTSXP, 1));
	INTEGER(ret)[0] = size;
	UNPROTECT(1);

	return ret;
}

SEXP size_ints() {
	SEXP ret = PROTECT(allocVector(INTSXP, 1));
	INTEGER(ret)[0] = i_size;
	UNPROTECT(1);

	return ret;
}
