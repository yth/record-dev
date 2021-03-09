test_that("test_open_close_bd_1", {
	expect_equal(open_db_for_write("test_db"), NULL)
	expect_equal(close_db(), NULL)
})

test_that("test_open_db_for_read_1", {
	expect_equal(open_db_for_read("test_db"), NULL)
})

test_that("test_open_db_for_read_2", {
	expect_error(open_db_for_read("test_db_2"), NULL)
})