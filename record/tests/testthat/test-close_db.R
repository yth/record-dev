test_that("test_close_bd_1", {
	file = open_db_for_write()
	expect_equal(close_db(file), NULL)
})
