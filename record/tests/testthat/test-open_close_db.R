test_that("test_open_close_bd_1", {
	expect_equal(open_db_for_write("test_dir"), NULL)
	expect_equal(close_db(), NULL)
})

test_that("test_open_db_for_read_1", {
	expect_equal(open_db_for_read("test_dir"), NULL)
})