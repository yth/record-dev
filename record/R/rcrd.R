open_db_for_write <- function(filename = "tmp.txt") {
	.Call(RCRD_open_db, filename)
}

close_db <- function(file) {
	.Call(RCRD_close_db, file)
}

add_value <- function(r_string_object, file) {
	.Call(RCRD_add_value, r_string_object, file)
}

# get_random_value <- function() {
# 	.Call(RCRD_dc2r)
# }
