test_that("test_open_close_bd_1", {
	expect_equal(open_db_for_write("test_db"), NULL)
	expect_equal(close_db(), NULL)
})

test_that("test_open_db_1", {
	expect_silent(open_db("test_db__dir_1", create = T))
})

test_that("test_open_db_2", {
	# expect_silent(open_db("test_db_dir_2", create = T))
	# expect_silent(close_db())
})

test_that("test_open_db_3", {
	# expect_silent(open_db("test_db_dir_2", create = F))
	# expect_silent(close_db())
})

