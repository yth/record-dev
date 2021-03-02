#' @export
open_db_for_write <- function(filename = "tmp.txt") {
	.Call(RCRD_open_db, filename)
}

#' @export
close_db <- function(file) {
	.Call(RCRD_close_db)
}

#' @export
add_value <- function(value) {
	.Call(RCRD_add_value, value)
}

#' @export
has_value <- function(value) {
	.Call(RCRD_has_value, value)
}

#' @export
get_random_value <- function() {
	.Call(RCRD_get_random_value)
}
