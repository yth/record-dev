#' @export
open_db_for_write <- function(dir = ".") {
	.Call(RCRD_open_db, dir)
}

#' @export
open_db_for_read <- function(dir = ".") {
	.Call(RCRD_open_db, dir)
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
get_vals <- function(from, to) {
	.Call(RCRD_read_vals, from, to)
}

#' @export
get_random_val <- function() {
	.Call(RCRD_get_random_val)
}
