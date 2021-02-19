open_db_for_write <- function(filename = "db.txt") {
	.Call(RCRD_record_init, filename)
}

close_db <- function(file) {
	.Call(RCRD_record_close, file)
}

add_value <- function(r_string_object, file) {
	.Call(RCRD_r2cd, r_string_object, file)
}

# get_random_value <- function() {
# 	.Call(RCRD_dc2r)
# }

testthis <- function(rstring) {
  .Call(RCRD_testthis, rstring)
}
