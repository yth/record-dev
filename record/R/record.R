#' @export
open_db_for_write <- function(db = ".") {
	.Call(RCRD_open_db, db)
}

#' @export
open_db_for_read <- function(db = ".") {
	files = list.files(db)
	if (length(files) == 2 && "gbov.bin" %in% files && "index.bin" %in% files) {
		print("Opened successfully")
	} else {
		stop("The specified database does not exist.")
	}
	# .Call(RCRD_open_db, db)
}

#' @export
# Open database specified by db_name.
# If create is True, then create the database if one does not exist.
# Otherwise, quit if the database does not exist.
# This function will only create the database in the current working directory.
open_db <- function(db_name = ".", create = False) {
	tryCatch(
		{
			setwd(db_name)
			files = list.files()
			if (length(files) == 2 &&
				"gbov.bin" %in% files &&
				"index.bin" %in% files) {
				.Call(RCRD_load_gbov, "gbov.bin")
				.Call(RCRD_load_indices, "indices.bin")
			}
		},
		error = function(c) {
			if (create) {
				dir.create(db_name)
				setwd(db_name)
				file.create("gbov.bin", showWarnings = TRUE)
				file.create("indices.bin", showWarnings = TRUE)
				.Call(RCRD_create_gbov, "gbov.bin")
				.Call(RCRD_create_indices, "indices.bin")
			} else {
				stop("Could not find the specified database.")
			}
		},
		warning = function(c) { stop("Got an unexpected warning.") },
		message = function(c) { stop("Got an unexpected message.") }
	)
}

#' @export
close_db <- function(file) {
	.Call(RCRD_close_db)
}

#' @export
add_val <- function(val) {
	.Call(RCRD_add_val, val)
}

#' @export
has_seen <- function(val) {
	.Call(RCRD_has_seen, val)
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
get_vals <- function(from, to) {
	.Call(RCRD_read_vals, from, to)
}

#' @export
get_random_val <- function() {
	.Call(RCRD_get_random_val)
}
