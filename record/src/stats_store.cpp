#include "stats_store.h"

#include "helper.h"

FILE *stats_file = NULL;

// Database Statistics
// TODO: Give count, size, g_offset, i_size better names
size_t count = 0; // TODO: Consider: Maybe better to make this a double
size_t size = 0; // TODO: Consider: Maybe better to make this a double
size_t i_size = 0; // number of unique simple ints encountered
size_t d_size = 0; // number of unique simple dbls encountered
size_t r_size = 0; // number of unique simple raws encountered
size_t s_size = 0; // number of unique simple strs encountered
size_t s_offset = 0; // number of bytes in simple str store
size_t g_size = 0; // number of unique generic values encountered
size_t g_offset = 0; // number of bytes in generic store

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

/**
 * Load/create a brand new stats store.
 * @method init_stats_store
 * @return R_NilValue on success, throw and error otherwise
 */
SEXP init_stats_store(SEXP stats) {
	stats_file = open_file(stats);

	// Database Information
	size = 0;
	count = 0;

	i_size = 0;
	d_size = 0;
	r_size = 0;
	s_size = 0;
	s_offset = 0;
	g_size = 0;
	g_offset = 0;

	// Performance Information
	bytes_read_session = 0;
	bytes_written_session = 0;

	bytes_serialized_session = 0;
	bytes_unserialized_session = 0;

	return R_NilValue;
}

/**
 * Load an existing stats store.
 * @method load_stats_store
 * @return R_NilValue on success, throw and error otherwise
 */
SEXP load_stats_store(SEXP stats) {
	stats_file = open_file(stats);

	// Database Information
	read_n(stats_file, &size, sizeof(size_t));
	read_n(stats_file, &count, sizeof(int));
	read_n(stats_file, &i_size, sizeof(size_t));
	read_n(stats_file, &d_size, sizeof(size_t));
	read_n(stats_file, &r_size, sizeof(size_t));
	read_n(stats_file, &s_size, sizeof(size_t));
	read_n(stats_file, &s_offset, sizeof(size_t));
	read_n(stats_file, &g_size, sizeof(size_t));
	read_n(stats_file, &g_offset, sizeof(size_t));

	// Performance Information
	read_n(stats_file, &bytes_serialized, sizeof(size_t));
	read_n(stats_file, &bytes_unserialized, sizeof(size_t));

	read_n(stats_file, &bytes_read, sizeof(size_t));
	bytes_read += bytes_read_session - sizeof(size_t);

	read_n(stats_file, &bytes_written, sizeof(size_t));

	return R_NilValue;
}

/**
 * This functions writes database statistics to file and closes the file.
 * @method close_stats_store
 * @return R_NilValue on success
 */
SEXP close_stats_store() {
	if (stats_file) {
		fseek(stats_file, 0, SEEK_SET);

		// Database Information
		write_n(stats_file, &size, sizeof(size_t));
		size = 0;

		write_n(stats_file, &count, sizeof(int));
		count = 0;

		write_n(stats_file, &i_size, sizeof(size_t));
		i_size = 0;

		write_n(stats_file, &d_size, sizeof(size_t));
		d_size = 0;

		write_n(stats_file, &r_size, sizeof(size_t));
		r_size = 0;

		write_n(stats_file, &s_size, sizeof(size_t));
		s_size = 0;

		write_n(stats_file, &s_offset, sizeof(size_t));
		s_offset = 0;

		write_n(stats_file, &g_size, sizeof(size_t));
		g_size = 0;

		write_n(stats_file, &g_offset, sizeof(size_t));
		g_offset = 0;

		// Performance Information
		write_n(stats_file, &bytes_serialized, sizeof(size_t));
		bytes_serialized = 0;

		write_n(stats_file, &bytes_unserialized, sizeof(size_t));
		bytes_unserialized = 0;

		write_n(stats_file, &bytes_read, sizeof(size_t));
		bytes_read = 0;

		// Record the bytes written for write_n and close_file
		bytes_written += sizeof(size_t) + 1;
		write_n(stats_file, &bytes_written, sizeof(size_t));
		// bytes_written = 0; // Must be zeroed out later

		close_file(&stats_file);
		bytes_written = 0; // close_file cause 1 bytes to be written

		// Zero out session information
		bytes_read_session = 0;
		bytes_written_session = 0;

		bytes_serialized_session = 0;
		bytes_unserialized_session = 0;
	}

	return R_NilValue;
}

/**
 * Report database statistics
 * @method print_report
 */
SEXP print_report() {
	// Session
	fprintf(stderr, "Session Information:\n");
	fprintf(stderr, "  bytes read: %lu\n", bytes_read_session);
	fprintf(stderr, "  bytes written: %lu\n", bytes_written_session);
	fprintf(stderr, "  bytes serialized: %lu\n", bytes_serialized_session);
	fprintf(stderr, "  bytes unserialized: %lu\n", bytes_unserialized_session);
	fprintf(stderr, "\n");

	// Lifetime
	fprintf(stderr, "Database Lifetime Information:\n");
	fprintf(stderr, "  bytes read: %lu\n", bytes_read);
	fprintf(stderr, "  bytes written: %lu\n", bytes_written);
	fprintf(stderr, "  bytes serialized: %lu\n", bytes_serialized);
	fprintf(stderr, "  bytes unserialized: %lu\n", bytes_unserialized);
	fprintf(stderr, "\n");

	fprintf(stderr, "Database Lifetime Performance Information:\n");
	fprintf(stderr, "  Number of times add_val was called: %lu\n", count);
	fprintf(stderr, "\n");

	// Database Statistics
	// TODO: Organize this better
	fprintf(stderr, "Database Statistics (NEED BETTER ORGANIZATION)\n");
	fprintf(stderr, "  Unique elements in the database: %lu\n", size);
	fprintf(stderr, "  Elements tried to be added to the database: %lu\n", count);
	fprintf(stderr, "  Elements in simple integer store: %lu\n", i_size);
	fprintf(stderr, "  Elements in simple double store: %lu\n", d_size);
	fprintf(stderr, "  Elements in simple raw store: %lu\n", r_size);
	fprintf(stderr, "  Elements in simple string store: %lu\n", s_size);
	fprintf(stderr, "  Elements in generic store: %lu\n", g_size);
	fprintf(stderr, "  Bytes in the generic database: %lu\n", g_offset);
	fprintf(stderr, "\n");

	return R_NilValue;
}

/**
 * This function asks for how many R values the C add_val has seen.
 * @method count_vals
 * @return number of times add_val was called
 */
SEXP count_vals() {
	SEXP ret = PROTECT(allocVector(INTSXP, 1));
	INTEGER(ret)[0] = count;
	UNPROTECT(1);

	return ret;
}

/**
 * This function asks for how many values are stored in the database
 * @method size_db
 * @return Non-zero numeric R value in form of a SEXP
 */
SEXP size_db() {
	SEXP ret = PROTECT(allocVector(INTSXP, 1));
	INTEGER(ret)[0] = size;
	UNPROTECT(1);

	return ret;
}

/**
 * This function asks for how many simple integer values stored in the database
 * @method size_ints
 * @return Non-zero numeric R value in form of a SEXP
 */
SEXP size_ints() {
	SEXP ret = PROTECT(allocVector(INTSXP, 1));
	INTEGER(ret)[0] = i_size;
	UNPROTECT(1);

	return ret;
}
