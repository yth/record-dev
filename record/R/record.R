#' @export
# Open database specified by db
# If create is TRUE, then create the database.
# If create is FALSE, then load the database.
open_db <- function(db = "db", create = FALSE) {
	if (dir.exists(db)){
		if (create) {
			stop(paste0(db, " already exists."))
		} else if (!file.exists(paste0(db, "/gbov.bin"))) {
			stop(paste0(db, " is not a database."))
		} else {# valid database
			# This must be called first
			.Call(RCRD_setup)

			# This must be called second
			.Call(RCRD_load_stats_store, paste0(db, "/stats.bin"))

			# Load specialty stores
			.Call(RCRD_load_simple_int_store, paste0(db, "/ints.bin"))
			.Call(RCRD_load_simple_dbl_store, paste0(db, "/dbls.bin"))

			# Load generic store
			.Call(RCRD_load_indices, paste0(db, "/indices.bin"))
			.Call(RCRD_load_gbov, paste0(db, "/gbov.bin"))
		}
	} else {
		if (!create) {
			stop(paste0(db, " does not exist."))
		} else {
			dir.create(db, recursive = TRUE)

			stats = paste0(db, "/stats.bin")
			ints = paste0(db, "/ints.bin")
			dbls = paste0(db, "/dbls.bin")

			gbov = paste0(db, "/gbov.bin")
			indices = paste0(db, "/indices.bin")

			file.create(ints, dbls, gbov, indices, stats, showWarnings = TRUE)

			# This must be called first
			.Call(RCRD_setup)

			# This must be called second
			.Call(RCRD_init_stats_store, stats)

			# Initialize specialty stores
			.Call(RCRD_init_simple_int_store, ints)
			.Call(RCRD_init_simple_dbl_store, dbls)

			# Initialize generic store
			.Call(RCRD_create_indices, indices)
			.Call(RCRD_create_gbov, gbov)
		}
	}
}

#' @export
close_db <- function(file) {
	# Close generic store
	.Call(RCRD_close_indices)
	.Call(RCRD_close_gbov)

	# Close specialty store
	.Call(RCRD_close_simple_int_store)
	.Call(RCRD_close_simple_dbl_store)

	# This must be called second to last
	.Call(RCRD_close_stats_store)

	# This must be called last
	.Call(RCRD_teardown)
}

#' @export
add_val <- function(val) {
	.Call(RCRD_add_val, val)
}

#' @export
have_seen <- function(val) {44
	.Call(RCRD_have_seen, val)
}

#' @export
count_vals <- function() {
	.Call(RCRD_count_vals)
}

#' @export
size_db <- function() {
	.Call(RCRD_size_db)
}

#' @export
size_ints <- function() {
	.Call(RCRD_size_ints)
}

#' @export
get_vals <- function(from, to) {
	.Call(RCRD_read_vals, from, to)
}

#' @export
get_random_val <- function() {
	.Call(RCRD_sample_val)
}

#' @export
report <- function() {
	.Call(RCRD_print_report)
}
