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
			.Call(RCRD_load_ints, paste0(db, "/ints.bin"))
			.Call(RCRD_load_indices, paste0(db, "/indices.bin"))
			.Call(RCRD_load_gbov, paste0(db, "/gbov.bin"))
		}
	} else {
		if (!create) {
			stop(paste0(db, " does not exist."))
		} else {
			dir.create(db, recursive = TRUE)

			ints = paste0(db, "/ints.bin")
			gbov = paste0(db, "/gbov.bin")
			indices = paste0(db, "/indices.bin")

			file.create(ints, gbov, indices, showWarnings = TRUE)

			.Call(RCRD_create_ints, ints)
			.Call(RCRD_create_indices, indices)
			.Call(RCRD_create_gbov, gbov)
		}
	}
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
	.Call(RCRD_report)
}
