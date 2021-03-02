test_that("test_open_close_bd_1", {
	expect_equal(open_db_for_write(), NULL)
	expect_equal(close_db(), NULL)
})
