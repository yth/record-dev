record_init <- function(path_name) {
	.Call(RCRD_record_init, path_name)
}

# r2c <- function(r_object, storage) {
# 	.Call(RCRD_r2c, r_object, storage)
# }

# c2r <- function(hash, storage) {
# 	.Call(RCRD_c2r, hash, storage)
# }
