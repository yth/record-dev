## Primary Functionality

#' @export
# Open database specified by db
# If create is TRUE, then create the database.
# If create is FALSE, then load the database.
open_db <- function(db = "db", create = FALSE) {
	if (dir.exists(db)) {
		if (create) {
			stop(paste0(db, " already exists."))
		} else if (!file.exists(paste0(db, "/generics.bin"))) {
			# TODO: Improve ways to see if we are working with a record db
			stop(paste0(db, " is not a database."))
		} else {# valid database
			# This must be called first
			.Call(RCRD_setup)

			# This must be called second
			.Call(RCRD_load_stats_store, paste0(db, "/stats.bin"))

			# Load Int Stores
			.Call(RCRD_load_int_index, paste0(db, "/int_index.bin"))
			.Call(RCRD_load_int_store, paste0(db, "/ints.bin"))
			.Call(RCRD_load_simple_int_store, paste0(db, "/s_ints.bin"))

			# Load specialty stores
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

			# Create Int Stores
			ints = paste0(db, "/ints.bin")
			int_index = paste0(db, "/int_index.bin")
			s_ints = paste0(db, "/s_ints.bin")

			# Create the specialty store files
			dbls = paste0(db, "/dbls.bin")
			raws = paste0(db, "/raws.bin")

			# Create specialty store with index
			strs = paste0(db, "/strs.bin")
			str_index = paste0(db, "/str_index.bin")

			# Create the generic store files
			generics = paste0(db, "/generics.bin")
			generic_index = paste0(db, "/generic_index.bin")

			file.create(ints,
						int_index,
						s_ints,
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

			# Initialize Int Stores
			.Call(RCRD_init_int_index, int_index)
			.Call(RCRD_init_int_store, ints)
			.Call(RCRD_init_simple_int_store, s_ints)


			# Initialize specialty stores
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

	# Close Int Stores
	.Call(RCRD_close_int_index)
	.Call(RCRD_close_int_store)
	.Call(RCRD_close_simple_int_store)

	# Close specialty store
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
merge_db <- function(other_db = "db") {
	# TODO: Create a better way to see if we are working with a record db
	# TODO: Create a better way to see if we are working with another record db
	if (dir.exists(other_db) &&
		file.exists(paste0(other_db, "/generics.bin"))) {

		ints = paste0(other_db, "/ints.bin")
		int_index = paste0(other_db, "/int_index.bin")
		s_ints = paste0(other_db, "/s_ints.bin")

		dbls = paste0(other_db, "/dbls.bin")
		raws = paste0(other_db, "/raws.bin")
		strs = paste0(other_db, "/strs.bin")
		str_index = paste0(other_db, "/str_index.bin")
		generics = paste0(other_db, "/generics.bin")
		generic_index = paste0(other_db, "/generic_index.bin")
		# stats = paste0(other_db, "/stats.bin") # No needed right now

		# Merge Ints
		.Call(RCRD_merge_int_store, ints, int_index)
		.Call(RCRD_merge_simple_int_store, s_ints)

		# Merge Rest
		.Call(RCRD_merge_simple_dbl_store, dbls)
		.Call(RCRD_merge_simple_raw_store, raws)
		.Call(RCRD_merge_simple_str_store, strs, str_index)
		.Call(RCRD_merge_generic_store, generics, generic_index)
		.Call(RCRD_merge_stats_store)
	}
}

#' @export
sample_val <- function(type = "any") {
	if (type == "any") {
		.Call(RCRD_sample_val)
	# } else if (type == "null") {
	# 	.Call(RCRD_sample_null)
	} else if (type == "integer") {
		.Call(RCRD_sample_int)
	} else {
		stop("Trying to sample unknown type")
	}
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
print_vals <- function () {
	if (size_db()) {
		for(i in 0:(size_db() - 1)) {
			print(.Call(RCRD_get_val, i))
		}
	} else {
		stop("There are no values in the database.")
	}
}

#' @export
view_db <- function() {
	if(size_db() == 0) {
		viewer <- list()
	} else {
		viewer <- lapply(seq(from=0, to=size_db()-1), function(i) .Call(RCRD_get_val, i))
	}
	viewer
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
