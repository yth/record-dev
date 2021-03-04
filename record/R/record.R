#' @export
open_db_for_write <- function(filename = "tmp.txt") {
	.Call(RCRD_open_db, filename)
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
count_val <- function(filename) {
	.Call(RCRD_count_val, filename)
}

#' @export
get_random_val <- function() {
	.Call(RCRD_get_random_val)
}
