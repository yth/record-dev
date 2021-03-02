test_that("test_open_db_1", {
	expect_silent(open_db_for_write())
	close_db()
})
