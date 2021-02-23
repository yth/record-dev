open_db_for_write <- function(filename = "tmp.txt") {
	.Call(RCRD_open_db, filename)
}

close_db <- function(file) {
	.Call(RCRD_close_db)
}

add_value <- function(value) {
	.Call(RCRD_add_value, value)
}

# get_random_value <- function() {
# 	.Call(RCRD_dc2r)
# }
