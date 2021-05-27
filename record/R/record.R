## Primary Functionality

#' @export
# Open database specified by db
# If create is TRUE, then create the database.
# If create is FALSE, then load the database.
open_db <- function(db = "db", create = FALSE) {
	if (dir.exists(db)){
		if (create) {
			stop(paste0(db, " already exists."))
		} else if (!file.exists(paste0(db, "/generics.bin"))) {
			stop(paste0(db, " is not a database."))
		} else {# valid database
			# This must be called first
			.Call(RCRD_setup)

			# This must be called second
			.Call(RCRD_load_stats_store, paste0(db, "/stats.bin"))

			# Load specialty stores
			.Call(RCRD_load_simple_int_store, paste0(db, "/ints.bin"))
			.Call(RCRD_load_simple_dbl_store, paste0(db, "/dbls.bin"))
			.Call(RCRD_load_simple_raw_store, paste0(db, "/raws.bin"))

			# Load specialty store with index
			.Call(RCRD_load_simple_str_index, paste0(db, "/str_index.bin"))
			.Call(RCRD_load_simple_str_store, paste0(db, "/strs.bin"))

			# Load generic store
			.Call(RCRD_load_generic_index, paste0(db, "/generic_index.bin"))
			.Call(RCRD_load_generic_store, paste0(db, "/generics.bin"))
		}
	} else {
		if (!create) {
			stop(paste0(db, " does not exist."))
		} else {
			dir.create(db, recursive = TRUE)

			# Create the stats store file
			stats = paste0(db, "/stats.bin")

			# Create the specialty store files
			ints = paste0(db, "/ints.bin")
			dbls = paste0(db, "/dbls.bin")
			raws = paste0(db, "/raws.bin")

			# Create specialty store with index
			strs = paste0(db, "/strs.bin")
			str_index = paste0(db, "/str_index.bin")

			# Create the generic store files
			generics = paste0(db, "/generics.bin")
			generic_index = paste0(db, "/generic_index.bin")

			file.create(ints,
						dbls,
						raws,
						strs,
						str_index,
						generics,
						generic_index,
						stats,
						showWarnings = TRUE)

			# This must be called first
			.Call(RCRD_setup)

			# This must be called second
			.Call(RCRD_init_stats_store, stats)

			# Initialize specialty stores
			.Call(RCRD_init_simple_int_store, ints)
			.Call(RCRD_init_simple_dbl_store, dbls)
			.Call(RCRD_init_simple_raw_store, raws)

			# Initialize specialty store with index
			.Call(RCRD_init_simple_str_index, str_index)
			.Call(RCRD_init_simple_str_store, strs)

			# Initialize generic store
			.Call(RCRD_init_generic_index, generic_index)
			.Call(RCRD_init_generic_store, generics)
		}
	}
}

#' @export
close_db <- function() {
	# Close generic store
	.Call(RCRD_close_generic_index)
	.Call(RCRD_close_generic_store)

	# Close specialty store
	.Call(RCRD_close_simple_int_store)
	.Call(RCRD_close_simple_dbl_store)
	.Call(RCRD_close_simple_raw_store)

	# Close specialty store with index
	.Call(RCRD_close_simple_str_index)
	.Call(RCRD_close_simple_str_store)

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
sample_val <- function() {
	.Call(RCRD_sample_val)
}

## Testing/Information Gathering Related Functionality

#' @export
have_seen <- function(val) {44
	.Call(RCRD_have_seen, val)
}

#' @export
report <- function() {
	.Call(RCRD_print_report)
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
