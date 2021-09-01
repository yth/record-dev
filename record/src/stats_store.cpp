#include "stats_store.h"

#include "helper.h"

FILE *stats_file = NULL;

// Database Statistics
size_t count = 0; // TODO: Consider: Maybe better to make this a double
size_t size = 0; // TODO: Consider: Maybe better to make this a double


size_t i_size = 0; // number of unique ints encountered
size_t i_offset = 0; // number of bytes in the int store
size_t s_i_size = 0; // number of unique simple ints encountered

size_t d_size = 0; // number of unique dbls encountered
size_t d_offset = 0; // number of bytes in the dbl store
size_t s_d_size = 0; // number of unique simple dbls encountered

size_t s_r_size = 0; // number of unique simple raws encountered
size_t s_s_size = 0; // number of unique simple strs encountered
size_t s_s_offset = 0; // number of bytes in simple str store


size_t g_size = 0; // number of unique generic values encountered
size_t g_offset = 0; // number of bytes in generic store

size_t r_count = 0; // number of raws in generic store
size_t s_count = 0; // number of strs in generic store

size_t m_count = 0; // number of times this database has merged another one

// Useful session counters
size_t bytes_read_session = 0;
size_t bytes_written_session = 0;

size_t bytes_serialized_session = 0;
size_t bytes_unserialized_session = 0;

// Useful lifetime counters
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
	i_offset = 0;
	s_i_size = 0;

	d_size = 0;
	d_offset = 0;
	s_d_size = 0;

	s_r_size = 0;
	s_s_size = 0;
	s_s_offset = 0;
	g_size = 0;
	g_offset = 0;

	r_count = 0;
	s_count = 0;

	m_count = 0;

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
	read_n(stats_file, &i_offset, sizeof(size_t));
	read_n(stats_file, &s_i_size, sizeof(size_t));

	read_n(stats_file, &d_size, sizeof(size_t));
	read_n(stats_file, &d_offset, sizeof(size_t));
	read_n(stats_file, &s_d_size, sizeof(size_t));

	read_n(stats_file, &s_r_size, sizeof(size_t));
	read_n(stats_file, &s_s_size, sizeof(size_t));
	read_n(stats_file, &s_s_offset, sizeof(size_t));
	read_n(stats_file, &g_size, sizeof(size_t));
	read_n(stats_file, &g_offset, sizeof(size_t));

	read_n(stats_file, &r_count, sizeof(size_t));
	read_n(stats_file, &s_count, sizeof(size_t));

	read_n(stats_file, &m_count, sizeof(size_t));

	// Performance Information
	read_n(stats_file, &bytes_serialized, sizeof(size_t));
	read_n(stats_file, &bytes_unserialized, sizeof(size_t));

	read_n(stats_file, &bytes_read, sizeof(size_t));
	bytes_read += bytes_read_session - sizeof(size_t);

	read_n(stats_file, &bytes_written, sizeof(size_t));

	return R_NilValue;
}

/**
 * This functions adjust information in the stats store based on the merge.
 * @method merge_stats_store
 * @return R_NilValue on success
 */
SEXP merge_stats_store() {
	m_count += 1;

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

		write_n(stats_file, &i_offset, sizeof(size_t));
		i_offset = 0;

		write_n(stats_file, &s_i_size, sizeof(size_t));
		s_i_size = 0;

		write_n(stats_file, &d_size, sizeof(size_t));
		d_size = 0;

		write_n(stats_file, &d_offset, sizeof(size_t));
		d_offset = 0;

		write_n(stats_file, &s_d_size, sizeof(size_t));
		s_d_size = 0;

		write_n(stats_file, &s_r_size, sizeof(size_t));
		s_r_size = 0;

		write_n(stats_file, &s_s_size, sizeof(size_t));
		s_s_size = 0;

		write_n(stats_file, &s_s_offset, sizeof(size_t));
		s_s_offset = 0;

		write_n(stats_file, &g_size, sizeof(size_t));
		g_size = 0;

		write_n(stats_file, &g_offset, sizeof(size_t));
		g_offset = 0;

		write_n(stats_file, &r_count, sizeof(size_t));
		r_count = 0;

		write_n(stats_file, &s_count, sizeof(size_t));
		s_count = 0;

		write_n(stats_file, &m_count, sizeof(size_t));
		m_count = 0;

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

	// Database Statistics
	fprintf(stderr, "Database Statistics\n");
	fprintf(stderr, "  Unique elements in the database: %lu\n", size);
	fprintf(stderr, "  Number of times add_val was called: %lu\n", count);
	fprintf(stderr, "  Number of times this database has merged another: %lu\n", m_count);
	fprintf(stderr, "\n");

	fprintf(stderr, "Database Stores Statistics\n");
	fprintf(stderr, "  Database Specialty Stores Statistics\n");
	fprintf(stderr, "    Elements in simple integer store: %lu\n", s_i_size);
	fprintf(stderr, "    Elements in int store: %lu\n", i_size);
	fprintf(stderr, "    Elements in dbl store: %lu\n", d_size);
	fprintf(stderr, "    Elements in simple double store: %lu\n", s_d_size);
	fprintf(stderr, "    Elements in simple raw store: %lu\n", s_r_size);
	fprintf(stderr, "    Elements in simple string store: %lu\n", s_s_size);

	fprintf(stderr, "  Database Generic Store Statistics\n");
	fprintf(stderr, "    Elements in generic store: %lu\n", g_size);
	fprintf(stderr, "      Raws in generic store: (%lu/%lu)\n",
					r_count, g_size);
	fprintf(stderr, "      Strings in generic store: (%lu/%lu)\n",
					s_count, g_size);
	fprintf(stderr, "      Other values in generic store: (%lu/%lu)\n",
					g_size - r_count - s_count, g_size);

	fprintf(stderr, "    Bytes in the generic database: %lu\n", g_offset);
	fprintf(stderr, "\n");

	return R_NilValue;
}

/**
 * This function asks for how many times C add_val has been called.
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
	INTEGER(ret)[0] = s_i_size;
	UNPROTECT(1);

	return ret;
}
