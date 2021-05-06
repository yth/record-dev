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
// TODO: Give count, size, offset better names
size_t count = 0; // TODO: Consider: Maybe better to make this a double
size_t size = 0; // TODO: Consider: Maybe better to make this a double
size_t offset = 0;

/**
 * Create stats.bin in the database
 * @method initiate_stats_store
 * @return R_NilValue on success, throw and error otherwise
 */
SEXP initiate_stats_store(SEXP stats) {
	stats_file = open_file(stats);

	bytes_read_session = 0;
	bytes_written_session = 0;

	bytes_serialized_session = 0;
	bytes_unserialized_session = 0;

	return R_NilValue;
}

/**
 * Loads stats.bin in the database
 * @method load_stats
 * @return R_NilValue on success, throw and error otherwise
 */
SEXP load_stats_store(SEXP stats) {
	initiate_stats_store(stats);

	read_n(stats_file, &bytes_read, sizeof(size_t));
	read_n(stats_file, &bytes_written, sizeof(size_t));

	read_n(stats_file, &bytes_serialized, sizeof(size_t));
	read_n(stats_file, &bytes_unserialized, sizeof(size_t));

	return R_NilValue;
}

SEXP close_stats_store() {
	if (stats_file) {
		write_n(stats_file, &bytes_read, sizeof(size_t));
		bytes_read = 0;

		write_n(stats_file, &bytes_written, sizeof(size_t));
		bytes_written = 0;

		write_n(stats_file, &bytes_serialized, sizeof(size_t));
		bytes_serialized = 0;

		write_n(stats_file, &bytes_unserialized, sizeof(size_t));
		bytes_unserialized = 0;

		write_n(stats_file, (void *) "\n", 1);
		fflush(stats_file);

		if (fclose(stats_file)) {
			Rf_error("Could not close the stats file.");
		}
		stats_file = NULL;
	}

	return R_NilValue;
}

SEXP report() {
	// Session
	printf("Session Information:\n");
	printf("  bytes read: %lu\n", bytes_read_session);
	printf("  bytes written: %lu\n", bytes_written_session);
	printf("  bytes serialized: %lu\n", bytes_serialized_session);
	printf("  bytes unserialized: %lu\n", bytes_unserialized_session);
	printf("\n");

	// Lifetime // Not implemented; just placeholder
	printf("Database Lifetime Information (APPROXIMATE ONLY):\n");
	printf("  bytes read: %lu\n", bytes_read);
	printf("  bytes written: %lu\n", bytes_written);
	printf("  bytes serialized: %lu\n", bytes_serialized);
	printf("  bytes unserialized: %lu\n", bytes_unserialized);
	printf("\n");

	// Database Statistics
	printf("Database Statistics\n");
	printf("  Unique elements in the database: %lu\n", size);
	printf("  Elements tried to be added to the database: %lu\n", count);
	printf("  Bytes in the generic database: %lu\n", offset);

	return R_NilValue;
}
